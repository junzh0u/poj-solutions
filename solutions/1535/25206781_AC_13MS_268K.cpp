// POJ 1535 - Forgetful
// Model: claude-sonnet-5
//
// Approach: Let dirF(t) be the forward walker's facing at step t (starting
// East from the source) and dirB(t) the backward walker's facing (starting
// West from the destination), both driven by the SAME command string.
// Since dirB(0) = -dirF(0) and every L/R command rotates both facings by
// the same angle, dirB(t) = -dirF(t) for all t. Then for an F or B command,
// the displacement applied to the backward walker's position is always the
// negation of the displacement applied to the forward walker's position
// (regardless of whether the command is F or B), so
//   posF(t) + posB(t)
// is invariant over the whole string, equal to source + destination = C.
// Hence posB(t) = C - posF(t) at every instant: the backward walker's
// position is completely determined by the forward walker's position, and
// need not be tracked/searched separately.
//
// So a valid string exists iff there is a 4-directional path (any walk
// realizable by inserting free L/R turns before each move) from source to
// destination such that at every visited cell p, BOTH p and C-p are inside
// the grid and are not closed crossroads. This reduces to a plain BFS/DFS
// on an m x n grid with a derived "blocked" mask, which is small and fast
// even though the true state space (posF,dirF,posB) would be huge.
//
// Board pitfall (poj.org/showmessage?message_id=2283/50613): a naive
// solution that only checks the forward path can walk the backward walker
// off the edge of the grid; the mirror position C-p must be bounds-checked
// independently, not assumed to stay on the grid.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

int m, n;
bool closedCell[100][100];
bool good_[100][100];
bool visited[100][100];

int main() {
    while (scanf("%d %d", &m, &n) == 2) {
        if (m == 0 && n == 0) break;
        int sx, sy, dx, dy;
        scanf("%d %d %d %d", &sx, &sy, &dx, &dy);
        int N;
        scanf("%d", &N);
        memset(closedCell, 0, sizeof(closedCell));
        for (int i = 0; i < N; ++i) {
            int cx, cy;
            scanf("%d %d", &cx, &cy);
            if (cx >= 0 && cx < m && cy >= 0 && cy < n)
                closedCell[cx][cy] = true;
        }
        int Cx = sx + dx, Cy = sy + dy;
        for (int x = 0; x < m; ++x) {
            for (int y = 0; y < n; ++y) {
                bool g = !closedCell[x][y];
                if (g) {
                    int mx = Cx - x, my = Cy - y;
                    if (mx < 0 || mx >= m || my < 0 || my >= n) g = false;
                    else if (closedCell[mx][my]) g = false;
                }
                good_[x][y] = g;
            }
        }
        bool ok = false;
        if (good_[sx][sy] && good_[dx][dy]) {
            memset(visited, 0, sizeof(visited));
            queue<pair<int,int> > q;
            q.push(make_pair(sx, sy));
            visited[sx][sy] = true;
            static const int dxs[4] = {1, -1, 0, 0};
            static const int dys[4] = {0, 0, 1, -1};
            while (!q.empty()) {
                pair<int,int> cur = q.front(); q.pop();
                if (cur.first == dx && cur.second == dy) { ok = true; break; }
                for (int k = 0; k < 4; ++k) {
                    int nx = cur.first + dxs[k], ny = cur.second + dys[k];
                    if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                    if (!good_[nx][ny] || visited[nx][ny]) continue;
                    visited[nx][ny] = true;
                    q.push(make_pair(nx, ny));
                }
            }
        }
        printf(ok ? "YES\n" : "NO\n");
    }
    return 0;
}
