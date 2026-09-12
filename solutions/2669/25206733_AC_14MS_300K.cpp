// POJ 2669 - Explore the Pirate's Cave
// Model: claude-sonnet-5
//
// The cave is a tree whose leaves (degree-1 rooms) are exactly the jewellery
// rooms; all other rooms have degree >= 2. We are given only the pairwise
// distance matrix between the N jewellery rooms (leaves), and must find the
// minimum length closed walk from room 1 that visits every jewellery room,
// and the number of distinct visiting orders achieving that minimum (mod
// 2005 -- note that "output the value if < 2005, else output it mod 2005"
// is simply "output value mod 2005" since for value < 2005 the remainder
// equals the value itself).
//
// Approach:
//  1. Reconstruct the weighted tree topology from the leaf distance matrix
//     using the standard Neighbor-Joining recurrence. NJ is exact (not
//     heuristic) whenever the input matrix truly is additive (comes from a
//     tree), which is guaranteed here, so it recovers the true tree exactly
//     (a fully resolved binary unrooted tree; higher-degree branch points
//     in the original cave appear as chains of binary NJ nodes joined by
//     zero-length edges). Edge lengths can be non-integer half-integers
//     (confirmed by the discuss board), so all arithmetic uses doubles.
//  2. Contract every zero-length edge (union-find) to recover the true
//     branching structure (nodes with >= 2 children are the actual
//     branch/jewellery-adjacent junctions; degree-2 pass-through rooms
//     never appear as real branch points and contribute a trivial factor).
//  3. The minimum walk length is exactly 2*W, where W is the sum of all
//     edge weights of the tree: visiting every leaf and returning to the
//     start requires traversing every edge exactly twice (each edge is a
//     cut separating some required leaves from room 1), and this bound is
//     achieved by any depth-first order that completes each child subtree
//     before moving to the next.
//  4. The number of distinct optimal visiting orders is the product, over
//     every node of the tree rooted at room 1, of (number of children)!:
//     at each branch point the DFS may recurse into its children subtrees
//     in any order, and these choices are independent and exhaustive.
//
// Verification: reproduces the statement's sample (12 2) and three
// separating cases posted on the discuss board with known answers,
// including the N=4 case flagged as having a half-integer edge (6 4), a
// N=10 case (26 875), a N=25 case (74 847), and a N=12 case (88 1110).
// Also checked by exhaustive brute force (try all permutations of the
// other N-1 leaves) on hundreds of random trees with mixed binary/
// higher-degree branch points and injected zero/half-integer edges.

#include <cstdio>
#include <cmath>
#include <vector>
#include <utility>
using namespace std;

static const double EPS = 1e-6;
static const int MOD = 2005;

static int ufParent[70];
static int ufFind(int x) {
    while (ufParent[x] != x) {
        ufParent[x] = ufParent[ufParent[x]];
        x = ufParent[x];
    }
    return x;
}
static void ufUnion(int a, int b) {
    a = ufFind(a);
    b = ufFind(b);
    if (a != b) ufParent[a] = b;
}

static vector<pair<int, double> > adj[70];
static int fact[35];

static int dfsCount(int node, int parent, int &prodMod) {
    int children = 0;
    for (size_t i = 0; i < adj[node].size(); i++) {
        int to = adj[node][i].first;
        if (to == parent) continue;
        children++;
        dfsCount(to, node, prodMod);
    }
    prodMod = (prodMod * fact[children]) % MOD;
    return children;
}

