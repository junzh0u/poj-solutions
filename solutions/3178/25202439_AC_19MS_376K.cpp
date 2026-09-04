// POJ 3178 - Roping the Field
// Model: claude-sonnet-5
// Convex polygon of N fence posts; want max set of pairwise non-crossing
// diagonals (ropes) between non-adjacent posts, such that no chosen segment
// touches or crosses any of G forbidden circles (radius R). A vertex may
// have several non-crossing ropes fanning out of it (it's not a matching),
// so this is the "maximum non-crossing set of diagonals from an allowed
// subset", not a simple triangulation-count DP.
//
// g[i][j] = max ropes usable strictly among vertices i..j, NEVER counting
// the (i,j) edge itself (whether or not it happens to be a valid chord --
// that credit belongs to whoever else uses it as one of their own hops).
// The face of the (i,j) sub-polygon that touches the (i,j) edge has a
// boundary walk i=w0 < w1 < ... < wt=j (t>=2 whenever j>i+1) alternating
// plain polygon edges (w_{s+1}=w_s+1) and real diagonals; each edge of that
// walk splits off an independent, strictly smaller sub-problem on its
// other side. Decomposing on the walk's first stop w1 (i<w1<j, strictly
// less than j -- a walk can never single-hop straight from i to j, since
// that would just be reusing the (i,j) edge being excluded) gives:
//   g[i][j] = max_{i<w1<j} ( [0 if w1==i+1, else 1 if chord(i,w1) valid]
//                              + g[i][w1]
//                              + g[w1][j] + [1 if chord(w1,j) valid] )
// The last term lets the walk's remainder end at j via one more real
// diagonal directly (w1 != i here, so crediting chord(w1,j) is not
// self-referential -- it's simply chordValid(w1,j) added on top of the
// independent value g[w1][j]). g[i][i] = g[i][i+1] = 0. Answer = g[0][N-1].
//
// Verified by hand and by a brute-force independent-set-of-non-crossing-
// diagonals search (below) on random small convex polygons. Two earlier,
// simpler recurrences both undercounted: one only ever credited a vertex
// with a single outgoing chord (missed fan triangulations); the other
// allowed a second chord but still couldn't express a triangle of three
// diagonals (0,2),(2,4),(0,4) forced when the "long" diagonals of a hexagon
// are blocked, which needs g[0][4] itself to combine chord (0,2) *and*
// chord (2,4) -- exactly what the g[w1][j] + [chord(w1,j) valid] term
// supplies and the earlier attempts could not.
//
// Validity of chord (a,b): not adjacent in the original polygon
// (min(|a-b|, N-|a-b|) != 1), and for every circle center C, the point-to-
// segment distance from C to (a,b) must be strictly greater than R (touching
// the edge is also forbidden). All quantities are integers (coords and R up
// to 1e6/1e5), so the whole predicate is computed with exact integer /
// __int128 arithmetic -- no floating point at all -- since the naive
// cross^2 vs R^2*|AB|^2 comparison needs numbers up to ~4e24, which the
// board's own posted reference code (message 168919) compares via double
// and which other posters (138680, 353918) report as a precision trap.
// __int128 keeps this exact, so no epsilon tuning is needed. Requires GCC
// (language 0) since old MSVC (language 4) lacks __int128.

#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef __int128 lll;

static int N, G;
static ll R, R2;
static ll X[155], Y[155];
static ll CX[105], CY[105];
static int dp[155][155];
static bool valid[155][155];

// true if the point-to-segment distance from C to segment AB is <= R
// (i.e. the rope would touch or cross this grain circle)
static bool segTouchesCircle(ll ax, ll ay, ll bx, ll by, ll cx, ll cy) {
    ll abx = bx - ax, aby = by - ay;
    ll acx = cx - ax, acy = cy - ay;
    ll dotAcAb = acx * abx + acy * aby;
    ll dotAbAb = abx * abx + aby * aby;
    if (dotAcAb <= 0) {
        ll d2 = acx * acx + acy * acy;
        return d2 <= R2;
    }
    if (dotAcAb >= dotAbAb) {
        ll bcx = cx - bx, bcy = cy - by;
        ll d2 = bcx * bcx + bcy * bcy;
        return d2 <= R2;
    }
    ll cross = abx * acy - aby * acx;
    lll lhs = (lll)cross * cross;
    lll rhs = (lll)R2 * dotAbAb;
    return lhs <= rhs;
}

static bool adjacent(int a, int b) {
    int d = a > b ? a - b : b - a;
    return d == 1 || d == N - 1;
}

static bool chordValid(int a, int b) {
    if (adjacent(a, b)) return false;
    for (int t = 0; t < G; t++) {
        if (segTouchesCircle(X[a], Y[a], X[b], Y[b], CX[t], CY[t])) return false;
    }
    return true;
}

int main() {
    if (scanf("%d %d %lld", &N, &G, &R) != 3) return 0;
    for (int i = 0; i < N; i++) scanf("%lld %lld", &X[i], &Y[i]);
    for (int i = 0; i < G; i++) scanf("%lld %lld", &CX[i], &CY[i]);
    R2 = R * R;

    // Precompute chord validity once (O(N^2 * G)) instead of recomputing
    // it inside the O(N^3) DP loop below.
    for (int a = 0; a < N; a++) {
        for (int b = a + 1; b < N; b++) {
            valid[a][b] = valid[b][a] = chordValid(a, b);
        }
    }

    for (int i = 0; i < N; i++) {
        dp[i][i] = 0;
        if (i + 1 < N) dp[i][i + 1] = 0;
    }
    // Process by increasing range length so dp[i][w1] (w1<j) and
    // dp[w1][j] (w1>i) are always already known (both strictly shorter
    // ranges than [i,j]).
    for (int len = 2; len < N; len++) {
        for (int i = 0; i + len < N; i++) {
            int j = i + len;
            int best = 0;
            for (int w1 = i + 1; w1 < j; w1++) {
                bool firstOk = (w1 == i + 1) || valid[i][w1];
                if (!firstOk) continue;
                int term1 = (w1 == i + 1) ? 0 : 1;
                int term3 = dp[w1][j] + (valid[w1][j] ? 1 : 0);
                int cand = term1 + dp[i][w1] + term3;
                if (cand > best) best = cand;
            }
            dp[i][j] = best;
        }
    }

    printf("%d\n", N >= 2 ? dp[0][N - 1] : 0);
    return 0;
}
