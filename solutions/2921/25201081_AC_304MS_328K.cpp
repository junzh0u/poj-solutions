// POJ 2921 - Knights of the Round Table
// Model: claude-sonnet-5
//
// n chairs are placed on a circle of radius n (the radius equals the
// number of knights), at angles 2*pi*p/q given already sorted counter-
// clockwise. We need the maximum Euclidean distance between any two
// chairs, i.e. the diameter of this set of co-circular points.
//
// For two points on a circle of radius r whose angular separation
// (taken as the value in (0, 2*pi) obtained by subtracting the smaller
// angle from the larger one, since all angles lie in [0, 2*pi)) is d,
// the chord length is 2*r*sin(d/2). Because d/2 lies in (0, pi), sin
// is non-negative there, and sin(d/2) = sin(pi - d/2) = sin((2pi-d)/2),
// so it makes no difference which of the two arcs between the points
// we call "the" separation - the formula already picks the correct
// chord. The discuss board (message 358842) confirms this: the maximum
// chord equals the maximum arc-separation-mapped distance, they are the
// same target.
//
// sin(d/2) for d in (0, 2*pi) is unimodal, peaking at d = pi. Since the
// points are sorted by increasing angle, for a fixed left index i the
// best right index j (giving d = a[j]-a[i] closest to pi) is
// non-decreasing as i increases - the classic two-pointer / rotating
// calipers argument for points in angular order. So a single forward
// sweep with a pointer that never resets backward finds the global
// maximum in O(n).
//
// Answer magnitude is bounded by 2*n <= 20000, no long long needed;
// only doubles for angles and distances. n <= 10000 per scenario with a
// generous 5000ms limit, so O(n) per scenario is comfortable.
#include <cstdio>
#include <cmath>
using namespace std;

int main(){
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        int n;
        scanf("%d", &n);
        static double ang[10005];
        for (int i = 0; i < n; ++i) {
            int p, q;
            scanf("%d %d", &p, &q);
            ang[i] = 2.0 * M_PI * (double)p / (double)q;
        }
        double r = (double)n;
        double best = 0.0;
        int j = 1;
        for (int i = 0; i < n; ++i) {
            if (j < i + 1) j = i + 1;
            while (j + 1 < n) {
                double cur = ang[j] - ang[i];
                double nxt = ang[j + 1] - ang[i];
                if (sin(nxt / 2.0) > sin(cur / 2.0)) {
                    j = j + 1;
                } else {
                    break;
                }
            }
            if (j < n) {
                double diff = ang[j] - ang[i];
                double dist = 2.0 * r * sin(diff / 2.0);
                if (dist > best) best = dist;
            }
        }
        printf("Scenario #%d:\n%.2f\n\n", tc, best);
    }
    return 0;
}
