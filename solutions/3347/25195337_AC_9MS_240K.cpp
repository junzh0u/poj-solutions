// POJ 3347 - Kadj Squares
// Model: claude-opus-5
//
// Squares tilted 45 degrees with their bottom vertex on y=0.  Scaling x (and y)
// by sqrt(2) turns the square of side s with bottom vertex at x into the lattice
// diamond with vertices (b,0), (b-s,s), (b,2s), (b+s,s), so every coordinate is
// an integer and no floating point is needed.
//
// Placement: two such diamonds with b_j < b_i have disjoint interiors exactly
// when b_i - b_j >= 2*min(s_i,s_j) (the smaller one's near edge comes to lie on
// the larger one's edge).  Verified against a bisection search on real polygon
// overlap area.  Since 2*min > 0, the "b_i > b_{i-1}" rule is implied, and
//     b_1 = s_1 (left vertex on x=0),  b_i = max_{j<i} (b_j + 2*min(s_i,s_j)).
//
// Visibility: at abscissa X the upper profile of diamond i is 2*s_i - |X - b_i|.
// The upward half-line from the highest point of S_i at X is clear iff that
// profile is strictly above every other diamond's profile there -- a tie means
// another (closed) square touches the half-line, and the second sample confirms
// a square whose apex only touches its neighbours counts as hidden.  So S_i is
// visible iff it is somewhere the *unique* maximum profile.  Profiles have slope
// +-1 with integer breakpoints, so all crossings sit on multiples of 1/2 and any
// interval of strict dominance is at least 1/2 wide; sampling a quarter-unit
// grid therefore cannot miss one.
//
// Output: each index is printed followed by a blank, i.e. lines end in a space.
// The statement only says "separated by blank characters", but the discuss board
// reports Presentation Error without the trailing blank, so the judge data has it.

#include <cstdio>
#include <cstdlib>

/* Sized well past the stated bounds (n <= 50, side <= 30) as insurance. */
static const int MAXN = 1005;
static const int GRID = 300000; /* 4 * (1000*60 + 30) + slack */

int s[MAXN], b[MAXN];
int best[GRID], bidx[GRID], bcnt[GRID];
bool vis[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j, x;
        for (i = 0; i < n; i++) scanf("%d", &s[i]);

        for (i = 0; i < n; i++) {
            b[i] = s[i];
            for (j = 0; j < i; j++) {
                int c = b[j] + 2 * (s[i] < s[j] ? s[i] : s[j]);
                if (c > b[i]) b[i] = c;
            }
        }

        int lo = 0, hi = 0;
        for (i = 0; i < n; i++) {
            int r = 4 * (b[i] + s[i]);
            if (r > hi) hi = r;
        }
        for (x = lo; x <= hi; x++) { best[x] = -1; bidx[x] = -1; bcnt[x] = 0; }

        for (i = 0; i < n; i++) {
            int c = 4 * b[i], top = 8 * s[i], w = 4 * s[i];
            for (x = c - w; x <= c + w; x++) {
                int v = top - abs(x - c); /* upper profile of square i at x/4 */
                if (v > best[x]) { best[x] = v; bidx[x] = i; bcnt[x] = 1; }
                else if (v == best[x]) bcnt[x]++;
            }
        }

        for (i = 0; i < n; i++) vis[i] = false;
        for (x = lo; x <= hi; x++)
            if (bcnt[x] == 1) vis[bidx[x]] = true;

        for (i = 0; i < n; i++)
            if (vis[i]) printf("%d ", i + 1);
        printf("\n");
    }
    return 0;
}
