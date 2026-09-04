// POJ 1307 - Mapping the Route
// Model: claude-sonnet-5
// Approach: simulate the deterministic DFS robot (try West, North, East,
// South in that fixed order at each cell, refusing to re-enter a cell it
// has already visited). The stack of cells held at the moment the goal is
// reached is exactly the printed path (numbered in order); any other cell
// the robot stepped into and later backtracked out of is drawn as "???";
// any cell the robot never reached before the search stopped (because the
// search halts the instant the goal is found) is left blank.
// Ambiguity: none found in the statement itself; the discuss board (msg
// 51626/51628, 3699/3700/3705) confirms the exact blank-line layout used
// below (one blank line under "Maze n", two blank lines after the grid,
// including after the last maze).
#include <cstdio>
#include <cstring>

int R, C, sr, sc, gr, gc;
int wallE[13][13], wallS[13][13]; // 1..R, 1..C
bool visited[13][13];
int label_[13][13]; // 0 = never visited, -1 = visited but not on path, >0 = seq number
int pr[200], pc[200];
int pathLen;
bool found;

bool movable(int r, int c, int dir) {
    // 0=W 1=N 2=E 3=S
    if (dir == 0) return c > 1 && !(wallE[r][c-1] & 1);
    if (dir == 1) return r > 1 && !(wallS[r-1][c] & 1);
    if (dir == 2) return c < C && !(wallE[r][c] & 1);
    return r < R && !(wallS[r][c] & 1);
}

void step(int r, int c, int dr, int dc, int &nr, int &nc) { nr = r+dr; nc = c+dc; }

void dfs(int r, int c) {
    visited[r][c] = true;
    pr[pathLen] = r; pc[pathLen] = c; pathLen++;
    if (r == gr && c == gc) { found = true; return; }
    static const int dr[4] = {0, -1, 0, 1};
    static const int dc[4] = {-1, 0, 1, 0};
    for (int d = 0; d < 4; d++) {
        if (!movable(r, c, d)) continue;
        int nr = r + dr[d], nc = c + dc[d];
        if (visited[nr][nc]) continue;
        dfs(nr, nc);
        if (found) return;
    }
    pathLen--; // dead end: backtrack, cell stays visited but not on the path
}

int main() {
    int mazeNum = 0;
    while (scanf("%d %d %d %d %d %d", &R, &C, &sr, &sc, &gr, &gc) == 6) {
        if (R == 0 && C == 0 && sr == 0 && sc == 0 && gr == 0 && gc == 0) break;
        mazeNum++;
        memset(wallE, 0, sizeof(wallE));
        memset(wallS, 0, sizeof(wallS));
        memset(visited, 0, sizeof(visited));
        memset(label_, 0, sizeof(label_));
        for (int r = 1; r <= R; r++) {
            for (int c = 1; c <= C; c++) {
                int v;
                scanf("%d", &v);
                wallE[r][c] = v & 1;
                wallS[r][c] = (v >> 1) & 1;
            }
        }
        pathLen = 0;
        found = false;
        dfs(sr, sc);
        for (int i = 0; i < pathLen; i++) label_[pr[i]][pc[i]] = i + 1;
        for (int r = 1; r <= R; r++)
            for (int c = 1; c <= C; c++)
                if (visited[r][c] && label_[r][c] == 0) label_[r][c] = -1;

        printf("Maze %d\n\n", mazeNum);
        // top boundary
        for (int c = 1; c <= C; c++) printf("+---");
        printf("+\n");
        for (int r = 1; r <= R; r++) {
            printf("|");
            for (int c = 1; c <= C; c++) {
                int lb = label_[r][c];
                if (lb > 0) printf("%3d", lb);
                else if (lb == -1) printf("???");
                else printf("   ");
                bool wall = (c == C) ? true : (wallE[r][c] != 0);
                printf(wall ? "|" : " ");
            }
            printf("\n");
            for (int c = 1; c <= C; c++) {
                bool wall = (r == R) ? true : (wallS[r][c] != 0);
                printf(wall ? "+---" : "+   ");
            }
            printf("+\n");
        }
        printf("\n\n");
    }
    return 0;
}
