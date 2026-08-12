// POJ 3485 - Highway
// Model: gpt-5.6-terra
// Each village induces the closed interval of highway positions where an exit
// can serve it.  Selecting the earliest finishing uncovered interval greedily
// gives the minimum number of positions stabbing all intervals.
#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

int main() {
    int L, D, n;
    const double EPS = 1e-9;
    while (cin >> L >> D >> n) {
        vector< pair<double, double> > intervals;
        int i;
        for (i = 0; i < n; ++i) {
            int x, y;
            cin >> x >> y;
            double reach = sqrt(max(0.0, (double)D * D - (double)y * y));
            double left = max(0.0, (double)x - reach);
            double right = min((double)L, (double)x + reach);
            intervals.push_back(make_pair(right, left));
        }
        sort(intervals.begin(), intervals.end());
        int answer = 0;
        double chosen = 0.0;
        for (i = 0; i < n; ++i) {
            if (answer == 0 || intervals[i].second > chosen + EPS) {
                ++answer;
                chosen = intervals[i].first;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
