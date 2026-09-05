// POJ 1429 - Alice and Bob
// Model: claude-sonnet-5
//
// Approach: The n+m given edges are the sides and non-crossing diagonals of a
// convex polygon whose boundary order is unknown; reconstruct the boundary
// Hamiltonian cycle using only the edge list (no embedding/coordinates).
//
// Each vertex needs exactly 2 true (side) edges. Track needed[v] (how many
// of its 2 true neighbors are still unconfirmed, init 2) and deg[v] (how
// many incident edges are still "live"/unclassified). Whenever
// deg[v] == needed[v] > 0, every remaining live edge at v must be a true
// side (if it weren't, a genuine still-unconfirmed side of v would also be
// live, since a true side stays live for both endpoints until confirmed,
// making deg[v] > needed[v]). Symmetrically, once needed[v] hits 0 (v's two
// true sides are already confirmed), any further live edge at v cannot be a
// true side (a vertex has only 2), so it is a dead diagonal and is
// discarded, decrementing the other endpoint's degree and possibly
// cascading further finalize/cleanup there.
//
// That local degree rule alone is not sufficient: once several confirmed
// true sides have chained several vertices into one contiguous run ("chain")
// of the final cycle, a live edge directly between the run's two loose ends
// would close it into a premature short cycle -- impossible unless the run
// already spans all n vertices. Such an edge can otherwise sit at a vertex
// whose degree never happens to match its remaining need, deadlocking the
// simple rule (this happens inside fully triangulated sub-polygons with no
// currently-visible plain ear). So a union-find tracks, for each chain, its
// two loose ("open") ends; each time two chains are joined by a newly
// confirmed side, the new pair of open ends is checked for a direct live
// edge, which -- if the joined chain does not yet cover all n vertices -- is
// immediately discarded as a dead diagonal (this is always safe: such an
// edge is live for the very first time only when this exact open-end pair
// first exists, since edges never become live again once dead).
//
// Driving both rules with a worklist is O((n+m) alpha(n)). Output: start at
// 1, then its smaller neighbor, then walk the reconstructed cycle.
//
// No statement ambiguity found. Verified against the sample and extensively
// against a randomized differential test (random convex-polygon vertex
// permutation + random non-crossing diagonal subset, including forced full
// triangulations, reconstructed and compared to the planted order up to
// rotation/reflection) for n up to 500, plus a stress input near n=10000.
// An early, purely-local-degree version of the rule (without the union-find
// premature-closing check) passed the sample but deadlocked on stress
// inputs with fully triangulated sub-polygons; the mutation was caught by
// the randomized test, not by the sample.

#include <cstdio>
#include <vector>
using namespace std;

static vector<int> parentDSU, openA, openB, sizeDSU;

// Edge keys fit comfortably in a plain (32-bit) int (n <= 10000), so a
// custom open-addressing hash table keyed on int avoids relying on
// <unordered_map>/long long, which are not guaranteed on POJ's older
// compilers. Since the table is built once from the fixed input edges and
// never needs entries removed (liveness is tracked separately via
// edgeDead[]), a simple linear-probing table with no delete support is
// enough for O(1) average lookups.
static const int EMPTY_KEY = -1;
static vector<int> hkey, hval;
static int hmask;

static inline int edgeKey(int a, int b) {
    if (a > b) { int t = a; a = b; b = t; }
    return a * 20001 + b;
}

static inline unsigned mixHash(int key) {
    unsigned h = (unsigned)key;
    h ^= h >> 16; h *= 0x85ebca6bu;
    h ^= h >> 13; h *= 0xc2b2ae35u;
    h ^= h >> 16;
    return h;
}

static void hashInsert(int key, int val) {
    unsigned idx = mixHash(key) & (unsigned)hmask;
    while (hkey[idx] != EMPTY_KEY) idx = (idx + 1) & (unsigned)hmask;
    hkey[idx] = key; hval[idx] = val;
}

// returns edge id, or -1 if no such edge exists at all in the input
static int hashFind(int key) {
    unsigned idx = mixHash(key) & (unsigned)hmask;
    while (hkey[idx] != EMPTY_KEY) {
        if (hkey[idx] == key) return hval[idx];
        idx = (idx + 1) & (unsigned)hmask;
    }
    return -1;
}

static int find(int x) {
    int r = x;
    while (parentDSU[r] != r) r = parentDSU[r];
    while (parentDSU[x] != r) { int nx = parentDSU[x]; parentDSU[x] = r; x = nx; }
    return r;
}

