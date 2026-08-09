// POJ 2961 - Sylvester construction
// Model: claude-sonnet-5
//
// The Sylvester recursive construction H_{2n} = [[H_n, H_n], [H_n, -H_n]]
// gives, for any power-of-two size and any 0-based row r / column c that
// lie inside that matrix, the closed form entry(r, c) = (-1)^popcount(r & c).
// The value never depends on n itself (only that n is a power of two large
// enough to contain r and c) because the recursion always keeps the
// existing matrix as the fixed top-left block when doubling the size, so
// n is a red herring in the input (confirmed on the discuss board: "n
// is useless"). Verified this formula by hand against both non-trivial
// sample matrices (n=4 sub-block and derived from the n=2 base case).
//
// Board pitfall: many WAs came from swapping x/y with row/column. The
// board settles it explicitly: y (paired with h) is the row index, x
// (paired with w) is the column index.
//
// n, x, y can be as large as 2^62, which needs 64-bit arithmetic; POJ's
// language 4 (old MSVC) has no long long, so this is submitted under
// G++ (language 0) using "long long" and cin/cout (avoids any %lld /
// %I64d format-specifier pitfalls noted on old MinGW builds).
#include <cstdio>
#include <iostream>
using namespace std;

typedef long long ll;

int popcount_parity(ll v) {
    int p = 0;
    while (v) {
        p ^= (int)(v & 1);
        v >>= 1;
    }
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        ll n, x, y, w, h;
        cin >> n >> x >> y >> w >> h;
        (void)n;
        for (ll dy = 0; dy < h; ++dy) {
            ll row = y + dy;
            for (ll dx = 0; dx < w; ++dx) {
                ll col = x + dx;
                ll a = row & col;
                int parity = popcount_parity(a);
                if (dx) cout << ' ';
                cout << (parity ? -1 : 1);
            }
            cout << '\n';
        }
        cout << '\n';
    }
    return 0;
}
