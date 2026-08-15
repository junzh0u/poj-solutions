// POJ 1292 - Will Indiana Jones Get There?
// Model: claude-sonnet-5
// Approach: N axis-aligned wall segments. Two segments are free to cross if
// they touch/intersect (distance 0); otherwise a wooden board of length equal
// to the minimum distance between the two segments is needed to bridge them,
// and boards are reusable. The answer is the minimax path value from segment
// 0 (Indy) to segment 1 (trapped person): minimize, over all paths, the
// maximum edge (segment-to-segment distance) used along the path. This is
// computed with a Prim's-MST-style relaxation (O(N^2) time, O(N) memory,
// distances recomputed on the fly rather than stored in an N x N matrix,
// which matters under the 10000K memory limit). Segment-to-segment distance
// uses the standard exact-integer segment-intersection test (cross products,
// since all coordinates are integers) with a fallback to the minimum of the
// four endpoint-to-segment distances when the segments don't intersect.
//
// Ambiguity: statement doesn't explicitly say "minimize the maximum board
// length used on the path" -- the discuss board (message 343995) confirms
// this reading explicitly: boards are reusable, and it is a minimize-the-
// maximum-gap problem, not a shortest-path problem.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

static const int MAXN = 1005;
static int ax[MAXN], ay[MAXN], bx[MAXN], by[MAXN];

static int cross(int ox, int oy, int ax_, int ay_,
                        int bx_, int by_) {
    return (ax_ - ox) * (by_ - oy) - (ay_ - oy) * (bx_ - ox);
}

static int sgn(int v) { return (v > 0) - (v < 0); }

static bool onSeg(int px, int py, int ax_, int ay_,
                   int bx_, int by_) {
    return min(ax_, bx_) <= px && px <= max(ax_, bx_) &&
           min(ay_, by_) <= py && py <= max(ay_, by_);
}

static bool segIntersect(int p1x, int p1y, int p2x, int p2y,
                          int p3x, int p3y, int p4x, int p4y) {
    int d1 = cross(p3x, p3y, p4x, p4y, p1x, p1y);
    int d2 = cross(p3x, p3y, p4x, p4y, p2x, p2y);
    int d3 = cross(p1x, p1y, p2x, p2y, p3x, p3y);
    int d4 = cross(p1x, p1y, p2x, p2y, p4x, p4y);
    int s1 = sgn(d1), s2 = sgn(d2), s3 = sgn(d3), s4 = sgn(d4);
    if (((s1 > 0 && s2 < 0) || (s1 < 0 && s2 > 0)) &&
        ((s3 > 0 && s4 < 0) || (s3 < 0 && s4 > 0)))
        return true;
    if (s1 == 0 && onSeg(p1x, p1y, p3x, p3y, p4x, p4y)) return true;
    if (s2 == 0 && onSeg(p2x, p2y, p3x, p3y, p4x, p4y)) return true;
    if (s3 == 0 && onSeg(p3x, p3y, p1x, p1y, p2x, p2y)) return true;
    if (s4 == 0 && onSeg(p4x, p4y, p1x, p1y, p2x, p2y)) return true;
    return false;
}

static double ptSegDist(double px, double py, double ax_, double ay_,
                         double bx_, double by_) {
    double dx = bx_ - ax_, dy = by_ - ay_;
    double len2 = dx * dx + dy * dy;
    double t;
    if (len2 < 1e-12) {
        t = 0.0;
    } else {
        t = ((px - ax_) * dx + (py - ay_) * dy) / len2;
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;
    }
    double cx = ax_ + t * dx, cy = ay_ + t * dy;
    double ddx = px - cx, ddy = py - cy;
    return sqrt(ddx * ddx + ddy * ddy);
}

static double segDist(int i, int j) {
    if (segIntersect(ax[i], ay[i], bx[i], by[i], ax[j], ay[j], bx[j], by[j]))
        return 0.0;
    double d1 = ptSegDist((double)ax[i], (double)ay[i], (double)ax[j], (double)ay[j], (double)bx[j], (double)by[j]);
    double d2 = ptSegDist((double)bx[i], (double)by[i], (double)ax[j], (double)ay[j], (double)bx[j], (double)by[j]);
    double d3 = ptSegDist((double)ax[j], (double)ay[j], (double)ax[i], (double)ay[i], (double)bx[i], (double)by[i]);
    double d4 = ptSegDist((double)bx[j], (double)by[j], (double)ax[i], (double)ay[i], (double)bx[i], (double)by[i]);
    double m = d1;
    if (d2 < m) m = d2;
    if (d3 < m) m = d3;
    if (d4 < m) m = d4;
    return m;
}

static double dist[MAXN];
static bool visited[MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) {
            int x, y, l;
            scanf("%d %d %d", &x, &y, &l);
            if (l >= 0) {
                ax[i] = x; ay[i] = y; bx[i] = x + l; by[i] = y;
            } else {
                ax[i] = x; ay[i] = y; bx[i] = x; by[i] = y - l;
            }
        }
        const double INF = 1e18;
        for (int i = 0; i < n; i++) { dist[i] = INF; visited[i] = false; }
        dist[0] = 0.0;
        for (int iter = 0; iter < n; iter++) {
            int u = -1;
            double best = INF;
            for (int v = 0; v < n; v++) {
                if (!visited[v] && dist[v] < best) { best = dist[v]; u = v; }
            }
            if (u == -1) break;
            visited[u] = true;
            for (int v = 0; v < n; v++) {
                if (visited[v]) continue;
                double d = segDist(u, v);
                double nd = d > dist[u] ? d : dist[u];
                if (nd < dist[v]) dist[v] = nd;
            }
        }
        printf("%.2f\n", dist[1]);
    }
    return 0;
}
