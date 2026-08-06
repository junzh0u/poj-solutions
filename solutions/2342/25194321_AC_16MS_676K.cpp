// POJ 2342 - Anniversary party
//
// Classic maximum-weight independent set on a tree. Root the supervisor
// tree at the employee with no supervisor (found by scanning, not assumed
// to be node 1). For each node u define:
//   dp1[u] = best sum of a guest list restricted to u's subtree that
//            includes u          = rating[u] + sum(dp0[child])
//   dp0[u] = best sum restricted to u's subtree that excludes u
//                                 = sum(max(dp0[child], dp1[child]))
// Answer is max(dp0[root], dp1[root]). Because dp0 always allows every
// child to be excluded too, the recurrence naturally permits an empty
// guest list, so all-negative ratings correctly yield 0 rather than
// forcing a pick.
//
// N is up to 6000, so a naive recursive post-order DFS is depth-safe in
// practice, but it's done iteratively here to avoid relying on stack
// depth at all: one pass with an explicit stack builds a visit order
// where every node precedes its children, and processing that order in
// reverse guarantees each node's children are already resolved before it
// (a valid post-order for the DP).
#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 6005;
int rating[MAXN];
vector<int> children[MAXN];
bool hasParent[MAXN];
int dp0[MAXN], dp1[MAXN];
int order_[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        for (int i = 1; i <= n; ++i) {
            children[i].clear();
            hasParent[i] = false;
        }
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &rating[i]);
        }
        int l, k;
        while (scanf("%d %d", &l, &k) == 2) {
            if (l == 0 && k == 0) break;
            children[k].push_back(l);
            hasParent[l] = true;
        }
        int root = 1;
        for (int i = 1; i <= n; ++i) {
            if (!hasParent[i]) { root = i; break; }
        }
        // iterative traversal producing an order where every child
        // appears after its parent; processing it in reverse gives
        // a valid postorder (children before parents).
        int top = 0;
        int ocount = 0;
        static int stk[MAXN];
        stk[top++] = root;
        while (top > 0) {
            int u = stk[--top];
            order_[ocount++] = u;
            for (size_t i = 0; i < children[u].size(); ++i) {
                stk[top++] = children[u][i];
            }
        }
        for (int idx = ocount - 1; idx >= 0; --idx) {
            int u = order_[idx];
            int sum0 = 0, sum1 = rating[u];
            for (size_t i = 0; i < children[u].size(); ++i) {
                int c = children[u][i];
                sum0 += (dp0[c] > dp1[c] ? dp0[c] : dp1[c]);
                sum1 += dp0[c];
            }
            dp0[u] = sum0;
            dp1[u] = sum1;
        }
        int ans = dp0[root] > dp1[root] ? dp0[root] : dp1[root];
        printf("%d\n", ans);
    }
    return 0;
}
