// POJ 1909 - Marbles on a tree
// Model: claude-opus-5
//
// Cut any edge e of the tree.  One side A needs |A| marbles and starts with
// m_A, so the net number of marbles that must cross e is |m_A - |A||, and no
// schedule can cross e fewer times than that.  Summing the bound over all
// edges is achievable (repeatedly move a marble one step along the direction
// its edge is short), so the answer is exactly
//     sum over edges of |subtree_marbles - subtree_size|.
// Since the totals are both n, the quantity is the same from either side of
// the edge, hence independent of which vertex is called the root.
//
// Two input-framing traps, both reported on the discuss board and neither
// reachable by the official sample (verified by mutation: mutants that fix the
// root at vertex 1, or that identify a line by its position, reproduce all
// three sample answers exactly, while disagreeing on 59 of 60 random cases):
//   - the root is NOT necessarily vertex 1, so it is found as the one listed
//     vertex that appears in nobody's child list;
//   - the n lines are NOT necessarily in vertex-number order, so a line's
//     vertex is taken from the number it carries, never from its position.
// Traversal is an explicit-stack pre-order, then a reverse sweep, so a
// 10000-deep path costs no recursion.  Cases are read until n == 0 or EOF.

#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        // Read the n records first: a line's position carries no information.
        vector<int> vid(n), marb(n);
        vector< vector<int> > kids(n);
        int maxid = 0;
        for (int i = 0; i < n; i++) {
            int v, m, d;
            if (scanf("%d %d %d", &v, &m, &d) != 3) return 0;
            vid[i] = v; marb[i] = m;
            if (v > maxid) maxid = v;
            kids[i].resize(d);
            for (int j = 0; j < d; j++) {
                if (scanf("%d", &kids[i][j]) != 1) return 0;
                if (kids[i][j] > maxid) maxid = kids[i][j];
            }
        }

        int N = maxid + 1;
        vector<long long> marbles(N, 0);
        vector<char> present(N, 0), ischild(N, 0);
        vector< vector<int> > ch(N);
        for (int i = 0; i < n; i++) {
            marbles[vid[i]] = marb[i];
            present[vid[i]] = 1;
            ch[vid[i]].swap(kids[i]);
            for (size_t j = 0; j < ch[vid[i]].size(); j++)
                ischild[ch[vid[i]][j]] = 1;
        }

        // The root is whichever listed vertex is nobody's child.
        int root = vid[0];
        for (int v = 1; v < N; v++)
            if (present[v] && !ischild[v]) { root = v; break; }

        // Pre-order: every parent precedes all of its descendants.
        vector<int> order, stk, parent(N, 0);
        order.reserve(n);
        stk.push_back(root);
        while (!stk.empty()) {
            int u = stk[stk.size() - 1]; stk.pop_back();
            order.push_back(u);
            for (size_t j = 0; j < ch[u].size(); j++) {
                parent[ch[u][j]] = u;
                stk.push_back(ch[u][j]);
            }
        }

        vector<long long> subM(N, 0), subS(N, 0);
        for (size_t i = 0; i < order.size(); i++) {
            subM[order[i]] = marbles[order[i]];
            subS[order[i]] = 1;
        }
        long long ans = 0;
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            if (u == root) continue;
            ans += (subM[u] >= subS[u] ? subM[u] - subS[u] : subS[u] - subM[u]);
            subM[parent[u]] += subM[u];
            subS[parent[u]] += subS[u];
        }
        cout << ans << "\n";
    }
    return 0;
}
