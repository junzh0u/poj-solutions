// POJ 2875 - In Defence of a Garden
// Model: gpt-5.6-terra
// Mark every unit grid edge on which the robot has ever built a fence.  A
// flood fill from a one-cell exterior border then reaches exactly the cells
// deer can enter; the remaining yard cells are the garden.
// A retraced edge is deliberately just marked again, so construction resumes
// naturally at the next previously unfenced edge of the walk.

#include <cstdio>
#include <cstring>
#include <queue>

using namespace std;

static bool vertical[101][100];
static bool horizontal[100][101];
static bool seen[102][102];

static bool blocked(int x, int y, int nx, int ny) {
    if (x != nx) {
        int line = x < nx ? x : nx;
        return y >= 1 && y <= 100 && vertical[line][y - 1];
    }
    int line = y < ny ? y : ny;
    return x >= 1 && x <= 100 && horizontal[x - 1][line];
}

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc = 1; tc <= cases; ++tc) {
        memset(vertical, 0, sizeof(vertical));
        memset(horizontal, 0, sizeof(horizontal));

        int x, y, moves;
        scanf("%d%d%d", &x, &y, &moves);
        for (int m = 0; m < moves; ++m) {
            char dir[2];
            int distance;
            scanf("%s%d", dir, &distance);
            for (int step = 0; step < distance; ++step) {
                if (dir[0] == 'N') {
                    vertical[x][y] = true;
                    ++y;
                } else if (dir[0] == 'S') {
                    --y;
                    vertical[x][y] = true;
                } else if (dir[0] == 'E') {
                    horizontal[x][y] = true;
                    ++x;
                } else {
                    --x;
                    horizontal[x][y] = true;
                }
            }
        }

        memset(seen, 0, sizeof(seen));
        queue<pair<int, int> > q;
        seen[0][0] = true;
        q.push(make_pair(0, 0));
        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};
        while (!q.empty()) {
            int cx = q.front().first;
            int cy = q.front().second;
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nx = cx + dx[d];
                int ny = cy + dy[d];
                if (nx < 0 || nx > 101 || ny < 0 || ny > 101) continue;
                if (!seen[nx][ny] && !blocked(cx, cy, nx, ny)) {
                    seen[nx][ny] = true;
                    q.push(make_pair(nx, ny));
                }
            }
        }

        int garden = 0;
        for (int i = 1; i <= 100; ++i)
            for (int j = 1; j <= 100; ++j)
                if (!seen[i][j]) ++garden;
        printf("Data Set %d: %d square feet.\n", tc, garden);
    }
    printf("End of Output\n");
    return 0;
}
