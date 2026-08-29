// POJ 2793 - Cactus
// Model: claude-sonnet-5
// A cactus is a connected graph where every edge lies on at most one simple
// cycle, i.e. every biconnected component is either a single bridge edge or
// a simple cycle. Decompose the graph into biconnected components with the
// classic iterative Tarjan edge-stack algorithm (recursion avoided since n
// can reach 20000). For each component with e edges and v vertices: e==1 is
// a bridge (must stay, factor 1); e==v is a simple cycle of length e; any
// other shape (e>v) means the graph is not a cactus at all (cactusness 0).
// If the graph is disconnected it is not a cactus either (0), matching the
// statement's third picture.
//
// Cactusness = product over all cycle components of (cycleLength+1). A
// spanning subgraph that stays a cactus must, inside every cycle block,
// keep all of that cycle's vertices in one connected piece (a cactus's
// non-cut vertices have no route to the rest of the graph except through
// the cycle, and two cut vertices of the same cycle cannot be connected any
// other way either, else some edge would lie on two cycles). For a cycle of
// length L that means: keep every edge (1 way) or drop exactly one edge,
// leaving a Hamiltonian path over the cycle (L ways) -- dropping 2+ edges
// always disconnects some vertex. So each cycle independently contributes a
// factor of L+1, and the total is their product. Verified against the
// sample (a 6-cycle and a 4-cycle => 7*5=35) and against a hand +
// brute-force check on small random cactuses/non-cactuses (enumerating all
// 2^m edge subsets for m<=18). The discuss board's only substantive note is
// that this problem's judge data is weak about cycles sharing a single cut
// vertex; that case falls out of the biconnected-component decomposition
// automatically and needed no special-casing here. The answer can be very
// large ("cactusness can be quite a large number"), so it's accumulated in
// a base-1e9 bignum rather than any fixed-width integer.
//
// n<=20000, m<=1000 paths of length<=1000 => at most ~1e6 edges to read,
// but any input that is actually a cactus has O(n) edges, so the bignum
// work stays small; the read/BCC pass is a single O(V+E) sweep either way.

#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

static int n, m;
static int *edgeU, *edgeV;
static int numEdges = 0, edgeCap;

static int *head_, *adjTo, *adjEid;

static bool *visited;
static int *disc, *low;
static int *parentEdge;
static bool *usedEdge;
static int *curIt;

