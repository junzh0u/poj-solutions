// POJ 1327 - Moving Object Recognition
// Model: claude-sonnet-5
// Per frame: BFS the largest 4-connected 'x' region, take its centroid
// (avg column = X, avg row = Y; the missing +0.5 block-center offset
// cancels in every difference below, so raw indices suffice). With N
// frames (N even), T=N/2, speed = sum_{t=0}^{T-1}(pos(t+T)-pos(t)) / T^2
// (least-squares slope for uniformly spaced linear-motion samples).
// Verified against the sample and against an accepted community
// solution on the discuss board (message 351887) using the same formula.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
using namespace std;

const int MAXN = 258;
static char grid[MAXN][MAXN];
static bool vis[MAXN][MAXN];
static int qx[MAXN * MAXN], qy[MAXN * MAXN];
int m, k;

int main() {
  while (scanf("%d %d", &m, &k) == 2) {
    if (m == 0 && k == 0) break;
    vector<double> px, py;
    for (;;) {
      for (int r = 0; r < k; r++) scanf("%s", grid[r]);
      memset(vis, 0, sizeof(vis));
      long best = -1;
      double bx = 0.0, by = 0.0;
      for (int r = 0; r < k; r++) for (int c = 0; c < m; c++) {
        if (grid[r][c] != 'x' || vis[r][c]) continue;
        int head = 0, tail = 0;
        qx[tail] = c; qy[tail] = r; tail++; vis[r][c] = true;
        long sz = 0; double sx = 0.0, sy = 0.0;
        while (head < tail) {
          int cx = qx[head], cy = qy[head]; head++;
          sz++; sx += cx; sy += cy;
          int ny = cy - 1;
          if (ny >= 0 && grid[ny][cx] == 'x' && !vis[ny][cx]) { vis[ny][cx] = true; qx[tail] = cx; qy[tail] = ny; tail++; }
          ny = cy + 1;
          if (ny < k && grid[ny][cx] == 'x' && !vis[ny][cx]) { vis[ny][cx] = true; qx[tail] = cx; qy[tail] = ny; tail++; }
          int nx = cx - 1;
          if (nx >= 0 && grid[cy][nx] == 'x' && !vis[cy][nx]) { vis[cy][nx] = true; qx[tail] = nx; qy[tail] = cy; tail++; }
          nx = cx + 1;
          if (nx < m && grid[cy][nx] == 'x' && !vis[cy][nx]) { vis[cy][nx] = true; qx[tail] = nx; qy[tail] = cy; tail++; }
        }
        if (sz > best) { best = sz; bx = sx; by = sy; }
      }
      px.push_back(bx / (double)best);
      py.push_back(by / (double)best);
      char sep[MAXN];
      scanf("%s", sep);
      if (sep[0] == '=') break;
    }
    int N = (int)px.size(), T = N / 2;
    double sx = 0.0, sy = 0.0;
    for (int t = 0; t < T; t++) { sx += px[t + T] - px[t]; sy += py[t + T] - py[t]; }
    double vx = sx / ((double)T * T), vy = sy / ((double)T * T);
    if (fabs(vx) < 0.005) vx = 0.0;
    if (fabs(vy) < 0.005) vy = 0.0;
    printf("%.2f %.2f\n", vx, vy);
  }
  return 0;
}
