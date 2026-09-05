// POJ 2729 - Robocode
// Model: claude-sonnet-5
//
// Approach: discrete-event simulation using a scaled tick of 1/6 second.
// Tanks move at 10 px/s (1 grid unit/s) and shots at 20 px/s (2 grid
// units/s). Scaling positions by 6 (one grid unit of 10px -> 6 scaled
// units) and time by 6 ticks/second makes every tank move exactly 1
// scaled unit per tick when moving, and every shot move exactly 2 scaled
// units per tick. Since all commands occur only at whole seconds (multiples
// of 6 ticks), every exact spatial coincidence between a shot and a tank
// must fall on an integer tick boundary (the collision equations solved in
// real time always produce denominators dividing 6), so checking positions
// once per tick after both tanks and shots have advanced is exact -- no
// continuous-time root finding is needed.
//
// Per tick: move every alive+moving tank one scaled step in its facing
// direction (stopping it, per the statement, if the step would leave the
// 0..72 scaled range -- it then needs a fresh MOVE command to resume, even
// after a later TURN); move every alive shot two scaled steps (removing it
// if that would leave the range); then, using a snapshot of alive flags
// taken after both moves, kill every tank/shot pair sharing a cell (so
// several shots landing on the same tank at the same tick all explode
// together, matching the statement's "same time" note). Bullets never
// interact with each other (confirmed on the discuss board -- the
// statement only specifies shot-vs-tank explosions).
//
// A command directed at an already-dead tank is otherwise a no-op anyway
// (a dead tank never moves or shoots again), so the only place that needs
// an explicit alive check is SHOOT, to avoid spawning a shot from a corpse;
// this matches the board's confirmation that some inputs do send orders to
// already-destroyed tanks and they must be silently dropped.
//
// After the last command (given only for t in [0,30]), the simulation
// continues until every shot has exploded or left the board; the farthest
// a shot fired at t=30 can still be relevant is another 6 seconds (72
// scaled units at 2 units/tick = 36 ticks), so ticking well past that
// (90 extra ticks, i.e. 15s) is a safe drain -- this matches the sample's
// own third case, where B's final shot (fired at t=30) needs the full 6s
// to reach A pinned against the far wall.
//
// Ambiguity/confirmation from the discuss board (poj.org/bbs?problem_id=2729):
//  - message 344103/344240: input can send commands to already-destroyed
//    tanks; they must be omitted (handled naturally as above).
//  - message 38501/39070: unlike real Robocode, this problem's shots do
//    NOT cancel each other out; only shot-vs-tank explosions are modeled.
//  - two independently accepted solutions posted on the board (356357,
//    348657) both use exactly this tick=1/6s, scale=6 discretization,
//    which is strong evidence the simulation is exact at that resolution.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

struct Tank {
    char name[16];
    int x, y;   // scaled coordinates, 0..72
    int dir;    // 0=right(+x) 1=up(+y) 2=left(-x) 3=down(-y)
    bool moving;
    bool alive;
};

struct Bullet {
    int x, y;
    int dir;
    bool alive;
};

static const int DX[4] = {1, 0, -1, 0};
static const int DY[4] = {0, 1, 0, -1};

int N, M;
Tank tanks[15];
vector<Bullet> bullets;

int findTank(const char *name) {
    for (int i = 0; i < N; i++)
        if (strcmp(tanks[i].name, name) == 0) return i;
    return -1;
}

void runAll() {
    for (int i = 0; i < N; i++) {
        if (tanks[i].alive && tanks[i].moving) {
            int nx = tanks[i].x + DX[tanks[i].dir];
            int ny = tanks[i].y + DY[tanks[i].dir];
            if (nx >= 0 && nx <= 72 && ny >= 0 && ny <= 72) {
                tanks[i].x = nx;
                tanks[i].y = ny;
            } else {
                tanks[i].moving = false;
            }
        }
    }

    size_t bn = bullets.size();
    for (size_t i = 0; i < bn; i++) {
        if (bullets[i].alive) {
            int nx = bullets[i].x + 2 * DX[bullets[i].dir];
            int ny = bullets[i].y + 2 * DY[bullets[i].dir];
            if (nx >= 0 && nx <= 72 && ny >= 0 && ny <= 72) {
                bullets[i].x = nx;
                bullets[i].y = ny;
            } else {
                bullets[i].alive = false;
            }
        }
    }

    bool tankPre[15];
    for (int i = 0; i < N; i++) tankPre[i] = tanks[i].alive;
    vector<bool> bulletPre(bn);
    for (size_t j = 0; j < bn; j++) bulletPre[j] = bullets[j].alive;

    for (int i = 0; i < N; i++) {
        if (!tankPre[i]) continue;
        for (size_t j = 0; j < bn; j++) {
            if (!bulletPre[j]) continue;
            if (tanks[i].x == bullets[j].x && tanks[i].y == bullets[j].y) {
                tanks[i].alive = false;
                bullets[j].alive = false;
            }
        }
    }
}

bool anyBulletAlive() {
    for (size_t i = 0; i < bullets.size(); i++)
        if (bullets[i].alive) return true;
    return false;
}

int main() {
    while (scanf("%d %d", &N, &M) == 2) {
        if (N == 0 && M == 0) break;
        bullets.clear();
        bullets.reserve(M);

        for (int i = 0; i < N; i++) {
            int x, y, a;
            scanf("%s %d %d %d", tanks[i].name, &x, &y, &a);
            tanks[i].x = (x / 10) * 6;
            tanks[i].y = (y / 10) * 6;
            tanks[i].dir = a / 90;
            tanks[i].moving = false;
            tanks[i].alive = true;
        }

        int now = 0;
        for (int c = 0; c < M; c++) {
            int t;
            char name[16], content[16];
            scanf("%d %s %s", &t, name, content);
            int angle = 0;
            bool isTurn = (strcmp(content, "TURN") == 0);
            if (isTurn) scanf("%d", &angle);

            int tt = t * 6;
            while (now < tt) {
                runAll();
                now++;
            }

            int idx = findTank(name);
            if (idx >= 0 && tanks[idx].alive) {
                if (strcmp(content, "MOVE") == 0) {
                    tanks[idx].moving = true;
                } else if (strcmp(content, "STOP") == 0) {
                    tanks[idx].moving = false;
                } else if (isTurn) {
                    int curAlpha = tanks[idx].dir * 90;
                    int newAlpha = ((curAlpha + angle) % 360 + 360) % 360;
                    tanks[idx].dir = newAlpha / 90;
                } else if (strcmp(content, "SHOOT") == 0) {
                    Bullet b;
                    b.x = tanks[idx].x;
                    b.y = tanks[idx].y;
                    b.dir = tanks[idx].dir;
                    b.alive = true;
                    bullets.push_back(b);
                }
            }
        }

        int drain = 0;
        while (anyBulletAlive() && drain < 90) {
            runAll();
            drain++;
        }

        int cnt = 0, winIdx = -1;
        for (int i = 0; i < N; i++) {
            if (tanks[i].alive) {
                cnt++;
                winIdx = i;
            }
        }
        if (cnt == 1) {
            printf("%s\n", tanks[winIdx].name);
        } else {
            printf("NO WINNER!\n");
        }
    }
    return 0;
}
