// POJ 1278 - BOAT
// Model: gpt-6.1-sol
// Process clients in ID order, keeping maximum profit for each occupied-day sum.
// Pack rentals from day 1: finishing on day d meets a deadline of d.
// Each client's best eligible offer is used once, or that client is skipped.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Offer { int deadline; long long money; };
int main() {
    int n;
    bool first = true;
    while (cin >> n) {
        vector<int> days(n);
        for (int i = 0; i < n; ++i) cin >> days[i];
        int m; cin >> m;
        vector< vector<Offer> > offers(n);
        for (int j = 0; j < m; ++j) {
            int id; Offer o;
            cin >> id >> o.deadline >> o.money;
            offers[id - 1].push_back(o);
        }
        vector<long long> dp(101, -1);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            vector<long long> next = dp;
            for (int t = 0; t + days[i] <= 100; ++t) {
                if (dp[t] < 0) continue;
                int finish = t + days[i];
                for (int j = 0; j < (int)offers[i].size(); ++j)
                    if (finish <= offers[i][j].deadline)
                        next[finish] = max(next[finish], dp[t] + offers[i][j].money);
            }
            dp.swap(next);
        }
        if (!first) cout << '\n';
        first = false;
        cout << *max_element(dp.begin(), dp.end()) << '\n';
    }
    return 0;
}
