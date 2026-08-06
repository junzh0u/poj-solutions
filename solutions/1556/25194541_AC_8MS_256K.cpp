// POJ 1556 - The Doors
//
// A 10x10 chamber, start (0,5), goal (10,5), up to 18 vertical interior walls.
// A wall line at x with doorway ends y0<y1<y2<y3 is really three solid pieces:
// (x,0)-(x,y0), (x,y1)-(x,y2), (x,y3)-(x,10); the two gaps [y0,y1] and [y2,y3]
// are the only way through.  The chamber's own sides are not obstacles - the
// walls span the full height, so nothing can be walked around anyway.
//
// Shortest-path-in-a-polygon: the optimum is a polyline bending only at
// obstacle corners, i.e. at doorway ends.  So build a visibility graph over
// {start, goal, all 4n doorway ends} and run Dijkstra on it.
//
// Visibility is cheap here because every obstacle is a vertical line: for a
// candidate segment p->q with px < qx, walk the walls with px < wx < qx,
// linearly interpolate the segment's y at wx, and require that y to lie in one
// of that wall's two doorway intervals.  No general segment intersection is
// needed.  Endpoints of a wall are *not* blocking for a segment that ends on
// them (a wall at wx == px or wx == qx is skipped), and grazing a doorway tip
// exactly is legal - hence the inclusive comparisons with an epsilon.  That
// matters: every bend of an optimal path touches a tip, and sample 1
// (one wall, doorway [4,6]) answers 10.00 only because y = 5 counts as inside.
//
// Vertical hops along a doorway ((x,y0)-(x,y1) and (x,y2)-(x,y3)) are added as
// edges too.  They are genuinely free space, and adding them costs nothing;
// the wall's solid middle is never joined this way.
//
// Ambiguities settled from the statement/samples: coordinates are reals (4.5
// appears in the sample), the y lists are increasing so a doorway can touch the
// floor or ceiling, and the answer is printed with exactly two decimals.
// Datasets repeat until a wall count of -1.
//
// n <= 18 => at most 74 nodes; edge construction is O(n * nodes^2) ~ 1e5 per
// dataset, so even thousands of datasets finish well inside the 1s limit.
#include <cstdio>
#include <cmath>

static const double EPS = 1e-9;
static const double INF = 1e18;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != -1) {
        double wx[20], wy[20][4];
        for (int i = 0; i < n; i++) {
            scanf("%lf", &wx[i]);
            for (int k = 0; k < 4; k++) scanf("%lf", &wy[i][k]);
        }
        int m = 4 * n + 2;
        double px[80], py[80];
        px[0] = 0.0; py[0] = 5.0;
        for (int i = 0; i < n; i++)
            for (int k = 0; k < 4; k++) {
                px[1 + 4 * i + k] = wx[i];
                py[1 + 4 * i + k] = wy[i][k];
            }
        px[m - 1] = 10.0; py[m - 1] = 5.0;

        static double d[80][80];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) d[i][j] = (i == j) ? 0.0 : INF;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) {
                if (px[i] >= px[j] - EPS) continue; /* only left-to-right */
                bool ok = true;
                for (int k = 0; k < n && ok; k++) {
                    if (wx[k] <= px[i] + EPS || wx[k] >= px[j] - EPS) continue;
                    double t = (wx[k] - px[i]) / (px[j] - px[i]);
                    double y = py[i] + t * (py[j] - py[i]);
                    bool pass = (y >= wy[k][0] - EPS && y <= wy[k][1] + EPS) ||
                                (y >= wy[k][2] - EPS && y <= wy[k][3] + EPS);
                    if (!pass) ok = false;
                }
                if (ok) {
                    double dx = px[j] - px[i], dy = py[j] - py[i];
                    double w = sqrt(dx * dx + dy * dy);
                    d[i][j] = w;
                    d[j][i] = w;
                }
            }

        /* vertical hops along a doorway: both endpoints are free space */
        for (int k = 0; k < n; k++) {
            int b = 1 + 4 * k;
            double w01 = wy[k][1] - wy[k][0];
            double w23 = wy[k][3] - wy[k][2];
            if (w01 < d[b][b + 1]) { d[b][b + 1] = w01; d[b + 1][b] = w01; }
            if (w23 < d[b + 2][b + 3]) { d[b + 2][b + 3] = w23; d[b + 3][b + 2] = w23; }
        }

        /* Dijkstra from the start over the visibility graph */
        static double dist[80];
        static bool done[80];
        for (int i = 0; i < m; i++) { dist[i] = INF; done[i] = false; }
        dist[0] = 0.0;
        for (int it = 0; it < m; it++) {
            int u = -1;
            for (int i = 0; i < m; i++)
                if (!done[i] && (u < 0 || dist[i] < dist[u])) u = i;
            if (u < 0 || dist[u] >= INF) break;
            done[u] = true;
            for (int v = 0; v < m; v++)
                if (d[u][v] < INF && dist[u] + d[u][v] < dist[v])
                    dist[v] = dist[u] + d[u][v];
        }

        printf("%.2f\n", dist[m - 1]);
    }
    return 0;
}
