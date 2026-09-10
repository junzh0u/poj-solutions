// POJ 1216 - Simulation Wizardry
// Model: claude-sonnet-5
//
// Simulate a pinball on an m*n grid (1<=x<=m, 1<=y<=n) whose edge cells
// are walls. Every timestep the ball attempts to move one cell in its
// current direction. If the target cell is an obstacle (wall or bumper)
// the ball does not move; it "hits": that consumes one base lifetime unit
// (the attempted step) plus the obstacle's cost, and then the direction
// turns 90 degrees clockwise. If the resulting lifetime is still positive
// and the obstacle was a bumper, its value is scored (a hit that kills the
// ball -- brings lifetime to <=0 -- scores nothing, per the statement's
// note about a ball with lifetime one dying on its next move). If the
// target cell is free the ball moves onto it, consuming one lifetime unit.
// The ball's simulation stops once lifetime is non-positive.
//
// This reading (hit = 1 base tick + obstacle cost, scored only if the
// ball survives the hit) was checked by hand-tracing the official sample
// against the discuss board's own diagram (message 144862: the same 4x4
// grid with the ball fully boxed in by two bumpers and two walls) and it
// reproduces all five given per-ball scores (0,0,1,2,2) and the total 5
// exactly, including the "lifetime one dies immediately" edge case.
//
// Direction encoding: 0=+x(right) 1=+y(up) 2=-x(left) 3=-y(down).
// Turning clockwise maps 1->0->3->2->1, i.e. newdir = (dir+3)%4.
//
// Bumpers may have negative cost (extends life) or negative value.
// Balls are read until EOF (no count is given for them).

#include <cstdio>

static int m, n, wallCost;
static bool isBumper[52][52];
static int bumperVal[52][52];
static int bumperCost[52][52];

int main() {
    scanf("%d %d", &m, &n);
    scanf("%d", &wallCost);
    int p;
    scanf("%d", &p);
    for (int i = 0; i < p; ++i) {
        int x, y, v, c;
        scanf("%d %d %d %d", &x, &y, &v, &c);
        isBumper[x][y] = true;
        bumperVal[x][y] = v;
        bumperCost[x][y] = c;
    }

    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};

    int x, y, dir, life;
    int total = 0;
    while (scanf("%d %d %d %d", &x, &y, &dir, &life) == 4) {
        int score = 0;
        while (life > 0) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            bool obstacle = false;
            bool bumper = false;
            int cost = 0, val = 0;
            if (nx == 1 || nx == m || ny == 1 || ny == n) {
                obstacle = true;
                cost = wallCost;
            } else if (isBumper[nx][ny]) {
                obstacle = true;
                bumper = true;
                cost = bumperCost[nx][ny];
                val = bumperVal[nx][ny];
            }
            if (obstacle) {
                int newlife = life - 1 - cost;
                if (newlife > 0 && bumper) score += val;
                life = newlife;
                dir = (dir + 3) % 4;
            } else {
                x = nx;
                y = ny;
                life -= 1;
            }
        }
        printf("%d\n", score);
        total += score;
    }
    printf("%d\n", total);
    return 0;
}
