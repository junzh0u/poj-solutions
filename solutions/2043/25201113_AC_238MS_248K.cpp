// POJ 2043 - Area of Polygons
// Model: claude-sonnet-5
//
// Approach: count unit grid squares that have a positive-area intersection
// with the polygon.  All polygon vertices have integer coordinates, so for
// any horizontal strip y in (j, j+1) (j integer) no vertex lies strictly
// inside it: every edge either fully spans the strip (its y-range covers
// [j, j+1]) or does not touch the open strip at all.  Within the strip the
// polygon boundary therefore consists of straight (non-crossing, since the
// polygon is simple) chords that are affine in y.  Sorting the edges that
// span the strip by their x position (breaking ties by the x position at
// the far side of the strip, to resolve local-min/local-max shared
// vertices) and pairing them up left/right (even-odd rule) gives the
// "inside" trapezoids of the strip.  For a trapezoid bounded on the left by
// edge L and on the right by edge R, the set of x values touched with
// positive area by *some* height within the strip is exactly
//   [min(L(j), L(j+1)), max(R(j), R(j+1))]
// because L and R are each linear (monotonic) in y, so the region they
// bound is a straight-sided trapezoid whose x-projection is attained at one
// of its two parallel sides.  A unit column i is touched with positive
// length by this trapezoid iff floor(xlo) <= i <= ceil(xhi) - 1.
//
// Doing this per strip needs only O(m) work to find active edges plus a
// sort of the (typically much smaller) active subset, and never touches an
// individual cell directly -- the overlapping ranges from different
// trapezoids within one row are merged as integer intervals, so a huge but
// simple polygon (e.g. a 4000x4000 square) costs O(rows) not O(rows*cols).
//
// All arithmetic is done with exact fractions (long long numerator over a
// positive denominator equal to an edge's dy) so there is no floating point
// error; POJ's C++ (judge language 4) supports long long/%lld fine (this
// archive has 269 accepted sources using them), so no need to avoid it.
//
// Statement ambiguity: none found beyond the usual "read polygons until a
// single 0" framing, which the sample input/output confirms directly.

#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

static ll floorDiv(ll a, ll b) { // b > 0
    ll q = a / b;
    if (a % b != 0 && a < 0) q--;
    return q;
}
static ll ceilDiv(ll a, ll b) { // b > 0
    return -floorDiv(-a, b);
}

struct Active {
    ll numJ, numJ1, den;
};

bool activeLess(const Active &a, const Active &b) {
    ll lhs = a.numJ * b.den, rhs = b.numJ * a.den;
    if (lhs != rhs) return lhs < rhs;
    ll lhs2 = a.numJ1 * b.den, rhs2 = b.numJ1 * a.den;
    return lhs2 < rhs2;
}

struct Edge {
    ll x1, y1, x2, y2; // y1 < y2 after normalization is NOT required; we keep ymin/ymax separately
    ll ymin, ymax;
    ll dx, dy; // dy != 0, sign of dy can be anything but we normalize on use
};

int main() {
    int m;
    while (scanf("%d", &m) == 1 && m != 0) {
        vector<ll> xs(m), ys(m);
        for (int i = 0; i < m; i++) scanf("%lld %lld", &xs[i], &ys[i]);

        vector<Edge> edges;
        edges.reserve(m);
        ll miny = ys[0], maxy = ys[0];
        for (int i = 0; i < m; i++) {
            int j = (i + 1) % m;
            ll x1 = xs[i], y1 = ys[i], x2 = xs[j], y2 = ys[j];
            if (y1 != y2) {
                Edge e;
                e.x1 = x1; e.y1 = y1; e.x2 = x2; e.y2 = y2;
                e.ymin = min(y1, y2);
                e.ymax = max(y1, y2);
                e.dx = x2 - x1;
                e.dy = y2 - y1;
                edges.push_back(e);
            }
            miny = min(miny, y1);
            maxy = max(maxy, y1);
        }

        ll total = 0;

        // For each row (strip) find active edges and process.
        // numAtJ/den and numAtJ1/den per edge with den > 0 (normalized).
        vector<Active> act;
        act.reserve(edges.size());

        for (ll j = miny; j < maxy; j++) {
            act.clear();
            for (size_t k = 0; k < edges.size(); k++) {
                const Edge &e = edges[k];
                if (e.ymin <= j && e.ymax >= j + 1) {
                    // x(y) = x1 + dx*(y-y1)/dy  => numerator over dy: x1*dy + dx*(y-y1)
                    ll rawJ = e.x1 * e.dy + e.dx * (j - e.y1);
                    ll rawJ1 = e.x1 * e.dy + e.dx * (j + 1 - e.y1);
                    ll den = e.dy;
                    ll numJ = rawJ, numJ1 = rawJ1;
                    if (den < 0) { den = -den; numJ = -numJ; numJ1 = -numJ1; }
                    Active a;
                    a.numJ = numJ; a.numJ1 = numJ1; a.den = den;
                    act.push_back(a);
                }
            }
            if (act.empty()) continue;

            // sort by x at j (numJ/den), tie-break by x at j+1 (numJ1/den)
            sort(act.begin(), act.end(), activeLess);

            // pair them up and compute integer ranges
            vector<pair<ll,ll> > ranges;
            ranges.reserve(act.size() / 2);
            for (size_t k = 0; k + 1 < act.size(); k += 2) {
                const Active &L = act[k];
                const Active &R = act[k + 1];
                ll xloNum = min(L.numJ, L.numJ1);
                ll xloDen = L.den;
                ll xhiNum = max(R.numJ, R.numJ1);
                ll xhiDen = R.den;
                ll imin = floorDiv(xloNum, xloDen);
                ll imax = ceilDiv(xhiNum, xhiDen) - 1;
                if (imin <= imax) ranges.push_back(make_pair(imin, imax));
            }

            if (ranges.empty()) continue;
            sort(ranges.begin(), ranges.end());
            ll curLo = ranges[0].first, curHi = ranges[0].second;
            for (size_t k = 1; k < ranges.size(); k++) {
                if (ranges[k].first > curHi + 1) {
                    total += curHi - curLo + 1;
                    curLo = ranges[k].first;
                    curHi = ranges[k].second;
                } else {
                    curHi = max(curHi, ranges[k].second);
                }
            }
            total += curHi - curLo + 1;
        }

        printf("%lld\n", total);
    }
    return 0;
}
