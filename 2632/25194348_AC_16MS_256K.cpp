// POJ 2632 - Crashing Robots
#include <cstdio>
using namespace std;

int main() {
    int K;
    scanf("%d", &K);
    while (K--) {
        int A, B;
        scanf("%d %d", &A, &B);
        int N, M;
        scanf("%d %d", &N, &M);
        int x[105], y[105], dir[105]; // dir: 0=E,1=N,2=W,3=S (CCW order)
        for (int i = 1; i <= N; i++) {
            char c;
            scanf("%d %d %c", &x[i], &y[i], &c);
            if (c == 'E') dir[i] = 0;
            else if (c == 'N') dir[i] = 1;
            else if (c == 'W') dir[i] = 2;
            else dir[i] = 3; // S
        }
        int dx[4] = {1, 0, -1, 0};
        int dy[4] = {0, 1, 0, -1};

        bool crashed = false;
        int crashRobot = -1, crashWith = -1; // crashWith == -2 means wall
        for (int ins = 0; ins < M; ins++) {
            int r;
            char act;
            int rep;
            scanf("%d %c %d", &r, &act, &rep);
            if (crashed) continue; // still must consume input, but skip simulation

            if (act == 'L') {
                dir[r] = (dir[r] + rep) % 4;
            } else if (act == 'R') {
                dir[r] = (dir[r] - rep % 4 + 4) % 4;
            } else { // F
                for (int step = 0; step < rep; step++) {
                    int nx = x[r] + dx[dir[r]];
                    int ny = y[r] + dy[dir[r]];
                    if (nx == 0 || nx == A + 1 || ny == 0 || ny == B + 1) {
                        crashed = true;
                        crashRobot = r;
                        crashWith = -2;
                        break;
                    }
                    x[r] = nx;
                    y[r] = ny;
                    bool hit = false;
                    for (int j = 1; j <= N; j++) {
                        if (j != r && x[j] == x[r] && y[j] == y[r]) {
                            hit = true;
                            crashed = true;
                            crashRobot = r;
                            crashWith = j;
                            break;
                        }
                    }
                    if (hit) break;
                }
            }
        }

        if (!crashed) {
            printf("OK\n");
        } else if (crashWith == -2) {
            printf("Robot %d crashes into the wall\n", crashRobot);
        } else {
            printf("Robot %d crashes into robot %d\n", crashRobot, crashWith);
        }
    }
    return 0;
}
