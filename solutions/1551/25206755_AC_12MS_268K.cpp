// POJ 1551 - Data Mining?
// Model: claude-sonnet-5
// Approach: brute force over every non-mine starting cell (board has <=40
// cells total, so at most 40 starts). For each start, simulate the casual
// player's deterministic deduction rules to a fixed point:
//   for a cleared cell, let f = flagged neighbors, c = still-hidden
//   neighbors (mine or not - the player can't tell), m = true mine count
//   among neighbors. If f == m, all remaining hidden neighbors are safe,
//   clear them. Else if f + c == m, all remaining hidden neighbors must be
//   mines, flag them. Repeat until no cell can make further progress, then
//   count the hidden non-mine cells left. Answer is the minimum over all
//   valid starting cells.
// No statement ambiguity found; matched the sample and a shared board on
// the discuss board (message 352471) which uses the identical rule
// definitions (c counts hidden cells regardless of true identity).
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 45;
int R, C;
bool mine[MAXN][MAXN];
int mineCnt[MAXN][MAXN];

enum State { HIDDEN, FLAGGED, CLEARED };
State st[MAXN][MAXN];

int dx[8] = {-1,-1,-1,0,0,1,1,1};
int dy[8] = {-1,0,1,-1,1,-1,0,1};

bool inRange(int x, int y) {
    return x >= 1 && x <= R && y >= 1 && y <= C;
}

int simulate(int sx, int sy) {
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            st[i][j] = HIDDEN;
    st[sx][sy] = CLEARED;

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 1; i <= R; i++) {
            for (int j = 1; j <= C; j++) {
                if (st[i][j] != CLEARED) continue;
                int f = 0, c = 0;
                int m = mineCnt[i][j];
                for (int k = 0; k < 8; k++) {
                    int nx = i + dx[k], ny = j + dy[k];
                    if (!inRange(nx, ny)) continue;
                    if (st[nx][ny] == FLAGGED) f++;
                    else if (st[nx][ny] == HIDDEN) c++;
                }
                if (c == 0) continue;
                if (f == m) {
                    for (int k = 0; k < 8; k++) {
                        int nx = i + dx[k], ny = j + dy[k];
                        if (!inRange(nx, ny)) continue;
                        if (st[nx][ny] == HIDDEN) st[nx][ny] = CLEARED;
                    }
                    changed = true;
                } else if (f + c == m) {
                    for (int k = 0; k < 8; k++) {
                        int nx = i + dx[k], ny = j + dy[k];
                        if (!inRange(nx, ny)) continue;
                        if (st[nx][ny] == HIDDEN) st[nx][ny] = FLAGGED;
                    }
                    changed = true;
                }
            }
        }
    }

    int remaining = 0;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            if (!mine[i][j] && st[i][j] == HIDDEN) remaining++;
    return remaining;
}

int main() {
    char line[64];
    while (scanf("%d %d", &R, &C) == 2) {
        if (R == 0 && C == 0) break;
        for (int i = 1; i <= R; i++) {
            scanf("%s", line);
            for (int j = 1; j <= C; j++)
                mine[i][j] = (line[j - 1] == 'M');
        }
        for (int i = 1; i <= R; i++) {
            for (int j = 1; j <= C; j++) {
                if (mine[i][j]) continue;
                int cnt = 0;
                for (int k = 0; k < 8; k++) {
                    int nx = i + dx[k], ny = j + dy[k];
                    if (inRange(nx, ny) && mine[nx][ny]) cnt++;
                }
                mineCnt[i][j] = cnt;
            }
        }

        int best = R * C + 1;
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++) {
                if (mine[i][j]) continue;
                int res = simulate(i, j);
                if (res < best) best = res;
            }
        printf("%d\n", best);
    }
    return 0;
}
