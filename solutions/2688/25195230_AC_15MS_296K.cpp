// POJ 2688 - Cleaning Robot
// Model: claude-opus-5
//
// Grid is at most 20x20 with at most 10 dirty tiles and one robot.  BFS from
// the robot and from every dirty tile gives all pairwise shortest walk lengths
// (obstacles 'x' are blocked, everything else is walkable and may be revisited,
// so plain 4-neighbour BFS on the grid is exact).  If any dirty tile is
// unreachable from the robot the answer is -1; otherwise the remaining question
// is a shortest Hamiltonian path from the robot over the dirty tiles, solved by
// the standard bitmask DP: dp[mask][i] = fewest moves to have cleaned exactly
// the tiles in mask, standing on dirty tile i.  11 * 1024 * 10 transitions.
//
// Notes: input is a stream of maps terminated by "0 0" (also stop at EOF), and
// a map with no dirty tiles answers 0.  Everything fits in int (<= 400 tiles,
// so a tour is bounded well below 2^31).

#include <cstdio>
#include <cstring>

static const int INF = 1000000000;

int w, h;
char grid[25][25];

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int qx[500], qy[500];
int dist[25][25];

int px[12], py[12];   /* index 0 = robot, 1..n = dirty tiles */
int cost[12][12];
int dp[1 << 10][10];

/* BFS from (sx, sy); fills dist[][] with -1 for unreachable cells. */
void bfs(int sx, int sy) {
    int y, x, head, tail, d;
    for (y = 0; y < h; ++y)
        for (x = 0; x < w; ++x) dist[y][x] = -1;
    head = tail = 0;
    dist[sy][sx] = 0;
    qx[tail] = sx; qy[tail] = sy; ++tail;
    while (head < tail) {
        int cx = qx[head], cy = qy[head];
        ++head;
        for (d = 0; d < 4; ++d) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
            if (grid[ny][nx] == 'x') continue;
            if (dist[ny][nx] != -1) continue;
            dist[ny][nx] = dist[cy][cx] + 1;
            qx[tail] = nx; qy[tail] = ny; ++tail;
        }
    }
}

int main() {
    while (scanf("%d %d", &w, &h) == 2 && (w != 0 || h != 0)) {
        int y, x, i, j, mask;
        int n = 0;
        int full, ans;
        int bad = 0;

        for (y = 0; y < h; ++y) scanf("%s", grid[y]);

        px[0] = py[0] = -1;
        for (y = 0; y < h; ++y) {
            for (x = 0; x < w; ++x) {
                if (grid[y][x] == 'o') { px[0] = x; py[0] = y; }
                else if (grid[y][x] == '*') { ++n; px[n] = x; py[n] = y; }
            }
        }

        if (n == 0) { printf("0\n"); continue; }

        for (i = 0; i <= n; ++i) {
            bfs(px[i], py[i]);
            for (j = 0; j <= n; ++j) {
                int dd = dist[py[j]][px[j]];
                cost[i][j] = (dd < 0) ? INF : dd;
            }
        }
        for (j = 1; j <= n; ++j)
            if (cost[0][j] >= INF) bad = 1;
        if (bad) { printf("-1\n"); continue; }

        full = 1 << n;
        for (mask = 0; mask < full; ++mask)
            for (i = 0; i < n; ++i) dp[mask][i] = INF;
        for (i = 0; i < n; ++i) dp[1 << i][i] = cost[0][i + 1];

        for (mask = 1; mask < full; ++mask) {
            for (i = 0; i < n; ++i) {
                if (!(mask & (1 << i))) continue;
                if (dp[mask][i] >= INF) continue;
                for (j = 0; j < n; ++j) {
                    if (mask & (1 << j)) continue;
                    int nd = dp[mask][i] + cost[i + 1][j + 1];
                    int nm = mask | (1 << j);
                    if (nd < dp[nm][j]) dp[nm][j] = nd;
                }
            }
        }

        ans = INF;
        for (i = 0; i < n; ++i)
            if (dp[full - 1][i] < ans) ans = dp[full - 1][i];
        printf("%d\n", ans >= INF ? -1 : ans);
    }
    return 0;
}
