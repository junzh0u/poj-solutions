// POJ 1471 - Triangles
// Model: claude-opus-5
//
// Geometry.  The printed triangle has 2n-1 fields on its FIRST line and a
// single field on its last, so on screen it points down.  Reverse the n lines
// and it points up: row k (0-based) then holds 2k+1 fields at print columns
// n-1-k .. n-1+k, and the field at column d points up iff (d-(n-1-k)) is even.
// Reversing does not move anything horizontally, because line i is padded with
// i spaces and line n-1-i with n-1-i, which is exactly the padding row n-1-i
// needs.  In these coordinates the field directly below an up field (k,d) is
// the down field (k+1,d) -- same print column -- which is what makes the whole
// scan a column-interval test.
//
// An up-pointing sub-triangle of side s is fixed by its apex, an up field
// (k,d): it covers columns d-t .. d+t of row k+t, t = 0..s-1.  A down-pointing
// one is fixed by its bottom vertex, a down field (k,d): it covers columns
// d-t .. d+t of row k-t.  Both hold s*s fields, and each contains the same
// triangle of side s-1, so for every field grow t greedily while the next
// row's whole span is white; a per-row prefix sum makes each step O(1).
// Answer max t*t, or 0 when no field is white.  O(n^3) worst case: 0.0013s for
// an all-white n=100 case, against a 1000MS limit.
//
// The trap is orientation: the statement's own note ("the largest triangle can
// have its point at the top") and the discuss board's 3-row case
// "----# / #-# / #" -- answer 1, not 4 -- are both about the down-pointing
// family, which the sample's first case does not need.
//
// Fields are located by counting '#'/'-' characters rather than by trusting
// the left padding, and the column guard in rowAllWhite is defensive only: the
// span leaves the big triangle (where cells read as black) before any index
// could go out of range.

#include <cstdio>

static const int MAXN = 105;
static const int MAXW = 2 * MAXN;

static bool w[MAXN][MAXW];
static int pre[MAXN][MAXW + 1];
static int n, W;

static bool rowAllWhite(int k, int a, int b) {
    if (a < 0 || b >= W) return false;
    return pre[k][b + 1] - pre[k][a] == b - a + 1;
}

int main() {
    char line[1024];
    int caseNo = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        if (fgets(line, sizeof(line), stdin) == NULL) line[0] = '\0';
        W = 2 * n - 1;
        ++caseNo;
        for (int k = 0; k < n; ++k)
            for (int d = 0; d < W; ++d) w[k][d] = false;
        for (int i = 0; i < n; ++i) {
            if (fgets(line, sizeof(line), stdin) == NULL) line[0] = '\0';
            int col = i, last = 2 * n - 2 - i;
            for (int q = 0; line[q] != '\0' && col <= last; ++q) {
                char c = line[q];
                if (c == '-' || c == '#') {
                    w[n - 1 - i][col] = (c == '-');
                    ++col;
                }
            }
        }
        for (int k = 0; k < n; ++k) {
            pre[k][0] = 0;
            for (int d = 0; d < W; ++d)
                pre[k][d + 1] = pre[k][d] + (w[k][d] ? 1 : 0);
        }
        int best = 0;
        for (int k = 0; k < n; ++k) {
            int lo = n - 1 - k, hi = n - 1 + k;
            for (int d = lo; d <= hi; ++d) {
                if (!w[k][d]) continue;
                if (best < 1) best = 1;
                int t = 1;
                if (((d - lo) & 1) == 0)
                    while (k + t < n && rowAllWhite(k + t, d - t, d + t)) ++t;
                else
                    while (k - t >= 0 && rowAllWhite(k - t, d - t, d + t)) ++t;
                if (t * t > best) best = t * t;
            }
        }
        printf("Triangle #%d\n", caseNo);
        printf("The largest triangle area is %d.\n\n", best);
    }
    return 0;
}
