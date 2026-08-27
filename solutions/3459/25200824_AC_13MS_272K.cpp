// POJ 3459 - Projects
// Model: gpt-5.6-terra
// Dynamic programming over projects and the total assigned employees.  For an
// allocation of j employees, the project pays their salaries only on success.
// An unstaffed project therefore still contributes its certain punishment.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int m, n;
        long long salary;
        cin >> m >> n >> salary;
        vector<long long> dp(n + 1, -(1LL << 60));
        dp[0] = 0;
        for (int i = 0; i < m; ++i) {
            vector<int> p(n + 1, 0);
            for (int j = 1; j <= n; ++j) cin >> p[j];
            long long reward, punishment;
            cin >> reward >> punishment;
            vector<long long> gain(n + 1);
            gain[0] = -100LL * punishment;
            for (int j = 1; j <= n; ++j) {
                gain[j] = 1LL * p[j] * (reward - salary * j)
                    - 1LL * (100 - p[j]) * punishment;
            }
            vector<long long> next(n + 1, -(1LL << 60));
            for (int used = 0; used <= n; ++used) {
                if (dp[used] == -(1LL << 60)) continue;
                for (int j = 0; used + j <= n; ++j) {
                    next[used + j] = max(next[used + j], dp[used] + gain[j]);
                }
            }
            dp.swap(next);
        }
        long long best = *max_element(dp.begin(), dp.end());
        cout << best << '\n';
        bool first = true;
        for (int j = 0; j <= n; ++j) {
            if (dp[j] == best) {
                if (!first) cout << ' ';
                cout << j;
                first = false;
            }
        }
        cout << '\n';
    }
    return 0;
}
