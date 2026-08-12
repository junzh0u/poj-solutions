// POJ 2444 - Partition a Matrix
// Model: gpt-5.6-terra
// Enumerate both parallel-strip cuts and the four orientations of a T-shaped
// pair of perpendicular cuts, using a two-dimensional prefix-sum table.
// The largest possible sum is 500 * 500 * 65535, so 64-bit arithmetic is
// required; this source is submitted with G++ (language 0).
#include <iostream>
#include <algorithm>
using namespace std;

typedef long long ll;

static ll score(ll a, ll b, ll c) {
    ll lo = min(a, min(b, c));
    ll hi = max(a, max(b, c));
    return 2 * (hi - lo);
}

int main() {
    int m, n;
    while (cin >> m >> n && (m || n)) {
        static ll sum[501][501];
        int i, j;
        for (i = 0; i <= m; ++i)
            sum[i][0] = 0;
        for (j = 0; j <= n; ++j)
            sum[0][j] = 0;
        for (i = 1; i <= m; ++i) {
            for (j = 1; j <= n; ++j) {
                ll x;
                cin >> x;
                sum[i][j] = x + sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
            }
        }

        ll total = sum[m][n];
        ll ans = total * 2;
        for (i = 1; i < m; ++i) {
            for (j = i + 1; j < m; ++j)
                ans = min(ans, score(sum[i][n], sum[j][n] - sum[i][n], total - sum[j][n]));
        }
        for (i = 1; i < n; ++i) {
            for (j = i + 1; j < n; ++j)
                ans = min(ans, score(sum[m][i], sum[m][j] - sum[m][i], total - sum[m][j]));
        }

        for (i = 1; i < m; ++i) {
            for (j = 1; j < n; ++j) {
                ll tl = sum[i][j];
                ll tr = sum[i][n] - tl;
                ll bl = sum[m][j] - tl;
                ll br = total - tl - tr - bl;
                ans = min(ans, score(tl, bl, tr + br));
                ans = min(ans, score(tr, br, tl + bl));
                ans = min(ans, score(tl, tr, bl + br));
                ans = min(ans, score(bl, br, tl + tr));
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
