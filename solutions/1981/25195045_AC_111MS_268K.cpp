// POJ 1981 - Circle and Points
// Model: claude-sonnet-5
// Approach: For each point p, treat it as lying exactly on the boundary of the
// enclosing unit-radius circle; the circle's center then lies on the unit
// circle around p at some angle theta. For every other point q within
// distance 2 of p, the circle also encloses q for theta in a contiguous arc
// [ang-da, ang+da], where ang is the direction from p to q and
// da = acos(d/2). Sweeping theta around the full circle (as +1/-1 events,
// splitting arcs that cross the -pi/pi cut) finds, for this p, the maximum
// number of points simultaneously enclosed by some unit circle through p.
// Taking the max over all p (and starting each sweep's count at 1 for p
// itself) gives the global answer; it is never less than 1 even for N=1,
// which is why the running maximum must be seeded at 1, not 0.
// No statement ambiguity found; the input's genericity guarantees (no two
// points at distance ~2, no three points ~concyclic on a unit circle) keep
// the sweep's endpoint handling well defined.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

static const double PI = acos(-1.0);
static const int MAXN = 305;

int main() {
    int n;
    static double x[MAXN], y[MAXN];
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; ++i) scanf("%lf %lf", &x[i], &y[i]);
        int ans = 1;
        for (int i = 0; i < n; ++i) {
            vector<pair<double, int> > ev;
            ev.reserve(4 * n);
            for (int j = 0; j < n; ++j) {
                if (j == i) continue;
                double dx = x[j] - x[i], dy = y[j] - y[i];
                double d = sqrt(dx * dx + dy * dy);
                if (d >= 2.0) continue;
                double ang = atan2(dy, dx);
                double c = d / 2.0;
                if (c > 1.0) c = 1.0;
                if (c < -1.0) c = -1.0;
                double da = acos(c);
                double lo = ang - da, hi = ang + da;
                if (lo < -PI) {
                    ev.push_back(make_pair(lo + 2 * PI, 1));
                    ev.push_back(make_pair(PI, -1));
                    ev.push_back(make_pair(-PI, 1));
                    ev.push_back(make_pair(hi, -1));
                } else if (hi > PI) {
                    ev.push_back(make_pair(lo, 1));
                    ev.push_back(make_pair(PI, -1));
                    ev.push_back(make_pair(-PI, 1));
                    ev.push_back(make_pair(hi - 2 * PI, -1));
                } else {
                    ev.push_back(make_pair(lo, 1));
                    ev.push_back(make_pair(hi, -1));
                }
            }
            sort(ev.begin(), ev.end());
            int cur = 1;
            if (cur > ans) ans = cur;
            for (size_t k = 0; k < ev.size(); ++k) {
                cur += ev[k].second;
                if (cur > ans) ans = cur;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
