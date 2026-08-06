// POJ 3026 - Borg Maze
//
// A group starts at 'S' and must reach every 'A'; the group may split at 'S'
// and at every alien, and the cost is the total distance walked by all groups.
// Splitting freely means the walked edges form a tree spanning {S} U {aliens}
// in the graph whose edge weights are shortest maze distances, so the answer is
// the weight of a minimum spanning tree over those <= 101 points.
//
// BFS from each interesting point over the 4-neighbour grid gives the all-pairs
// distance matrix (<= 101 * 2500 cells), then Prim over the dense matrix.
// At the stated maxima (50 cases of 50x50 with 100 aliens) this runs in ~0.06s
// locally, far inside the 1000MS limit.
//
// Statement ambiguities / input traps:
//  - ' ' (space) is a legal open cell, so maze rows cannot be read with token
//    based input. The dimension line is parsed out of a whole line read with
//    fgets (it may carry trailing blanks or junk after the two integers), and
//    each of the y maze rows is then read as a whole line.
//  - Rows may be shorter than x when trailing spaces have been stripped (POJ's
//    own sample has rows both shorter and longer than x), so a short row is
//    padded with spaces -- what was stripped could only have been blanks.
//    Characters past column x are ignored.
//  - "everyone is reachable" is taken at face value, but Prim still stops on an
//    infinite edge rather than adding a garbage weight.
//  - The total fits comfortably in int (<= ~101 * 2500), so no long long: POJ's
//    ancient toolchain is unreliable about %lld.

#include <cstdio>
#include <cstring>

static const int MAXD = 55;
static const int MAXP = 110;
static const int INF = 1000000000;

static char g[MAXD][MAXD];
static int id[MAXD][MAXD];
static int px[MAXP], py[MAXP];
static int d[MAXP][MAXP];
static int dist[MAXD][MAXD];
static int qx[MAXD * MAXD], qy[MAXD * MAXD];
static int lowc[MAXP];
static bool used[MAXP];
static char line[4096];

static const int dx4[4] = {1, -1, 0, 0};
static const int dy4[4] = {0, 0, 1, -1};

int main() {
    int T;
    /* first line: number of test cases */
    while (fgets(line, sizeof(line), stdin)) {
        if (sscanf(line, "%d", &T) == 1) break;
    }
    while (T-- > 0) {
        int x = 0, y = 0;
        /* dimension line: read whole line, parse two ints, discard the rest */
        while (fgets(line, sizeof(line), stdin)) {
            if (sscanf(line, "%d %d", &x, &y) == 2) break;
        }
        int i, j, k;
        for (i = 0; i < y; i++) {
            if (!fgets(line, sizeof(line), stdin)) line[0] = '\0';
            int len = (int)strlen(line);
            while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
                line[--len] = '\0';
            }
            for (j = 0; j < x; j++) {
                g[i][j] = (j < len) ? line[j] : ' ';
            }
        }
        int n = 0;
        for (i = 0; i < y; i++) {
            for (j = 0; j < x; j++) {
                if (g[i][j] == 'A' || g[i][j] == 'S') {
                    id[i][j] = n;
                    px[n] = j;
                    py[n] = i;
                    n++;
                } else {
                    id[i][j] = -1;
                }
            }
        }
        for (k = 0; k < n; k++) {
            for (i = 0; i < y; i++)
                for (j = 0; j < x; j++) dist[i][j] = -1;
            int head = 0, tail = 0;
            qx[tail] = px[k];
            qy[tail] = py[k];
            tail++;
            dist[py[k]][px[k]] = 0;
            for (j = 0; j < n; j++) d[k][j] = INF;
            d[k][k] = 0;
            int found = 1;
            while (head < tail && found < n) {
                int cx = qx[head], cy = qy[head];
                head++;
                int D = dist[cy][cx];
                for (int t = 0; t < 4; t++) {
                    int nx = cx + dx4[t], ny = cy + dy4[t];
                    if (nx < 0 || ny < 0 || nx >= x || ny >= y) continue;
                    if (g[ny][nx] == '#') continue;
                    if (dist[ny][nx] != -1) continue;
                    dist[ny][nx] = D + 1;
                    if (id[ny][nx] >= 0) {
                        d[k][id[ny][nx]] = D + 1;
                        found++;
                    }
                    qx[tail] = nx;
                    qy[tail] = ny;
                    tail++;
                }
            }
        }
        /* Prim */
        int total = 0;
        for (i = 0; i < n; i++) {
            used[i] = false;
            lowc[i] = INF;
        }
        if (n > 0) {
            lowc[0] = 0;
            for (k = 0; k < n; k++) {
                int best = -1;
                for (i = 0; i < n; i++) {
                    if (!used[i] && (best == -1 || lowc[i] < lowc[best])) best = i;
                }
                if (best == -1 || lowc[best] >= INF) break; /* disconnected */
                used[best] = true;
                total += lowc[best];
                for (i = 0; i < n; i++) {
                    if (!used[i] && d[best][i] < lowc[i]) lowc[i] = d[best][i];
                }
            }
        }
        printf("%d\n", total);
    }
    return 0;
}
