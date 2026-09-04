// POJ 1387 - The Constraint Densest Submatrix
// Model: claude-sonnet-5
// Approach: find the densest (consecutive) submatrix with at least R rows and
// at least C columns. Key theorem (classic "maximum average subarray with
// minimum length" result, extended to 2D independently on rows/columns):
// there always exists an optimal submatrix whose row count p satisfies
// R <= p <= 2R-1 and whose column count q satisfies C <= q <= 2C-1. Proof
// idea: if p >= 2R, split the p rows into a top block and a bottom block,
// each with >= R rows (possible since p >= 2R); the whole submatrix's
// average is a weighted average of the two blocks' averages (same columns),
// so at least one block is at least as dense and still satisfies the column
// constraint, and it has fewer rows. Repeat until p < 2R. Symmetric argument
// bounds q < 2C. So it suffices to try every (p,q) pair in that bounded
// range and, for each, every placement, using a 2D prefix-sum table for
// O(1) sum queries. The total work is bounded by
// (sum_{p=R}^{min(m,2R-1)} (m-p+1)) * (sum_{q=C}^{min(n,2C-1)} (n-q+1)),
// which is maximized around R,C ~ m/3 and stays well under 10^8 for
// m,n,R,C <= 200.
// Densities are compared exactly via cross-multiplication of (sum, area)
// pairs using 64-bit integers (max sum ~200*200*800 = 3.2e7, max area
// 200*200 = 4e4, product ~1.28e12, needs 64 bits) to avoid any floating
// point tie-break ambiguity; ties are broken by the lexicographically
// smallest (r1,c1,r2,c2) as required by the statement. The tie-break chain
// is written as flat single-statement branches (no nested braces) rather
// than nested "else if (...) { ... }" blocks, after a first submission was
// corrupted client-side: the submit page's own code editor silently ate one
// closing brace out of a run of stacked "else if" blocks before the POST,
// producing a Compile Error on code that compiled and ran correctly locally
// (confirmed byte-for-byte via the judge's own showsource page).
// Ambiguity: none found in the statement itself; the discuss board's
// "1 4 1 2 / 97 100 100 90" counterexample was posted against a different
// (incorrect) O(n) sliding-window heuristic, not against this theorem-based
// enumeration, and was checked by hand to be handled correctly here.
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

static const int MAXN = 205;
static int a[MAXN][MAXN];
static ll P[MAXN][MAXN];
static ll band[MAXN];

static int m, n, R, C;
static bool found;
static ll bestSum;
static int bestArea, br1, bc1, br2, bc2;

static void consider(int r1, int c1, int r2, int c2, ll sum, int area) {
    if (!found) {
        found = true;
        bestSum = sum; bestArea = area;
        br1 = r1; bc1 = c1; br2 = r2; bc2 = c2;
        return;
    }
    ll lhs = sum * (ll)bestArea;
    ll rhs = bestSum * (ll)area;
    bool better;
    if (lhs != rhs) better = (lhs > rhs);
    else if (r1 != br1) better = (r1 < br1);
    else if (c1 != bc1) better = (c1 < bc1);
    else if (r2 != br2) better = (r2 < br2);
    else better = (c2 < bc2);
    if (better) {
        bestSum = sum; bestArea = area;
        br1 = r1; bc1 = c1; br2 = r2; bc2 = c2;
    }
}

static void scanBand(int r1, int r2, int p, int Q2) {
    for (int q = C; q <= Q2; ++q) {
        int area = p * q;
        for (int c1 = 1; c1 + q - 1 <= n; ++c1) {
            int c2 = c1 + q - 1;
            ll sum = band[c2] - band[c1 - 1];
            consider(r1, c1, r2, c2, sum, area);
        }
    } }

int main() {
    while (scanf("%d", &m) == 1 && m != 0) {
        scanf("%d %d %d", &n, &R, &C);
        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                scanf("%d", &a[i][j]);

        for (int i = 0; i <= m; ++i) P[i][0] = 0;
        for (int j = 0; j <= n; ++j) P[0][j] = 0;
        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                P[i][j] = P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1] + a[i][j];

        int P2 = min(m, 2 * R - 1);
        int Q2 = min(n, 2 * C - 1);

        found = false;
        bestSum = 0;
        bestArea = 0; br1 = 0; bc1 = 0; br2 = 0; bc2 = 0;

        for (int p = R; p <= P2; ++p) {
            for (int r1 = 1; r1 + p - 1 <= m; ++r1) {
                int r2 = r1 + p - 1;
                const ll *top = P[r2];
                const ll *bot = P[r1 - 1];
                for (int j = 0; j <= n; ++j) band[j] = top[j] - bot[j];
                scanBand(r1, r2, p, Q2);
            }
        }

        printf("%d %d %d %d\n", br1, bc1, br2, bc2);
    }
    printf("*\n");
    return 0;
}
