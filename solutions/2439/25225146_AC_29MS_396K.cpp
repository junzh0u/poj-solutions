// POJ 2439 - Disconnect the Orange!?
// Model: gpt-6.1-sol
// Every prefix must stay connected. A minimum-edge remainder therefore
// chooses exactly one edge from each vertex v > 1 to a smaller vertex.
// Only the available colors of those edges matter, including parallel edges.
// DP records attainable counts of colors 0 and 1; color 2 is implicit.
// Time O(N^3), memory O(N^2). All counts fit in 32-bit integers.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
static bool dp[2][270][270];
int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;
    int mask[270] = {0};
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        mask[max(a, b)] |= 1 << c;
    }
    dp[0][0][0] = true;
    int cur = 0;
    for (int v = 2; v <= n; ++v) {
        int next = 1 - cur;
        memset(dp[next], 0, sizeof(dp[next]));
        int used = v - 2;
        for (int a = 0; a <= used; ++a)
            for (int b = 0; a + b <= used; ++b) {
                if (!dp[cur][a][b]) continue;
                if (mask[v] & 1) dp[next][a + 1][b] = true;
                if (mask[v] & 2) dp[next][a][b + 1] = true;
                if (mask[v] & 4) dp[next][a][b] = true;
            }
        cur = next;
    }
    int answer = 0;
    for (int a = 0; a < n; ++a)
        for (int b = 0; a + b < n; ++b)
            if (dp[cur][a][b])
                answer = max(answer, min(a, min(b, n - 1 - a - b)));
    printf("%d\n", answer);
    return 0;
}
