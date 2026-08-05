// POJ 3669 - Meteor Shower
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

// Meteors only ever hit x,y in [0,300], and their blast destroys the four
// rectilinearly adjacent cells too, so the farthest a destroyed cell can ever
// be is [0,301]. Cells at index 302 are therefore guaranteed to never be
// touched by any meteor -- reaching one of those (or any other cell whose
// destroy time stays "never") is automatically a safe place to stop. Using a
// 303x303 grid (indices 0..302) lets a single uniform BFS check ("is this
// cell's destroy time still INF when I pop it?") find the answer without any
// special-cased "stepped off the meteor-affected region" logic.
const int MAXC = 303; // valid coordinates 0..302
const int INF = 1 << 30;

int destroyTime[MAXC][MAXC];
int dist_[MAXC][MAXC];
bool visited[MAXC][MAXC];

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;

    for (int i = 0; i < MAXC; ++i)
        for (int j = 0; j < MAXC; ++j)
            destroyTime[i][j] = INF;

    int dx[5] = {0, 1, -1, 0, 0};
    int dy[5] = {0, 0, 0, 1, -1};

    for (int i = 0; i < m; ++i) {
        int x, y, t;
        scanf("%d %d %d", &x, &y, &t);
        for (int k = 0; k < 5; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx < 0 || ny < 0 || nx >= MAXC || ny >= MAXC) continue;
            if (t < destroyTime[nx][ny]) destroyTime[nx][ny] = t;
        }
    }

    memset(visited, 0, sizeof(visited));

    if (destroyTime[0][0] <= 0) {
        // Bessie is destroyed at time 0, the instant she starts.
        printf("-1\n");
        return 0;
    }

    queue<pair<int, int> > q;
    dist_[0][0] = 0;
    visited[0][0] = true;
    q.push(make_pair(0, 0));

    int ddx[4] = {1, -1, 0, 0};
    int ddy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        int cx = cur.first, cy = cur.second;
        int d = dist_[cx][cy];

        if (destroyTime[cx][cy] == INF) {
            printf("%d\n", d);
            return 0;
        }

        for (int k = 0; k < 4; ++k) {
            int nx = cx + ddx[k];
            int ny = cy + ddy[k];
            if (nx < 0 || ny < 0 || nx >= MAXC || ny >= MAXC) continue;
            if (visited[nx][ny]) continue;
            int nd = d + 1;
            if (nd < destroyTime[nx][ny]) {
                visited[nx][ny] = true;
                dist_[nx][ny] = nd;
                q.push(make_pair(nx, ny));
            }
        }
    }

    printf("-1\n");
    return 0;
}
