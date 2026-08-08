// POJ 1894 - Alternative Scale of Notation
// Model: claude-opus-5
//
// Invert the bijective base-B numeration V_B, whose digit set is {1,...,B}
// (no zero digit, a "B" digit instead).  V_B(a[1..n]) = sum a[n-i]*B^i.
//
// The inverse is the ordinary radix expansion shifted by one: writing
// x = q*B + d with d in [1,B] forces q = (x-1)/B and d = (x-1)%B + 1, so
// repeatedly subtracting 1, dividing by B and emitting remainder+1 peels off
// the digits from least significant to most.  x = 0 is the empty string eps,
// since V_B(eps) = 0 and 0 is not a legal digit.
//
// x <= 10^100 needs bignum.  x is held as base-10^4 limbs so that the division
// step r*10000 + limb stays under 100000 and never needs a 64-bit type.
//
// Input framing (not in the statement, taken from the discuss board and made
// safe either way): the file holds several data sets and must be read to EOF,
// and x = 0 prints an empty line rather than "0".  The official sample has one
// data set and no zero, so it cannot exercise either.

#include <cstdio>
#include <cstring>

const int MAXD = 1200;   /* decimal digits of x (statement: <= 101) */
const int MAXL = 320;    /* base-10^4 limbs */
const int MAXOUT = 4200; /* output digits: log2(10^100) ~ 333 */

static char buf[MAXD + 16];
static int limb[MAXL + 4];
static char out[MAXOUT + 16];

int main() {
    int B;
    while (scanf("%d", &B) == 1) {
        if (scanf("%s", buf) != 1) break;
        int len = (int)strlen(buf);
        int st = 0;
        while (st + 1 < len && buf[st] == '0') st++;

        int n = 0;
        int i = len;
        while (i > st) {
            int lo = i - 4;
            if (lo < st) lo = st;
            int v = 0;
            for (int j = lo; j < i; j++) v = v * 10 + (buf[j] - '0');
            limb[n++] = v;
            i = lo;
        }
        while (n > 0 && limb[n - 1] == 0) n--;

        int m = 0;
        while (n > 0) {
            /* x >= 1 here: subtract 1 */
            int k = 0;
            while (limb[k] == 0) { limb[k] = 9999; k++; }
            limb[k]--;
            while (n > 0 && limb[n - 1] == 0) n--;
            /* divide by B, keeping the remainder */
            int r = 0;
            for (int j = n - 1; j >= 0; j--) {
                int cur = r * 10000 + limb[j];
                limb[j] = cur / B;
                r = cur % B;
            }
            while (n > 0 && limb[n - 1] == 0) n--;
            out[m++] = (char)('0' + r + 1);
        }
        while (m > 0) putchar(out[--m]);
        putchar('\n');
    }
    return 0;
}
