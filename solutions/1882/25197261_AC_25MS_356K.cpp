// POJ 1882 - Stamps
// Model: gpt-5.6-terra
// For each candidate denomination set, unbounded coin-change DP gives the
// smallest number of stamps for every attainable postage.  The first value
// needing more than S stamps ends the consecutive coverage interval.
// Denominations are sorted because POJ data may not preserve the stated order.
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

static int coverage(const vector<int>& d, int limit) {
    int top = limit * d.back();
    vector<int> best(top + 1, INT_MAX / 2);
    best[0] = 0;
    for (int value = 1; value <= top; ++value) {
        for (size_t j = 0; j < d.size(); ++j) {
            if (d[j] <= value && best[value - d[j]] + 1 < best[value])
                best[value] = best[value - d[j]] + 1;
        }
        if (best[value] > limit) return value - 1;
    }
    return top;
}

int main() {
    ios::sync_with_stdio(false);
    int s;
    while (cin >> s && s) {
        int n;
        cin >> n;
        vector<int> answer;
        int answerCoverage = -1;
        for (int i = 0; i < n; ++i) {
            int count;
            cin >> count;
            vector<int> d(count);
            for (int j = 0; j < count; ++j) cin >> d[j];
            sort(d.begin(), d.end());
            int currentCoverage = coverage(d, s);
            if (currentCoverage > answerCoverage ||
                (currentCoverage == answerCoverage && d.size() < answer.size()) ||
                (currentCoverage == answerCoverage && d.size() == answer.size() && d.back() < answer.back())) {
                answerCoverage = currentCoverage;
                answer = d;
            }
        }
        cout << "max coverage = " << answerCoverage << " :";
        for (size_t i = 0; i < answer.size(); ++i) cout << ' ' << answer[i];
        cout << '\n';
    }
    return 0;
}