struct Big {
    vector<unsigned int> d; // base 1e9, little endian
    Big() { d.push_back(1); }
    void mulSmall(long long f) {
        long long carry = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            long long cur = (long long)d[i] * f + carry;
            d[i] = (unsigned int)(cur % 1000000000LL);
            carry = cur / 1000000000LL;
        }
        while (carry) {
            d.push_back((unsigned int)(carry % 1000000000LL));
            carry /= 1000000000LL;
        }
    }
    void print() const {
        printf("%u", d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) printf("%09u", d[i]);
        printf("\n");
    }
};

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 0;

    edgeCap = 1000 * 1000 + 10;
    edgeU = (int*)malloc(sizeof(int) * edgeCap);
    edgeV = (int*)malloc(sizeof(int) * edgeCap);

    vector<int> pathBuf;
    for (int i = 0; i < m; ++i) {
        int k;
        scanf("%d", &k);
        pathBuf.resize(k);
        for (int j = 0; j < k; ++j) scanf("%d", &pathBuf[j]);
        for (int j = 0; j + 1 < k; ++j) {
            if (numEdges >= edgeCap) {
                edgeCap *= 2;
                edgeU = (int*)realloc(edgeU, sizeof(int) * edgeCap);
                edgeV = (int*)realloc(edgeV, sizeof(int) * edgeCap);
            }
            edgeU[numEdges] = pathBuf[j];
            edgeV[numEdges] = pathBuf[j + 1];
            numEdges++;
        }
    }

    int *deg = (int*)calloc(n + 2, sizeof(int));
    for (int e = 0; e < numEdges; ++e) {
        deg[edgeU[e]]++;
        deg[edgeV[e]]++;
    }
    head_ = (int*)malloc(sizeof(int) * (n + 2));
    int *pos = (int*)malloc(sizeof(int) * (n + 2));
    int total = 0;
    for (int v = 1; v <= n; ++v) { head_[v] = total; pos[v] = total; total += deg[v]; }
    head_[n + 1] = total;
    adjTo = (int*)malloc(sizeof(int) * (total > 0 ? total : 1));
    adjEid = (int*)malloc(sizeof(int) * (total > 0 ? total : 1));
    for (int e = 0; e < numEdges; ++e) {
        int u = edgeU[e], v = edgeV[e];
        adjTo[pos[u]] = v; adjEid[pos[u]] = e; pos[u]++;
        adjTo[pos[v]] = u; adjEid[pos[v]] = e; pos[v]++;
    }
    free(deg); free(pos);

    visited = (bool*)calloc(n + 2, sizeof(bool));
    disc = (int*)malloc(sizeof(int) * (n + 2));
    low = (int*)malloc(sizeof(int) * (n + 2));
    parentEdge = (int*)malloc(sizeof(int) * (n + 2));
    usedEdge = (bool*)calloc(numEdges > 0 ? numEdges : 1, sizeof(bool));
    curIt = (int*)malloc(sizeof(int) * (n + 2));

    int *edgeStack = (int*)malloc(sizeof(int) * (numEdges > 0 ? numEdges : 1));
    int edgeStackTop = 0;

    int *dfsStack = (int*)malloc(sizeof(int) * (n + 2));
    int dfsTop = 0;

    int *vertStamp = (int*)calloc(n + 2, sizeof(int));
    int curBcc = 0;

    int timer = 0;
    int numRoots = 0;
    bool notCactus = false;

    Big answer;

    for (int s = 1; s <= n; ++s) {
        if (visited[s]) continue;
        numRoots++;
        visited[s] = true;
        disc[s] = low[s] = ++timer;
        parentEdge[s] = -1;
        curIt[s] = head_[s];
        dfsStack[dfsTop++] = s;

        while (dfsTop > 0) {
            int v = dfsStack[dfsTop - 1];
            if (curIt[v] < head_[v + 1]) {
                int idx = curIt[v]++;
                int to = adjTo[idx];
                int eid = adjEid[idx];
                if (usedEdge[eid]) continue;
                usedEdge[eid] = true;
                if (!visited[to]) {
                    visited[to] = true;
                    disc[to] = low[to] = ++timer;
                    parentEdge[to] = eid;
                    curIt[to] = head_[to];
                    edgeStack[edgeStackTop++] = eid;
                    dfsStack[dfsTop++] = to;
                } else {
                    edgeStack[edgeStackTop++] = eid;
                    if (disc[to] < low[v]) low[v] = disc[to];
                }
            } else {
                dfsTop--;
                if (dfsTop > 0) {
                    int u = dfsStack[dfsTop - 1];
                    if (low[v] < low[u]) low[u] = low[v];
                    if (low[v] >= disc[u]) {
                        curBcc++;
                        int cntE = 0, cntV = 0;
                        int pe = parentEdge[v];
                        for (;;) {
                            int e = edgeStack[--edgeStackTop];
                            cntE++;
                            int a = edgeU[e], b = edgeV[e];
                            if (vertStamp[a] != curBcc) { vertStamp[a] = curBcc; cntV++; }
                            if (vertStamp[b] != curBcc) { vertStamp[b] = curBcc; cntV++; }
                            if (e == pe) break;
                        }
                        if (cntE == 1) {
                            // bridge, factor 1
                        } else if (cntE == cntV) {
                            answer.mulSmall(cntE + 1);
                        } else {
                            notCactus = true;
                        }
                    }
                }
            }
        }
    }

    if (numRoots > 1) notCactus = true;

    if (notCactus) printf("0\n");
    else answer.print();

    return 0;
}
