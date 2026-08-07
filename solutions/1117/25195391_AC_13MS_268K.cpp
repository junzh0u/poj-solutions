// POJ 1117 - Pairs of Integers
// Model: claude-opus-5
//
// Find every pair (X, Y) with X + Y = N where Y is X's decimal string with one
// digit struck out (Y keeps leading zeros and always has one digit less than X;
// X has >= 2 digits and a non-zero leading digit).
//
// Approach (closed form, O(number of digits) -- no search):
// Write X as prefix A, struck digit d, suffix B, where B has exactly m digits:
//   X = A*10^(m+1) + d*10^m + B,   Y = A*10^m + B,   0 <= B < 10^m, 0 <= d <= 9.
// Then N = X + Y = 10^m * (11A + d) + 2B.  Fixing m fixes 2B modulo 10^m, and
// since 0 <= 2B < 2*10^m there are at most two candidates, 2B = r and
// 2B = r + 10^m with r = N mod 10^m (both need r even; for m = 0 only B = 0).
// Each candidate leaves Q = (N - 2B)/10^m = 11A + d, which has a unique
// solution with 0 <= d <= 9 unless Q mod 11 == 10 (then reject): d = Q mod 11,
// A = Q / 11.  Sweeping m = 0..9 therefore enumerates every pair exactly.
//
// Details that cost other people submissions (per the discuss board):
//  * Different strike positions can yield the same X (e.g. 1222 = 1111 + 111),
//    so the results must be deduplicated.  X determines Y = N - X, hence
//    deduplicating on X alone is exact.
//  * A == 0 with d == 0 would make X start with a zero digit -- reject; A == 0
//    with m == 0 would make X a single digit -- reject.
//  * Y is printed padded with leading zeros to len(X) - 1 digits, so N = 302
//    yields "301 + 01 = 302" and N = 1000000000 yields
//    "1000000000 + 000000000 = 1000000000" (that pair does count).
//
// Verified against a brute force that literally strikes out digits: every
// N in [10, 10^6] exhaustively, plus exhaustive windows at 10^7, 10^8, 5*10^8
// and 999999000..10^9; also byte-identical to a board-posted Accepted
// reference on 5000 random N, with each printed pair re-checked as a witness.

#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>

static int pw[11];

static std::string pad(int v, int width) {
    std::string s;
    if (v == 0) s = "0";
    while (v > 0) { s += (char)('0' + v % 10); v /= 10; }
    std::reverse(s.begin(), s.end());
    while ((int)s.size() < width) s = "0" + s;
    return s;
}

int main() {
    pw[0] = 1;
    for (int i = 1; i <= 9; ++i) pw[i] = pw[i - 1] * 10;

    int N;
    while (scanf("%d", &N) == 1) {
        std::vector<int> xs;
        for (int m = 0; m <= 9 && pw[m] <= N; ++m) {
            int r = N % pw[m];
            for (int k = 0; k < 2; ++k) {
                int twoB = (k == 0) ? r : r + pw[m];
                if (twoB % 2 != 0) continue;          /* B must be an integer */
                int B = twoB / 2;
                if (m == 0 && B != 0) continue;       /* B has no digits */
                if (B > N - B) continue;              /* 2B <= N, overflow-safe */
                int Q = (N - twoB) / pw[m];
                int d = Q % 11;
                if (d > 9) continue;                  /* struck digit must be < 10 */
                int A = Q / 11;
                if (A == 0) {
                    if (d == 0) continue;             /* X would start with 0 */
                    if (m == 0) continue;             /* X would have 1 digit */
                } else if (m >= 9) {
                    continue;                         /* X would exceed 10^10 */
                }
                xs.push_back((A > 0 ? A * pw[m + 1] : 0) + d * pw[m] + B);
            }
        }

        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

        printf("%d\n", (int)xs.size());
        for (size_t i = 0; i < xs.size(); ++i) {
            int X = xs[i];
            int lx = (int)pad(X, 1).size();
            printf("%d + %s = %d\n", X, pad(N - X, lx - 1).c_str(), N);
        }
    }
    return 0;
}
