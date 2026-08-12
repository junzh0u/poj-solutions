// POJ 2964 - Tourist
// Model: gpt-5.6-terra
// Dynamic programming follows the outward trip and the reversed return trip
// simultaneously on each north-west to south-east diagonal.  The two walkers
// collect a location once when they share it, and blocked squares invalidate a
// state.  The discussion board confirms this three-dimensional diagonal DP and
// highlights that row bounds must be checked before indexing the grid.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int w, h;
        cin >> w >> h;
        vector<string> city(h);
        for (int r = 0; r < h; ++r) cin >> city[r];

        const int bad = -1000000000;
        vector< vector<int> > previous(h, vector<int>(h, bad));
        previous[0][0] = city[0][0] == '*' ? 1 : 0;
        const int last = h + w - 2;
        for (int step = 1; step <= last; ++step) {
            vector< vector<int> > current(h, vector<int>(h, bad));
            int lo = max(0, step - (w - 1));
            int hi = min(h - 1, step);
            for (int r1 = lo; r1 <= hi; ++r1) {
                int c1 = step - r1;
                if (city[r1][c1] == '#') continue;
                for (int r2 = lo; r2 <= hi; ++r2) {
                    int c2 = step - r2;
                    if (city[r2][c2] == '#') continue;
                    int best = previous[r1][r2];
                    if (r1 > 0) best = max(best, previous[r1 - 1][r2]);
                    if (r2 > 0) best = max(best, previous[r1][r2 - 1]);
                    if (r1 > 0 && r2 > 0)
                        best = max(best, previous[r1 - 1][r2 - 1]);
                    if (best == bad) continue;
                    int gain = (city[r1][c1] == '*' ? 1 : 0);
                    if (r1 != r2 && city[r2][c2] == '*') ++gain;
                    current[r1][r2] = best + gain;
                }
            }
            previous.swap(current);
        }
        cout << previous[h - 1][h - 1] << '\n';
    }
    return 0;
}