int main(){
    int d;
    if (scanf("%d", &d) != 1) return 0;
    while (d--) {
        int n, m;
        scanf("%d %d", &n, &m);
        int totalEdges = n + m;
        vector<vector<pair<int,int> > > adj(n + 1);
        vector<char> edgeDead(totalEdges, 0);
        vector<int> deg(n + 1, 0);
        int hcap = 16;
        while (hcap < totalEdges * 4 + 8) hcap <<= 1;
        hmask = hcap - 1;
        hkey.assign(hcap, EMPTY_KEY);
        hval.assign(hcap, -1);
        for (int i = 0; i < totalEdges; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            adj[a].push_back(make_pair(b, i));
            adj[b].push_back(make_pair(a, i));
            deg[a]++; deg[b]++;
            hashInsert(edgeKey(a, b), i);
        }
        vector<int> needed(n + 1, 2);
        vector<int> finalNb(2 * (n + 1), 0);
        vector<int> finalCount(n + 1, 0);
        vector<int> ptr(n + 1, 0);

        parentDSU.assign(n + 1, 0);
        openA.assign(n + 1, 0);
        openB.assign(n + 1, 0);
        sizeDSU.assign(n + 1, 1);
        for (int v = 0; v <= n; ++v) { parentDSU[v] = v; openA[v] = v; openB[v] = v; }

        vector<int> q;
        vector<char> inq(n + 1, 0);
        q.reserve((size_t)n * 2 + 10);

        for (int v = 1; v <= n; ++v) {
            if (deg[v] == needed[v]) { q.push_back(v); inq[v] = 1; }
        }

        bool finished = false;
        size_t qi = 0;
        while (qi < q.size()) {
            int v = q[qi++];
            inq[v] = 0;
            if (finished) continue;
            if (deg[v] == 0) continue;
            int mode; // 1 = finalize, 0 = cleanup
            if (needed[v] == 0) mode = 0;
            else if (deg[v] == needed[v]) mode = 1;
            else continue;

            while (ptr[v] < (int)adj[v].size() && edgeDead[adj[v][ptr[v]].second]) ptr[v]++;
            if (ptr[v] >= (int)adj[v].size()) continue;
            int nbr = adj[v][ptr[v]].first;
            int eid = adj[v][ptr[v]].second;
            ptr[v]++;

            if (mode == 0) {
                edgeDead[eid] = 1;
                deg[v]--; deg[nbr]--;
                if (deg[v] > 0 && (needed[v] == 0 || deg[v] == needed[v]) && !inq[v]) { inq[v]=1; q.push_back(v); }
                if (deg[nbr] > 0 && (needed[nbr] == 0 || deg[nbr] == needed[nbr]) && !inq[nbr]) { inq[nbr]=1; q.push_back(nbr); }
            } else {
                int ru = find(v), rv = find(nbr);
                if (ru == rv) {
                    if (sizeDSU[ru] == n) {
                        edgeDead[eid] = 1;
                        deg[v]--; deg[nbr]--;
                        finalNb[2*v + finalCount[v]] = nbr; finalCount[v]++;
                        finalNb[2*nbr + finalCount[nbr]] = v; finalCount[nbr]++;
                        needed[v]--; needed[nbr]--;
                        finished = true;
                    } else {
                        // premature closing edge - cannot be a true side; discard
                        edgeDead[eid] = 1;
                        deg[v]--; deg[nbr]--;
                        if (deg[v] > 0 && (needed[v] == 0 || deg[v] == needed[v]) && !inq[v]) { inq[v]=1; q.push_back(v); }
                        if (deg[nbr] > 0 && (needed[nbr] == 0 || deg[nbr] == needed[nbr]) && !inq[nbr]) { inq[nbr]=1; q.push_back(nbr); }
                    }
                } else {
                    edgeDead[eid] = 1;
                    deg[v]--; deg[nbr]--;
                    finalNb[2*v + finalCount[v]] = nbr; finalCount[v]++;
                    finalNb[2*nbr + finalCount[nbr]] = v; finalCount[nbr]++;
                    needed[v]--; needed[nbr]--;

                    int other_v = (openA[ru] == v) ? openB[ru] : openA[ru];
                    int other_nbr = (openA[rv] == nbr) ? openB[rv] : openA[rv];

                    if (sizeDSU[ru] < sizeDSU[rv]) { int t = ru; ru = rv; rv = t; }
                    parentDSU[rv] = ru;
                    sizeDSU[ru] += sizeDSU[rv];
                    openA[ru] = other_v;
                    openB[ru] = other_nbr;

                    if (other_v != other_nbr && sizeDSU[ru] < n) {
                        // O(1) average lookup for a live edge between the new open ends
                        int xid = hashFind(edgeKey(other_v, other_nbr));
                        if (xid != -1 && !edgeDead[xid]) {
                            edgeDead[xid] = 1;
                            deg[other_v]--; deg[other_nbr]--;
                            if (deg[other_v] > 0 && (needed[other_v]==0 || deg[other_v]==needed[other_v]) && !inq[other_v]) { inq[other_v]=1; q.push_back(other_v); }
                            if (deg[other_nbr] > 0 && (needed[other_nbr]==0 || deg[other_nbr]==needed[other_nbr]) && !inq[other_nbr]) { inq[other_nbr]=1; q.push_back(other_nbr); }
                        }
                    }
                    if (deg[v] > 0 && (needed[v] == 0 || deg[v] == needed[v]) && !inq[v]) { inq[v]=1; q.push_back(v); }
                    if (deg[nbr] > 0 && (needed[nbr] == 0 || deg[nbr] == needed[nbr]) && !inq[nbr]) { inq[nbr]=1; q.push_back(nbr); }
                }
            }
        }

        vector<int> ans(n);
        ans[0] = 1;
        int a0 = finalNb[2*1+0], a1 = finalNb[2*1+1];
        int second = (a0 < a1) ? a0 : a1;
        ans[1] = second;
        int prev = 1, cur = second;
        for (int i = 2; i < n; ++i) {
            int x = finalNb[2*cur+0], y = finalNb[2*cur+1];
            int nxt = (x == prev) ? y : x;
            ans[i] = nxt;
            prev = cur; cur = nxt;
        }
        for (int i = 0; i < n; ++i) {
            printf("%d%c", ans[i], (i+1<n) ? ' ' : '\n');
        }
    }
    return 0;
}
