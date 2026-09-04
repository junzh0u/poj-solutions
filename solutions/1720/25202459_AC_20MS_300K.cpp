// POJ 1720 - SQUARES
// Model: claude-sonnet-5
// Approach: For each square, compute the angular interval [lo,hi] it
// subtends at the origin, using the bottom-right corner (min angle) and
// top-left corner (max angle) -- valid because all squares lie strictly in
// the first quadrant (X,Y>=1) and are convex, so each presents a single
// angular silhouette. Since the squares are pairwise disjoint (no touch, no
// overlap), the "which square is nearer along a ray" relation cannot flip
// within the overlap of two squares' angular intervals (a flip would force
// their boundaries to cross, i.e. touch/overlap). Hence between any two
// consecutive breakpoints (the 2N interval endpoints, sorted), the set of
// active squares and their near/far order is constant, so it suffices to
// evaluate a single representative angle (the midpoint) per elementary
// interval, find the nearest active square there (smallest ray-entry
// parameter t = max(X/cos,Y/sin)), and mark it visible. A square is visible
// overall iff it is nearest for at least one elementary interval. This is
// the standard O(N^2) discretized angular sweep for this classic CEOI'98
// problem (no ambiguity in the statement; the board just discusses
// complexity/precision, no framing traps).
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int N;
int X[1005], Y[1005], L[1005];
double lo[1005], hi[1005];
bool vis[1005];

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &L[i]);
        lo[i] = atan2((double)Y[i], (double)(X[i] + L[i]));
        hi[i] = atan2((double)(Y[i] + L[i]), (double)X[i]);
        vis[i] = false;
    }

    double pts[2005];
    int m = 0;
    for (int i = 0; i < N; i++) {
        pts[m++] = lo[i];
        pts[m++] = hi[i];
    }
    sort(pts, pts + m);

    for (int k = 0; k + 1 < m; k++) {
        double a = pts[k], b = pts[k + 1];
        if (b - a < 1e-12) continue;
        double mid = (a + b) / 2.0;
        double cosm = cos(mid), sinm = sin(mid);
        int best = -1;
        double bestT = 1e18;
        for (int i = 0; i < N; i++) {
            if (lo[i] < mid && mid < hi[i]) {
                double t1 = (double)X[i] / cosm;
                double t2 = (double)Y[i] / sinm;
                double t = t1 > t2 ? t1 : t2;
                if (t < bestT) {
                    bestT = t;
                    best = i;
                }
            }
        }
        if (best != -1) vis[best] = true;
    }

    int cnt = 0;
    for (int i = 0; i < N; i++) if (vis[i]) cnt++;
    printf("%d\n", cnt);
    return 0;
}