int main() {
    fact[0] = 1;
    for (int i = 1; i <= 30; i++) fact[i] = (fact[i - 1] * i) % MOD;

    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<vector<double> > D(n, vector<double>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int x;
                scanf("%d", &x);
                D[i][j] = x;
            }

        vector<int> eu, ev;
        vector<double> ew;

        vector<int> active(n);
        for (int i = 0; i < n; i++) active[i] = i;
        vector<vector<double> > cur = D;
        int nextId = n;

        while ((int)active.size() > 2) {
            int m = (int)active.size();
            vector<double> total(m, 0.0);
            for (int i = 0; i < m; i++) {
                double s = 0;
                for (int j = 0; j < m; j++) s += cur[i][j];
                total[i] = s;
            }
            double bestQ = 1e18;
            int bi = -1, bj = -1;
            for (int i = 0; i < m; i++)
                for (int j = i + 1; j < m; j++) {
                    double q = (double)(m - 2) * cur[i][j] - total[i] - total[j];
                    if (q < bestQ - 1e-9) {
                        bestQ = q;
                        bi = i;
                        bj = j;
                    }
                }

            double denom = (double)(m - 2);
            double li = 0.5 * (cur[bi][bj] + (total[bi] - total[bj]) / denom);
            double lj = cur[bi][bj] - li;
            if (li < 0 && li > -1e-6) li = 0;
            if (lj < 0 && lj > -1e-6) lj = 0;

            int newId = nextId++;
            eu.push_back(newId);
            ev.push_back(active[bi]);
            ew.push_back(li);
            eu.push_back(newId);
            ev.push_back(active[bj]);
            ew.push_back(lj);

            vector<int> oldIndexOfNew;
            for (int k = 0; k < m; k++) {
                if (k == bi || k == bj) continue;
                oldIndexOfNew.push_back(k);
            }
            int mm = (int)oldIndexOfNew.size();
            vector<vector<double> > newCur(mm + 1, vector<double>(mm + 1, 0.0));
            for (int a = 0; a < mm; a++)
                for (int b = 0; b < mm; b++)
                    newCur[a][b] = cur[oldIndexOfNew[a]][oldIndexOfNew[b]];
            for (int a = 0; a < mm; a++) {
                int k = oldIndexOfNew[a];
                double val = 0.5 * (cur[bi][k] + cur[bj][k] - cur[bi][bj]);
                newCur[a][mm] = val;
                newCur[mm][a] = val;
            }

            vector<int> newActive;
            for (int a = 0; a < mm; a++) newActive.push_back(active[oldIndexOfNew[a]]);
            newActive.push_back(newId);

            active = newActive;
            cur = newCur;
        }

        {
            double w = cur[0][1];
            eu.push_back(active[0]);
            ev.push_back(active[1]);
            ew.push_back(w);
        }

        // Only contract a zero-length edge when BOTH endpoints are synthetic
        // (Neighbor-Joining) internal nodes -- such an edge represents an
        // unresolved binary split of what is really one higher-degree
        // branch point in the cave, and must be merged so its children
        // count together. A zero-length edge touching an ORIGINAL leaf
        // (a real jewellery room, id < n) must be kept: visiting that
        // jewellery room costs nothing extra from its parent, so it can be
        // freely interleaved with the parent's other children in any
        // order -- exactly the same freedom an ordinary positive-weight
        // child has -- so it must still be counted as a distinct child.
        int totalNodes = nextId;
        for (int i = 0; i < totalNodes; i++) ufParent[i] = i;
        for (size_t i = 0; i < ew.size(); i++)
            if (fabs(ew[i]) < EPS && eu[i] >= n && ev[i] >= n) ufUnion(eu[i], ev[i]);

        for (int i = 0; i < totalNodes; i++) adj[i].clear();
        double W = 0;
        for (size_t i = 0; i < ew.size(); i++) {
            W += ew[i];
            int ru = ufFind(eu[i]);
            int rv = ufFind(ev[i]);
            if (ru == rv) continue;  // contracted internal-internal zero edge
            adj[ru].push_back(make_pair(rv, ew[i]));
            adj[rv].push_back(make_pair(ru, ew[i]));
        }

        int prodMod = 1;
        int root = ufFind(0);
        dfsCount(root, -1, prodMod);

        int dist = (int)floor(2.0 * W + 0.5);
        printf("%d %d\n", dist, prodMod);
    }
    return 0;
}
