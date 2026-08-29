// POJ 3004 - Subway planning
// Model: claude-sonnet-5
//
// Each subway line is a RAY from the central station (0,0) at a chosen
// angle, extending as far as needed, with stations allowed anywhere along
// it. A place p is already covered (needs no line) iff |p| <= d (the
// board's message 110804 confirms exact equality needs no line either;
// since all inputs are integers this is checked exactly via
// x^2+y^2 <= d^2, no floating-point epsilon involved). Otherwise the set
// of ray-directions theta that bring some point of the ray within
// distance d of p is exactly the angular interval
// [phi - beta, phi + beta] centered at p's own angle phi = atan2(y,x),
// where beta = asin(d/r), r = |p|. (If the ray direction departs from phi
// by more than beta, the foot of the perpendicular either lies past d or
// lies behind the origin, in which case the closest ray point is the
// origin itself, at distance r > d, so it doesn't help; this is why beta
// is always < 90 degrees and every arc has width < 180 degrees.)
//
// So the task reduces to: given m arcs on a circle, each of angular width
// < 180 degrees, find the minimum number of points that stab every arc.
//
// If the arcs do NOT cover the whole circle, there is an angle theta0
// touched by no arc at all. Cutting the circle open there turns every arc
// into an honest, non-wrapping linear interval (none can wrap around the
// cut, since none reaches it), and the problem becomes the textbook
// linear "minimum points to stab intervals" greedy: sort by right
// endpoint, repeatedly take the next uncovered interval's right endpoint.
// That free angle is found by sweeping/merging the arcs' coverage of the
// circle (an interval-union computation) in O(m log m).
//
// If the arcs DO cover the whole circle, no such free cut exists, and a
// point of some optimal solution cannot be identified without knowing
// which arc "goes first" -- there is no canonical starting point on a
// full circle. (An earlier attempt tried picking a single global anchor,
// e.g. the arc with numerically smallest raw end value or one rebased
// relative to an arbitrary arc's own start; both are unsound, because any
// single fixed rebasing point can have some other arc sitting just before
// it, which then gets pushed all the way around to look like it "ends
// very late" even though its true position is early -- caught by a
// stress-test mismatch, e.g. n=4 d=4 points (7,-8) (4,1) (-3,1) (4,-8),
// kept as test data.) So in this case we fall back to brute-force trying
// every arc's own right endpoint as the anchor (there are only m of them,
// and WLOG some optimal solution touches one of them, by the standard
// exchange argument: slide any point stabbing that arc clockwise until it
// is about to leave it), taking the best result -- O(m^2 log m), which
// for m <= 500 is a couple million operations, comfortably fast.
//
// Verified against an exact set-cover DP (over arc endpoints) on
// thousands of small random cases, including cases engineered to hit
// each branch (free-gap and full-coverage).

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

static const double TWO_PI = 2.0 * M_PI;
static const double EPS = 1e-9;

bool cmpByEnd(const pair<double, double>& a, const pair<double, double>& b) {
    return a.second < b.second;
}

