// POJ 3021 - e-Coins
// Model: gpt-5.6-terra
// Approach: unbounded shortest-path dynamic programming over all nonnegative
// coordinate sums (x, y) with x, y <= S.  The requested e-modulus is met
// precisely by states satisfying x*x + y*y == S*S.
// Input has an explicit case count; formatted extraction intentionally skips
// the blank line between cases.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int m, s;
        cin >> m >> s;
        vector<int> cx(m), cy(m);
        int k;
        for (k = 0; k < m; ++k) cin >> cx[k] >> cy[k];

        const int INF = 1000000;
        vector< vector<int> > dp(s + 1, vector<int>(s + 1, INF));
        dp[0][0] = 0;
        int x, y;
        for (x = 0; x <= s; ++x) {
            for (y = 0; y <= s; ++y) {
                if (dp[x][y] == INF) continue;
                for (k = 0; k < m; ++k) {
                    int nx = x + cx[k];
                    int ny = y + cy[k];
                    if (nx <= s && ny <= s && (cx[k] != 0 || cy[k] != 0)) {
                        dp[nx][ny] = min(dp[nx][ny], dp[x][y] + 1);
                    }
                }
            }
        }

        int answer = INF;
        for (x = 0; x <= s; ++x) {
            for (y = 0; y <= s; ++y) {
                if (x * x + y * y == s * s) answer = min(answer, dp[x][y]);
            }
        }
        if (answer == INF) cout << "not possible\n";
        else cout << answer << '\n';
    }
    return 0;
}
