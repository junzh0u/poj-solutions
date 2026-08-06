// POJ 1155 - TELE
// Model: gpt-5.6-terra
// Tree knapsack: dp[u][k] is the maximum net revenue obtained by serving
// exactly k users in u's subtree.  Combining a child charges its edge exactly
// when at least one user is selected from that child.
// A transmitter edge is unnecessary when its descendant selection is empty.
#include <cstdio>
#include <vector>
#include <algorithm>

const int NEG = -1000000000;

struct Edge {
    int to;
    int cost;
    Edge(int to_, int cost_) : to(to_), cost(cost_) {}
};

int n, m;
std::vector<Edge> tree[3005];
std::vector<int> dp[3005];
int leaves[3005];
int price[3005];

void solve(int u) {
    if (u > n - m) {
        leaves[u] = 1;
        dp[u].assign(2, NEG);
        dp[u][1] = price[u];
        return;
    }

    leaves[u] = 0;
    dp[u].assign(1, 0);
    for (std::size_t e = 0; e < tree[u].size(); ++e) {
        int v = tree[u][e].to;
        int cost = tree[u][e].cost;
        solve(v);
        std::vector<int> next(leaves[u] + leaves[v] + 1, NEG);
        for (int have = 0; have <= leaves[u]; ++have) {
            if (dp[u][have] == NEG) continue;
            for (int take = 0; take <= leaves[v]; ++take) {
                if (take > 0 && dp[v][take] == NEG) continue;
                int gain = (take == 0) ? 0 : dp[v][take] - cost;
                next[have + take] = std::max(next[have + take], dp[u][have] + gain);
            }
        }
        leaves[u] += leaves[v];
        dp[u].swap(next);
    }
}

int main() {
    if (std::scanf("%d%d", &n, &m) != 2) return 0;
    for (int u = 1; u <= n - m; ++u) {
        int k;
        std::scanf("%d", &k);
        while (k--) {
            int v, cost;
            std::scanf("%d%d", &v, &cost);
            tree[u].push_back(Edge(v, cost));
        }
    }
    for (int u = n - m + 1; u <= n; ++u) std::scanf("%d", &price[u]);
    solve(1);
    for (int answer = m; answer >= 0; --answer) {
        if (dp[1][answer] >= 0) {
            std::printf("%d\n", answer);
            return 0;
        }
    }
    return 0;
}
