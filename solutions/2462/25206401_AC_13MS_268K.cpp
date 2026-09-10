// POJ 2462 - Cutting a polygon
// Model: claude-sonnet-5
//
// Approach: for each cutting line, set up local coordinates with the line
// as axis: t = signed position along the (unit) line direction, h = signed
// perpendicular distance from the line. Snap |h| < 1e-8 to 0, matching the
// statement's own tolerance for "vertex lies on the cutting line".
//
// The set of points of the (infinite) line lying in the closed polygon is
// exactly the union of:
//   (a) sub-segments of edges that lie entirely on the line (both endpoints
//       have h == 0) -- these are boundary and always counted, regardless
//       of interior/exterior parity nearby (a simple polygon can't have
//       another edge crossing through the same range, so no double count).
//   (b) intervals where the line is strictly inside the polygon, found by
//       an even/odd crossing-parity sweep along t.
//
// For (b), an edge whose endpoints have opposite nonzero sides contributes
// one crossing (found by interpolating h to 0). A vertex that merely
// touches the line (h==0) needs special handling: group maximal runs of
// consecutive h==0 vertices (a run of length 1 is a lone touching vertex;
// length > 1 is a chain of collinear edges, already counted in (a)), and
// compare the side just before the run to the side just after. If they
// differ, the run is a crossing; a length-1 crossing is just a point event.
// A length > 1 crossing run needs care: its own [lo,hi] range is already
// fully counted in (a), so it must contribute nothing extra to (b) even
// though a naive "one flip point somewhere in the run" sweep would credit
// part of that range as inside. The fix: place the flip at the run's
// midpoint as usual for parity bookkeeping, but afterwards subtract from
// (b) whatever part of [lo,hi] the naive sweep counted as inside (using
// the parity strictly left of lo, and its flip at the midpoint for the
// crossing case). A length > 1 *touch* run (same side before and after)
// gets no flip event, but can still sit inside an "inside" stretch created
// by unrelated crossings elsewhere -- that same subtraction removes it
// too. No other edge of a simple polygon can pass through a run's own
// [lo,hi] (that range is already occupied by the polygon's own boundary),
// so exactly one such adjustment per run suffices; nothing else overlaps.
//
// All point crossing t-values are sorted; starting outside at t = -inf,
// each crossing toggles inside/outside, and the lengths of the resulting
// "inside" intervals are summed, then corrected by the run subtraction
// above, together with the boundary length from (a).
//
// No statement ambiguity beyond what the text and board already resolve:
// the "cutting line" is an infinite line, not a segment (confirmed on the
// discuss board with a worked unit-square example), and the polygon's own
// perimeter counts as part of the polygon.

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    int n, m;
    while(scanf("%d %d", &n, &m) == 2){
        if(n == 0 && m == 0) break;
        vector<double> px(n), py(n);
        for(int i = 0; i < n; i++) scanf("%lf %lf", &px[i], &py[i]);
        for(int q = 0; q < m; q++){
            double x1, y1, x2, y2;
            scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
            double dx = x2 - x1, dy = y2 - y1;
            double len = sqrt(dx*dx + dy*dy);
            dx /= len; dy /= len;

            vector<double> t(n), h(n);
            vector<int> side(n);
            for(int i = 0; i < n; i++){
                double rx = px[i] - x1, ry = py[i] - y1;
                t[i] = rx*dx + ry*dy;
                double hh = dx*ry - dy*rx;
                if(fabs(hh) < 1e-8) hh = 0.0;
                h[i] = hh;
                side[i] = (hh == 0.0) ? 0 : (hh > 0 ? 1 : -1);
            }

            double boundaryLen = 0.0;
            vector<double> events;
            for(int i = 0; i < n; i++){
                int j = (i + 1) % n;
                if(side[i] == 0 && side[j] == 0){
                    boundaryLen += fabs(t[j] - t[i]);
                } else if(side[i] != 0 && side[j] != 0 && side[i] != side[j]){
                    double tc = t[i] + (t[j] - t[i]) * ((0.0 - h[i]) / (h[j] - h[i]));
                    events.push_back(tc);
                }
            }

            // multi-vertex runs (lo < hi) that need the post-hoc subtraction
            vector<double> runLo, runHi, runMid;
            vector<int> runCrossing;

            bool anyZero = false;
            for(int i = 0; i < n; i++) if(side[i] == 0) anyZero = true;
            if(anyZero){
                int start = -1;
                for(int i = 0; i < n; i++) if(side[i] != 0){ start = i; break; }
                // start must exist: a valid simple polygon has positive area,
                // so it cannot have all vertices collinear with any line.
                vector<double> t2(n);
                vector<int> side2(n);
                for(int k = 0; k < n; k++){
                    int idx = (start + k) % n;
                    t2[k] = t[idx];
                    side2[k] = side[idx];
                }
                int k = 1;
                while(k < n){
                    if(side2[k] == 0){
                        int runStart = k;
                        while(k < n && side2[k] == 0) k++;
                        int prevIdx = runStart - 1;          // nonzero
                        int nextIdx = (k < n) ? k : 0;        // nonzero
                        double lo = t2[runStart], hi = t2[k - 1];
                        if(lo > hi){ double tmp = lo; lo = hi; hi = tmp; }
                        bool crossing = (side2[prevIdx] != side2[nextIdx]);
                        if(lo == hi){
                            // single touching vertex: zero-width, no range issue
                            if(crossing) events.push_back(lo);
                        } else {
                            double mid = (lo + hi) / 2.0;
                            if(crossing) events.push_back(mid);
                            runLo.push_back(lo);
                            runHi.push_back(hi);
                            runMid.push_back(mid);
                            runCrossing.push_back(crossing ? 1 : 0);
                        }
                    } else {
                        k++;
                    }
                }
            }

            sort(events.begin(), events.end());
            double insideLen = 0.0;
            for(size_t k = 0; k + 1 < events.size(); k += 2){
                insideLen += events[k+1] - events[k];
            }

            // Correct for multi-vertex runs: their own [lo,hi] range is
            // already fully counted via boundaryLen and must never also
            // count toward insideLen.
            double subtract = 0.0;
            for(size_t r = 0; r < runLo.size(); r++){
                long cntLo = (long)(lower_bound(events.begin(), events.end(), runLo[r]) - events.begin());
                int parityLo = (int)(cntLo % 2);
                if(runCrossing[r]){
                    if(parityLo == 1) subtract += runMid[r] - runLo[r];
                    int parityAfter = 1 - parityLo;
                    if(parityAfter == 1) subtract += runHi[r] - runMid[r];
                } else {
                    if(parityLo == 1) subtract += runHi[r] - runLo[r];
                }
            }

            printf("%.3f\n", boundaryLen + insideLen - subtract);
        }
    }
    return 0;
}
