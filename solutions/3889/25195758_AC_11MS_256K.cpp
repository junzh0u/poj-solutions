// POJ 3889 - Fractal Streets
// Model: claude-opus-5
//
// House m (1-indexed) of an order-n Hilbert curve sits at a grid cell; the answer
// is 10 * |pos(h) - pos(o)| rounded to the nearest integer.
//
// pos() is the standard order-n -> order-(n-1) recursion, with the four quadrant
// transforms read off figure 2: the order-1 cup is 1=(0,0) 2=(0,1) 3=(1,1) 4=(1,0),
// and the order-2 picture shows the top-left sub-cup transposed ((r,c)->(c,r)), the
// top-right and bottom-right ones translated unchanged, and the bottom-left one
// anti-transposed ((r,c)->(len-1-c, len-1-r)) before translation. Depth is n<=15,
// so each query is O(n).
//
// Ambiguity: figure 2(b) labels houses 7 and 8 on the wrong driveways -- the road
// drawn in that same figure runs 6 -> (1,3) -> (1,2) -> 9, and the printed labels
// put 7 at (1,2) and 8 at (1,3), which would make consecutive houses non-adjacent
// and contradict "the distance between two adjacent houses is 10m". The road wins;
// the discuss board reports the same swap. Every other label in 2(b) agrees with
// the recursion above.
//
// Rounding is done in exact integers: with S = 100*(dr^2 + dc^2) the answer is the
// k with (2k-1)^2 <= 4S < (2k+1)^2. An exact .5 tie cannot occur, since 400*S is
// even and (2k+1)^2 is odd. Only intermediates need 64 bits (S < 2.2e11), so this
// goes out under G++; h, o < 2^31 and the answer < 2^19 both fit in int.

#include <cstdio>
#include <cmath>

typedef long long ll;

/* (row, col) of the m-th house, 0-indexed, on a Hilbert curve of order n */
static void calc(int n, int m, int &r, int &c) {
    if (n == 0) { r = 0; c = 0; return; }
    int len = 1 << (n - 1);
    int cnt = 1 << (2 * n - 2);
    int pr, pc;
    calc(n - 1, m % cnt, pr, pc);
    switch (m / cnt) {
        case 0: r = pc;                      c = pr;             break;
        case 1: r = pr;                      c = pc + len;       break;
        case 2: r = pr + len;                c = pc + len;       break;
        default: r = len + (len - 1 - pc);   c = len - 1 - pr;   break;
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n, h, o;
        if (scanf("%d %d %d", &n, &h, &o) != 3) return 0;
        int r1, c1, r2, c2;
        calc(n, h - 1, r1, c1);
        calc(n, o - 1, r2, c2);
        ll dr = r1 - r2, dc = c1 - c2;
        ll s = 100LL * (dr * dr + dc * dc);      /* squared distance in metres */
        ll k = (ll)sqrt((double)s);
        while (k > 0 && k * k > s) --k;
        while ((k + 1) * (k + 1) <= s) ++k;
        if (4 * s >= (2 * k + 1) * (2 * k + 1)) ++k;   /* round to nearest */
        printf("%d\n", (int)k);
    }
    return 0;
}