// Fallback: try every arc's right endpoint as the stabbing anchor.
// s[], e[] are raw (atan2-derived) angles, m arcs. O(m^2 log m).
int solveTryAllAnchors(const vector<double>& s, const vector<double>& e, int m) {
    vector<double> width(m);
    for (int j = 0; j < m; j++) width[j] = e[j] - s[j];

    int best = m + 1;
    vector<pair<double, double> > remaining;
    remaining.reserve(m);
    for (int i = 0; i < m; i++) {
        double anchor = e[i];
        int count = 1;
        remaining.clear();
        for (int j = 0; j < m; j++) {
            double sp = fmod(s[j] - anchor, TWO_PI);
            if (sp < 0) sp += TWO_PI;
            double ep = sp + width[j];
            if (sp <= EPS || ep >= TWO_PI - EPS) continue; // contains anchor
            remaining.push_back(make_pair(sp, ep));
        }
        sort(remaining.begin(), remaining.end(), cmpByEnd);
        double last = -1e18;
        for (size_t k = 0; k < remaining.size(); k++) {
            if (remaining[k].first > last + EPS) {
                count++;
                last = remaining[k].second;
            }
        }
        if (count < best) best = count;
    }
    return best;
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;
    while (N--) {
        int n, dInt;
        scanf("%d %d", &n, &dInt);
        double d = dInt;
        vector<double> s(n), e(n);
        int m = 0;
        for (int i = 0; i < n; i++) {
            int xi, yi;
            scanf("%d %d", &xi, &yi);
            // All inputs are integers, so compare r <= d exactly via
            // integer arithmetic (r*r <= d*d) rather than risking a
            // floating-point epsilon on this make-or-break comparison --
            // the board's message 110804 specifically tests points sitting
            // exactly at r == d.
            long rr2 = (long)xi * xi + (long)yi * yi;
            long dd2 = (long)dInt * dInt;
            if (rr2 <= dd2) continue; // already covered by central station
            double x = xi, y = yi;
            double r = sqrt(x * x + y * y);
            double phi = atan2(y, x);
            double ratio = d / r;
            if (ratio > 1.0) ratio = 1.0;
            double beta = asin(ratio);
            s[m] = phi - beta;
            e[m] = phi + beta;
            m++;
        }
        if (m == 0) {
            printf("0\n");
            continue;
        }
        s.resize(m);
        e.resize(m);

        // Normalize each arc's start into [0, 2*pi); keep width < pi.
        vector<double> ns(m), width(m);
        for (int i = 0; i < m; i++) {
            double v = fmod(s[i], TWO_PI);
            if (v < 0) v += TWO_PI;
            ns[i] = v;
            width[i] = e[i] - s[i];
        }

        // Compute the union of arc coverage on the circle to look for a
        // free angle. Split any arc that wraps past 2*pi into two pieces.
        vector<pair<double, double> > pieces;
        pieces.reserve(m + 4);
        for (int i = 0; i < m; i++) {
            double a = ns[i], b = ns[i] + width[i];
            if (b <= TWO_PI + 1e-12) {
                pieces.push_back(make_pair(a, min(b, TWO_PI)));
            } else {
                pieces.push_back(make_pair(a, TWO_PI));
                pieces.push_back(make_pair(0.0, b - TWO_PI));
            }
        }
        sort(pieces.begin(), pieces.end());
        vector<pair<double, double> > merged;
        merged.reserve(pieces.size());
        for (size_t i = 0; i < pieces.size(); i++) {
            double a = pieces[i].first, b = pieces[i].second;
            if (!merged.empty() && a <= merged.back().second + EPS) {
                if (b > merged.back().second) merged.back().second = b;
            } else {
                merged.push_back(make_pair(a, b));
            }
        }

        bool hasFreeGap = false;
        double cut = 0.0;
        if (merged.empty()) {
            hasFreeGap = true;
            cut = 0.0;
        } else if (merged[0].first > EPS) {
            hasFreeGap = true;
            cut = 0.0;
        } else if (merged.size() > 1) {
            hasFreeGap = true;
            cut = merged[0].second;
        } else if (merged[0].second < TWO_PI - EPS) {
            hasFreeGap = true;
            cut = merged[0].second;
        }

        int answer;
        if (hasFreeGap) {
            // Rebase every arc relative to the free cut; none can wrap.
            vector<pair<double, double> > intervals;
            intervals.reserve(m);
            for (int i = 0; i < m; i++) {
                double sp = fmod(ns[i] - cut, TWO_PI);
                if (sp < 0) sp += TWO_PI;
                double ep = sp + width[i];
                intervals.push_back(make_pair(sp, ep));
            }
            sort(intervals.begin(), intervals.end(), cmpByEnd);
            int count = 0;
            double last = -1e18;
            for (size_t k = 0; k < intervals.size(); k++) {
                if (intervals[k].first > last + EPS) {
                    count++;
                    last = intervals[k].second;
                }
            }
            answer = count;
        } else {
            answer = solveTryAllAnchors(s, e, m);
        }
        printf("%d\n", answer);
    }
    return 0;
}
