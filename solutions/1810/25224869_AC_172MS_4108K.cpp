// POJ 1810 - Covering
// Model: gpt-6.1-sol
// Count mines in unit cells, build a 2D prefix sum, and scan all legal
// M-by-M windows. Ties prefer larger y, then larger x (up, then right).
// Noninteger coordinates make truncation to a unit cell exact for coverage.
#include <iostream>
#include <cstring>
using namespace std;
static int sum[1001][1001];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int k, n, m;
        cin >> k >> n >> m;
        memset(sum, 0, sizeof(sum));
        for (int i = 0; i < k; ++i) {
            double x, y;
            cin >> x >> y;
            ++sum[(int)y + 1][(int)x + 1];
        }
        for (int y = 1; y <= n; ++y)
            for (int x = 1; x <= n; ++x)
                sum[y][x] += sum[y-1][x] + sum[y][x-1] - sum[y-1][x-1];
        int best = -1, bx = 0, by = 0;
        for (int y = 0; y <= n-m; ++y)
            for (int x = 0; x <= n-m; ++x) {
                int count = sum[y+m][x+m] - sum[y][x+m]
                          - sum[y+m][x] + sum[y][x];
                if (count >= best) {
                    best = count;
                    bx = x;
                    by = y;
                }
            }
        cout << bx << ' ' << by << '\n';
    }
    return 0;
}
