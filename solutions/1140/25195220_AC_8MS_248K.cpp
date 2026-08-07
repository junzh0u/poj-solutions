// POJ 1140 - Expanding Fractions
// Model: claude-opus-5
//
// Long division with remainder-cycle detection.  For n/d we iterate
// r <- 10*r mod d, emitting one digit per step and recording the index at
// which each remainder was first seen.  Hitting r == 0 means the expansion
// terminates; hitting an already-seen remainder at index j while at index i
// means the digit sequence has pre-period j and period i-j, so exactly i
// digits are printed and the answer is i-j.
//
// That this yields the *shortest* repeating pattern is not an accident of the
// simulation: r_k/d equals 0.d_{k+1}d_{k+2}..., so equal digit tails force
// equal remainders.  Any period p of the digit sequence therefore makes the
// remainder sequence p-periodic too, hence the minimal remainder cycle length
// is the minimal digit period (and likewise for the pre-period).
//
// Output formatting (the part this problem actually punishes):
//  - lines are exactly 50 characters *including* the leading '.';
//  - no newline is emitted after the final digit even when it lands exactly on
//    a 50-character boundary -- the single '\n' before the trailing sentence
//    is the only one.  Emitting both gives a blank line and a Presentation
//    Error (the discuss board is full of these), and 1/998 -- 499 digits, so
//    500 characters, exactly 10 full lines -- is the case that catches it.
//
// Input is read to EOF as well as on the "0 0" sentinel.  n % d is taken
// defensively; the statement guarantees n < d.
#include <cstdio>

static int first_at[1000];
static char digits[1100];

int main() {
    int n, d;
    while (scanf("%d %d", &n, &d) == 2) {
        if (n == 0 && d == 0) break;
        if (d == 0) break;

        int i;
        for (i = 0; i < d; ++i) first_at[i] = -1;

        int r = n % d;
        int cnt = 0;
        int period = 0;
        int terminates = 0;
        for (;;) {
            if (r == 0) { terminates = 1; break; }
            if (first_at[r] != -1) { period = cnt - first_at[r]; break; }
            first_at[r] = cnt;
            r *= 10;
            digits[cnt++] = (char)('0' + r / d);
            r %= d;
        }

        putchar('.');
        int col = 1;
        for (i = 0; i < cnt; ++i) {
            putchar(digits[i]);
            ++col;
            if (col == 50 && i != cnt - 1) { putchar('\n'); col = 0; }
        }
        putchar('\n');
        if (terminates) printf("This expansion terminates.\n");
        else printf("The last %d digits repeat forever.\n", period);
    }
    return 0;
}
