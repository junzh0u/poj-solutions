// POJ 1973 - Software Company
// Model: gpt-5.6-terra
// Binary-search the common deadline.  For a fixed deadline, dp[a] is the
// greatest number of second-project subprojects achievable after processing
// some employees while completing a first-project subprojects.  Giving an
// employee k first-project jobs leaves enough time for
// floor((deadline-k*x)/y) second-project jobs.
// Both projects need m independent subprojects; a worker may split the
// deadline between them, but cannot work on two subprojects at once.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Worker {
    long long first;
    long long second;
};

static bool feasible(long long deadline, const vector<Worker>& workers, int m) {
    vector<int> dp(m + 1, -1);
    dp[0] = 0;
    for (size_t i = 0; i < workers.size(); ++i) {
        vector<int> next(m + 1, -1);
        for (int doneFirst = 0; doneFirst <= m; ++doneFirst) {
            if (dp[doneFirst] < 0) continue;
            for (int addFirst = 0; doneFirst + addFirst <= m; ++addFirst) {
                if ((long long)addFirst * workers[i].first > deadline) break;
                long long availableSecond =
                    (deadline - (long long)addFirst * workers[i].first) / workers[i].second;
                int totalSecond = dp[doneFirst] +
                    (availableSecond >= m ? m : (int)availableSecond);
                if (totalSecond > m) totalSecond = m;
                int& cell = next[doneFirst + addFirst];
                if (totalSecond > cell) cell = totalSecond;
            }
        }
        dp.swap(next);
    }
    return dp[m] >= m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, m;
        cin >> n >> m;
        vector<Worker> workers(n);
        long long high = 0;
        for (int i = 0; i < n; ++i) {
            cin >> workers[i].first >> workers[i].second;
            high = max(high, (workers[i].first + workers[i].second) * (long long)m);
        }
        long long low = 0;
        while (low < high) {
            long long middle = low + (high - low) / 2;
            if (feasible(middle, workers, m)) high = middle;
            else low = middle + 1;
        }
        cout << low << '\n';
    }
    return 0;
}
