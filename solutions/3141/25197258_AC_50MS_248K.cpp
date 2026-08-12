// POJ 3141 - Distant Galaxy
// Model: gpt-5.6-terra
// Enumerate the two x coordinates.  For each pair, scan all y-coordinate
// pairs using prefix sums for the stars on the vertical sides.
// Equal coordinates represent the opposite side being moved infinitesimally
// away, which covers rectangles having stars on only one such side.
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, tc = 0;
    while (cin >> n && n) {
        vector<int> px(n), py(n), xs, ys;
        for (int i = 0; i < n; ++i) {
            cin >> px[i] >> py[i];
            xs.push_back(px[i]);
            ys.push_back(py[i]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        const int nx = (int)xs.size(), ny = (int)ys.size();
        vector< vector<int> > cnt(nx, vector<int>(ny, 0));
        for (int i = 0; i < n; ++i) {
            int x = (int)(lower_bound(xs.begin(), xs.end(), px[i]) - xs.begin());
            int y = (int)(lower_bound(ys.begin(), ys.end(), py[i]) - ys.begin());
            ++cnt[x][y];
        }

        int answer = 0;
        for (int xl = 0; xl < nx; ++xl) {
            for (int xr = xl; xr < nx; ++xr) {
                vector<int> side(ny, 0), row(ny, 0), pref(ny + 1, 0);
                for (int y = 0; y < ny; ++y) {
                    side[y] = cnt[xl][y];
                    if (xl != xr) side[y] += cnt[xr][y];
                    for (int x = xl; x <= xr; ++x) row[y] += cnt[x][y];
                    pref[y + 1] = pref[y] + side[y];
                    answer = max(answer, row[y]);
                }
                for (int yb = 0; yb < ny; ++yb) {
                    for (int yt = yb + 1; yt < ny; ++yt) {
                        int total = pref[yt + 1] - pref[yb]
                                  + row[yb] - side[yb]
                                  + row[yt] - side[yt];
                        answer = max(answer, total);
                    }
                }
            }
        }
        cout << "Case " << ++tc << ": " << answer << '\n';
    }
    return 0;
}
