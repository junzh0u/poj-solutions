// POJ 2057 - The Lost House
// Model: claude-opus-5
//
// Rooted tree at key point 1; the house sits at a uniformly random leaf.  The
// snail searches depth-first (it may not leave a fork before finishing every
// end grown from it), except that a worm at a key point c reveals whether the
// house lies in subtree(c), so a "no" lets the snail turn straight back after
// stepping onto c.  Minimise the expected distance walked.
//
// For each node v define, over subtree(v):
//   nleaf[v] = number of leaves,
//   cost[v]  = distance walked to search subtree(v) exhaustively and return to
//              v, *given the house is not there*,
//   S[v]     = sum over leaves l of the distance from v to l along the optimal
//              route, given the house is at l.
// Answer = S[1] / nleaf[1].
//
// A child c costs w(c) = 2 when it carries a worm (step on, ask, step back)
// and w(c) = 2 + cost[c] otherwise, so cost[v] = sum of w over children and is
// independent of the visiting order.  With children visited in some order,
// S[v] = sum_i ( nleaf[c_i] * (1 + sum_{j<i} w(c_j)) + S[c_i] ); only the
// prefix term depends on the order, so the classic exchange argument applies:
// visit children sorted by w/nleaf ascending.  Because cost[] is
// order-independent, each subtree can be optimised on its own.
//
// Two traps, both confirmed on the discuss board:
//  * the key points are NOT listed parent-before-child - a node's stated
//    previous key point may have a larger number - so the tree is built from
//    the parent array and traversed iteratively (also keeps a 1000-deep path
//    off the C stack);
//  * the child ordering must be compared as w_a*n_b < w_b*n_a in integers;
//    a floating-point w/n comparison is reported to fail here.  Everything is
//    integer until the single final division (S[1] and the products fit
//    comfortably in int for N <= 1000), and the result is printed with %f,
//    since %lf is reported unreliable on POJ's compilers.
//
// The statement's "a fork has at most eight branches" is not relied upon; the
// sort handles arbitrary fan-out.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

struct Item { int w, n, s; };

static bool cmpItem(const Item &a, const Item &b) {
    return a.w * b.n < b.w * a.n;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<int> par(n + 1, 0);
        vector<char> worm(n + 1, 0);
        vector<vector<int> > ch(n + 1);
        for (int i = 1; i <= n; i++) {
            int p; char c;
            if (scanf("%d %c", &p, &c) != 2) return 0;
            par[i] = p;
            worm[i] = (c == 'Y') ? 1 : 0;
        }
        for (int i = 1; i <= n; i++)
            if (par[i] >= 1 && par[i] <= n && par[i] != i)
                ch[par[i]].push_back(i);

        int root = 1;
        vector<int> order;
        order.reserve(n);
        vector<char> vis(n + 1, 0);
        vector<int> stk;
        stk.push_back(root);
        vis[root] = 1;
        while (!stk.empty()) {
            int v = stk.back(); stk.pop_back();
            order.push_back(v);
            for (size_t k = 0; k < ch[v].size(); k++) {
                int c = ch[v][k];
                if (!vis[c]) { vis[c] = 1; stk.push_back(c); }
            }
        }

        vector<int> nleaf(n + 1, 0), cost(n + 1, 0), S(n + 1, 0);
        vector<Item> items;
        for (int idx = (int)order.size() - 1; idx >= 0; idx--) {
            int v = order[idx];
            if (ch[v].empty()) { nleaf[v] = 1; cost[v] = 0; S[v] = 0; continue; }
            items.clear();
            int tot = 0;
            for (size_t k = 0; k < ch[v].size(); k++) {
                int c = ch[v][k];
                Item it;
                it.w = 2 + (worm[c] ? 0 : cost[c]);
                it.n = nleaf[c];
                it.s = S[c];
                items.push_back(it);
                tot += it.n;
            }
            sort(items.begin(), items.end(), cmpItem);
            int d = 0, sv = 0;
            for (size_t k = 0; k < items.size(); k++) {
                sv += items[k].n * (d + 1) + items[k].s;
                d += items[k].w;
            }
            nleaf[v] = tot; cost[v] = d; S[v] = sv;
        }

        printf("%.4f\n", (double)S[root] / (double)nleaf[root]);
    }
    return 0;
}
