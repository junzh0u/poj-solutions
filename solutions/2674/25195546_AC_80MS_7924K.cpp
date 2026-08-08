// POJ 2674 - Linear world
// Model: claude-opus-5
//
// Ants on a pole.  Two inhabitants bouncing off each other is, as a multiset of
// trajectories, indistinguishable from them passing through each other, so the
// set of fall times is just { x/V for 'n', (L-x)/V for 'p' } and the answer time
// is the maximum of those.  Identity needs the second half of the trick: bodies
// never cross, so the i-th inhabitant from the left is always the i-th smallest
// position.  Hence the K = #('n') inhabitants that leave over the left edge are
// exactly the K leftmost ones, in index order, and the R = #('p') that leave
// over the right edge are the R rightmost, in reverse index order.  The last
// departure is therefore inhabitant K (1-based) when the maximal trajectory
// exits left, and K+1 when it exits right -- no sorting and no simulation, one
// O(N) pass.  Input already comes in increasing POS, and the discuss board
// reports sorting it again is what makes slow solutions time out.
//
// Output is truncated to two decimals, not rounded (discuss board; both samples
// have integral answers and cannot tell the two apart -- a rounding variant
// passes them and every randomized trial too).  Truncation is done in whole
// hundredths with a small epsilon, so a quotient landing one ulp below an exact
// hundredth -- 0.3/0.1 == 2.9999999999999996 -- still prints 3.00, not 2.99.
// The value is formatted from that integer rather than with %.2f, and without
// long long, which POJ's non-GCC C++ compiler need not support.
//
// Direction letters are accepted in either case ('p'/'P', 'n'/'N').  Input is
// streamed through a fixed buffer and names into a fixed table, so memory does
// not scale with the size of the input file.

#include <cstdio>
#include <cstdlib>
#include <cmath>

#define MAXN   32005
#define NAMELEN 256

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* next whitespace-delimited token into dst; 0 at end of input */
static int readToken(char *dst, int cap) {
    int c, k = 0;
    do {
        c = gc();
        if (c < 0) { dst[0] = '\0'; return 0; }
    } while (c <= ' ');
    while (c > ' ') {
        if (k < cap - 1) dst[k++] = (char)c;
        c = gc();
        if (c < 0) break;
    }
    dst[k] = '\0';
    return 1;
}

static char names[MAXN][NAMELEN];
static char tok[NAMELEN + 64];

int main() {
    for (;;) {
        if (!readToken(tok, sizeof(tok))) break;
        int n = atoi(tok);
        if (n <= 0) break;

        readToken(tok, sizeof(tok));
        double L = atof(tok);
        readToken(tok, sizeof(tok));
        double V = atof(tok);

        double best = -1.0;
        int bestRight = 0;
        int cntN = 0;
        for (int i = 0; i < n; i++) {
            readToken(tok, sizeof(tok));
            int right = (tok[0] == 'p' || tok[0] == 'P');
            readToken(tok, sizeof(tok));
            double x = atof(tok);
            readToken(i < MAXN ? names[i] : tok, i < MAXN ? NAMELEN : (int)sizeof(tok));
            if (!right) cntN++;
            double t = right ? (L - x) / V : x / V;
            if (t > best) {
                best = t;
                bestRight = right;
            }
        }

        int idx = bestRight ? cntN : cntN - 1;
        if (idx < 0) idx = 0;
        if (idx >= n) idx = n - 1;
        if (idx >= MAXN) idx = MAXN - 1;

        double s = best * 100.0;
        double f = floor(s + 1e-9 + s * 1e-11);   /* answer in whole hundredths */
        if (f < 0.0) f = 0.0;
        double whole = floor(f / 100.0);
        int frac = (int)(f - whole * 100.0);
        char num[64];
        sprintf(num, "%.0f.%02d", whole, frac);
        printf("%13s %s\n", num, names[idx]);
    }
    return 0;
}
