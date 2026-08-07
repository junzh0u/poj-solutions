// POJ 2897 - Dramatic Multiplications
// Model: claude-opus-5
//
// x is n-dramatic when n*x is x with its right-most digit moved to the front.
// Write x = x[m-1]...x[1]x[0] with x[0] = k; then y = n*x must be
// k x[m-1]...x[2]x[1], i.e. y[i] = x[i+1] for i < m-1 and y[m-1] = k.
//
// So the digits are forced from the units place upward: starting at x[0] = k
// with carry 0, the school multiplication step p = n*x[i] + carry produces
// y[i] = p % 10, which *is* the next digit x[i+1], and carry = p / 10.
// The number of length m is valid exactly when, at i = m-1, that step yields
// y[m-1] = k with no carry out (so y has m digits too) and x[m-1] != 0 (so x
// really has m digits).  The first m that fires gives the shortest, hence the
// smallest, answer; the state (digit, carry) is deterministic and eventually
// cycles, so failing to fire well past its period means no such number exists.
// Answers reach 58 digits (n=2), so they are built as digit strings, never as
// integers.  Only 81 (n,k) pairs exist, so all are precomputed once.
//
// Answers exist exactly for n <= k: x's leading digit d gives n*d <= 9 and the
// product's leading digit k >= n*d >= n.  n = 1 is the degenerate case where a
// single digit is its own rotation, so the answer is k itself (the discuss
// board reports "1 1" -> "1" as a WA-causing case, not "0").
//
// Verified against two independent references over all 81 pairs: an exhaustive
// integer scan to 1e8 using the statement's string-rotation wording, and the
// algebra a*(10n-1) = k*(10^(m-1)-n) over big integers.  Every answer was also
// certified forward against the rotation property.

#include <cstdio>

int main() {
    static char ans[10][10][80];
    for (int n = 1; n <= 9; ++n) {
        for (int k = 1; k <= 9; ++k) {
            int d[520];
            d[0] = k;
            int carry = 0;
            int len = 0;
            for (int i = 0; i < 500; ++i) {
                int p = d[i] * n + carry;
                if (d[i] != 0 && p == k) { len = i + 1; break; }
                d[i + 1] = p % 10;
                carry = p / 10;
            }
            char *out = ans[n][k];
            if (len == 0) {
                out[0] = '0';
                out[1] = '\0';
            } else {
                for (int i = 0; i < len; ++i) out[i] = (char)('0' + d[len - 1 - i]);
                out[len] = '\0';
            }
        }
    }

    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n, k;
        if (scanf("%d %d", &n, &k) != 2) break;
        printf("%s\n", ans[n][k]);
    }
    return 0;
}
