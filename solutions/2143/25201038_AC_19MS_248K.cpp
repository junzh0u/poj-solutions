// POJ 2143 - Make a Sequence
// Model: claude-sonnet-5
// Simulate a 3D tic-tac-toe. Balls stack under gravity on n*n pegs; after
// each move check all 13 undirected line directions through the just-placed
// ball for a run of >= m same-colored balls. First player to reach an
// m-sequence wins immediately; further moves (even additional m-sequences)
// are ignored per the statement. If the board fills with no winner, it's a
// draw. Ambiguity: none found; sample covers draw/white/black cases.
#include <cstdio>
#include <cstring>

int n, m, p;
int grid[8][8][8]; // 0 empty, 1 black, 2 white
int height[8][8];

int dirs[13][3] = {
    {1,0,0}, {0,1,0}, {0,0,1},
    {1,1,0}, {1,-1,0}, {1,0,1}, {1,0,-1}, {0,1,1}, {0,1,-1},
    {1,1,1}, {1,1,-1}, {1,-1,1}, {1,-1,-1}
};

bool inb(int x, int y, int z) {
    return x >= 1 && x <= n && y >= 1 && y <= n && z >= 1 && z <= n;
}

bool checkWin(int x, int y, int z, int color) {
    for (int d = 0; d < 13; d++) {
        int dx = dirs[d][0], dy = dirs[d][1], dz = dirs[d][2];
        int cnt = 1;
        int cx = x + dx, cy = y + dy, cz = z + dz;
        while (inb(cx, cy, cz) && grid[cx][cy][cz] == color) {
            cnt++;
            cx += dx; cy += dy; cz += dz;
        }
        cx = x - dx; cy = y - dy; cz = z - dz;
        while (inb(cx, cy, cz) && grid[cx][cy][cz] == color) {
            cnt++;
            cx -= dx; cy -= dy; cz -= dz;
        }
        if (cnt >= m) return true;
    }
    return false;
}

int main() {
    while (scanf("%d %d %d", &n, &m, &p) == 3) {
        if (n == 0 && m == 0 && p == 0) break;
        memset(grid, 0, sizeof(grid));
        memset(height, 0, sizeof(height));
        int winner = 0; // 0 = none/draw, 1 = black, 2 = white
        int endMove = -1;
        for (int i = 1; i <= p; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            if (winner != 0) continue; // game already decided, ignore move
            int color = (i % 2 == 1) ? 1 : 2; // black plays odd moves
            int z = ++height[x][y];
            grid[x][y][z] = color;
            if (checkWin(x, y, z, color)) {
                winner = color;
                endMove = i;
            }
        }
        if (winner == 1) {
            printf("Black %d\n", endMove);
        } else if (winner == 2) {
            printf("White %d\n", endMove);
        } else {
            printf("Draw\n");
        }
    }
    return 0;
}
