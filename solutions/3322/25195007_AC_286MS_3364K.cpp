// POJ 3322 - Bloxorz I
// Model: claude-sonnet-5
// Approach: BFS over box states (r,c,orientation) where orientation is
// 0 = standing on single cell (r,c), 1 = lying horizontally on (r,c)-(r,c+1),
// 2 = lying vertically on (r,c)-(r+1,c). Each of the 4 possible rolls from a
// state is generated explicitly and validated against the grid: an empty
// cell '#' may never be covered by any part of the box (bounds-checked
// before any array access, since a stale WA report on the discuss board
// traced to exactly this kind of out-of-bounds slip); an easily-broken cell
// 'E' may be one of the two cells the box lies on but can never be the sole
// cell a standing box rests on. The initial position may be a single 'X'
// (already standing) or two neighbouring 'X's (already lying, horizontally
// or vertically - the discuss board flags that the starting orientation is
// not always vertical). Goal is reaching the standing (orientation 0) state
// on the unique 'O' cell. Answer is the BFS depth, or "Impossible" if the
// goal state is never reached.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 502;
char grid[MAXN][MAXN];
int R, C;

inline bool inb(int r, int c) {
    return r >= 0 && r < R && c >= 0 && c < C;
}
inline bool solidCell(int r, int c) {
    // in-bounds and not empty; caller must check inb first for lying cells,
    // but this is safe to call standalone too.
    return inb(r, c) && grid[r][c] != '#';
}

struct State {
    int r, c, t; // t: 0 standing, 1 horiz, 2 vert
};

inline State mkState(int r, int c, int t) {
    State s;
    s.r = r; s.c = c; s.t = t;
    return s;
}

int dist_[MAXN][MAXN][3];

int main() {
    while (scanf("%d %d", &R, &C) == 2) {
        if (R == 0 && C == 0) break;
        for (int i = 0; i < R; ++i) {
            scanf("%s", grid[i]);
        }
        int xr[2], xc[2], xn = 0;
        int orr = -1, occ = -1;
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < C; ++j) {
                if (grid[i][j] == 'X') {
                    if (xn < 2) { xr[xn] = i; xc[xn] = j; }
                    xn++;
                } else if (grid[i][j] == 'O') {
                    orr = i; occ = j;
                }
            }
        }
        State start;
        if (xn == 1) {
            start.r = xr[0]; start.c = xc[0]; start.t = 0;
        } else {
            // two neighbouring X's; anchor = the one with smaller row, or
            // if same row, smaller col.
            int a = 0, b = 1;
            if (xr[a] == xr[b]) {
                // horizontal neighbours
                int lc = xc[a] < xc[b] ? xc[a] : xc[b];
                start.r = xr[a]; start.c = lc; start.t = 1;
            } else {
                int lr = xr[a] < xr[b] ? xr[a] : xr[b];
                start.r = lr; start.c = xc[a]; start.t = 2;
            }
        }

        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                for (int k = 0; k < 3; ++k)
                    dist_[i][j][k] = -1;

        dist_[start.r][start.c][start.t] = 0;
        queue<State> q;
        q.push(start);

        int answer = -1;
        if (start.t == 0 && start.r == orr && start.c == occ) {
            answer = 0;
        }

        while (!q.empty() && answer < 0) {
            State cur = q.front(); q.pop();
            int r = cur.r, c = cur.c, t = cur.t;
            int d = dist_[r][c][t];

            State nxt[4];
            int nn = 0;
            if (t == 0) {
                nxt[nn++] = mkState(r, c - 2, 1);
                nxt[nn++] = mkState(r, c + 1, 1);
                nxt[nn++] = mkState(r - 2, c, 2);
                nxt[nn++] = mkState(r + 1, c, 2);
            } else if (t == 1) {
                nxt[nn++] = mkState(r, c - 1, 0);
                nxt[nn++] = mkState(r, c + 2, 0);
                nxt[nn++] = mkState(r - 1, c, 1);
                nxt[nn++] = mkState(r + 1, c, 1);
            } else { // t == 2
                nxt[nn++] = mkState(r - 1, c, 0);
                nxt[nn++] = mkState(r + 2, c, 0);
                nxt[nn++] = mkState(r, c - 1, 2);
                nxt[nn++] = mkState(r, c + 1, 2);
            }

            for (int i = 0; i < nn; ++i) {
                int nr = nxt[i].r, nc = nxt[i].c, nt = nxt[i].t;
                bool ok = false;
                if (nt == 0) {
                    ok = inb(nr, nc) && grid[nr][nc] != '#' && grid[nr][nc] != 'E';
                } else if (nt == 1) {
                    ok = inb(nr, nc) && inb(nr, nc + 1) &&
                         grid[nr][nc] != '#' && grid[nr][nc + 1] != '#';
                } else { // nt == 2
                    ok = inb(nr, nc) && inb(nr + 1, nc) &&
                         grid[nr][nc] != '#' && grid[nr + 1][nc] != '#';
                }
                if (!ok) continue;
                if (dist_[nr][nc][nt] != -1) continue;
                dist_[nr][nc][nt] = d + 1;
                if (nt == 0 && nr == orr && nc == occ) {
                    answer = d + 1;
                    break;
                }
                q.push(mkState(nr, nc, nt));
            }
        }

        if (answer >= 0) {
            printf("%d\n", answer);
        } else {
            printf("Impossible\n");
        }
    }
    return 0;
}
