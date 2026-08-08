// POJ 1715 - Hexadecimal Numbers
// Model: claude-opus-5
//
// X = positive integers whose hex representation has at most 8 digits, all
// distinct, leading digit nonzero.  Print the n-th largest element.
//
// Elements with more digits are always larger (a leading digit is nonzero), and
// within one length the numeric order is the lexicographic order of the digit
// string.  So the ranking is: unrank the length, then unrank the digits left to
// right, at each step skipping whole blocks of completions.
//
//   cnt[L] = 15 * P(15, L-1)   elements of length L
//   after i digits are fixed, each remaining choice at position i covers
//   P(15-i, L-1-i) completions
//
// |X| = 546481140 < 2^31, and the largest block is P(15,7) = 32432400, so every
// intermediate value fits in a 32-bit int; no 64-bit arithmetic is involved.
//
// The statement describes one n, but the input is read to EOF so a file holding
// several is handled too.
//
// The `i == 0 && d == 0` guard is provably redundant (rem <= cnt[L]-1 is less
// than the 15 nonzero blocks, so the descending scan can never reach 0 at the
// leading position) and is kept only to state the constraint.

#include <cstdio>

static const char *DIG = "0123456789ABCDEF";

/* P[a][b] = number of ordered selections of b items out of a. */
static int P[17][17];

int main() {
    int a, b, i, d, L, n;

    for (a = 0; a <= 16; ++a) {
        P[a][0] = 1;
        for (b = 1; b <= a && b <= 8; ++b) P[a][b] = P[a - 1][b - 1] * a;
    }

    /* cnt[L] = number of elements of X with exactly L hex digits:
       15 choices for the leading digit, then P(15, L-1) for the rest. */
    int cnt[9];
    for (L = 1; L <= 8; ++L) cnt[L] = 15 * P[15][L - 1];

    while (scanf("%d", &n) == 1) {
        for (L = 8; L >= 1; --L) {
            if (n <= cnt[L]) break;
            n -= cnt[L];
        }

        int rem = n - 1;
        bool used[16];
        for (d = 0; d < 16; ++d) used[d] = false;

        char out[9];
        for (i = 0; i < L; ++i) {
            int block = P[15 - i][L - 1 - i];
            for (d = 15; d >= 0; --d) {
                if (used[d]) continue;
                if (i == 0 && d == 0) continue;
                if (rem >= block) { rem -= block; continue; }
                break;
            }
            used[d] = true;
            out[i] = DIG[d];
        }
        out[L] = '\0';
        printf("%s\n", out);
    }
    return 0;
}
