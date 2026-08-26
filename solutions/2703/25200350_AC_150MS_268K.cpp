// POJ 2703 - Maximum Area Covered by Rectangles
// Model: gpt-5.6-terra
// Each equal-width input group is reduced to its two greatest heights.  The
// greatest rectangle is placed horizontally and the second greatest vertically.
// In increasing-width order, replacing that choice swaps a gain over a later
// interval of length width for one over the shorter gap to the preceding width,
// so it cannot improve the union.  The union of the resulting origin-anchored
// rectangles is then evaluated directly by its 10000-column skyline.
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int n;
    while (cin >> n && n != -1) {
        map<int, vector<int> > groups;
        for (int i = 0; i < n; ++i) {
            int w, h;
            cin >> w >> h;
            groups[w].push_back(h);
        }
        vector<int> skyline(10001, 0);
        for (map<int, vector<int> >::iterator it = groups.begin();
             it != groups.end(); ++it) {
            int w = it->first;
            vector<int>& hs = it->second;
            sort(hs.begin(), hs.end(), greater<int>());
            int first = hs[0];
            int second = hs[1];
            for (int x = 0; x < first; ++x)
                skyline[x] = max(skyline[x], w);
            for (int x = 0; x < w; ++x)
                skyline[x] = max(skyline[x], second);
        }
        int answer = 0;
        for (int x = 0; x < 10000; ++x)
            answer += skyline[x];
        cout << answer << '\n';
    }
    return 0;
}
