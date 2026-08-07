// POJ 2157 - Maze
// Model: claude-opus-5
//
// Moves are reversible and keys are permanent, so the set of cells Acm can
// ever stand on is the least fixpoint of: flood fill from S through '.', key
// cells and already-opened doors; whenever every key cell of a door letter has
// been swept into that region, the door opens and the flood is repeated.  At
// most five doors, so at most five rounds of an O(M*N) flood.
//
// Statement points that decide the answer:
//   * "to open a door he needs to find ALL the door's keys (at least one)" --
//     a door letter with no key anywhere in the maze can never be opened
//     (vacuous truth would open it, which the parenthetical rules out).
//   * A maze may contain several 'G' cells (and a letter may label several
//     door cells); answer YES iff the flood ever touches SOME 'G', rather than
//     testing one remembered goal position.  A maze may also contain no 'G' at
//     all, so nothing may be carried over from the previous test case.
//   * 'G' is a goal marker, not a corridor: it is never walked through (which
//     can never change an answer -- touching one already means YES).  An 'S'
//     block is an ordinary empty block; if a maze carried several, the start
//     is the last one read.

#include <cstdio>
#include <cstring>

static const int MAXD = 64;

int m, n;
char g[MAXD][MAXD + 2];
bool opened[5];
int total[5], found_[5];
bool vis[MAXD][MAXD];
int qx[MAXD * MAXD], qy[MAXD * MAXD];

static const int dx[4] = {1, -1, 0, 0};
static const int dy[4] = {0, 0, 1, -1};

/* Flood from (sx,sy) over currently walkable cells.  Fills found_ with the
   number of key cells reached per letter; returns true if a 'G' is touched. */
static bool flood(int sx, int sy)
{
    int head = 0, tail = 0, i, k;
    bool goal = false;

    memset(vis, 0, sizeof(vis));
    for (i = 0; i < 5; i++) found_[i] = 0;

    vis[sx][sy] = true;
    qx[tail] = sx; qy[tail] = sy; tail++;

    while (head < tail) {
        int x = qx[head], y = qy[head];
        head++;
        for (k = 0; k < 4; k++) {
            int nx = x + dx[k], ny = y + dy[k];
            char c;
            if (nx < 0 || ny < 0 || nx >= m || ny >= n) continue;
            if (vis[nx][ny]) continue;
            c = g[nx][ny];
            if (c == 'G') { goal = true; continue; }   /* goal, not a corridor */
            if (c == 'X') continue;                    /* wall */
            if (c >= 'A' && c <= 'E' && !opened[c - 'A']) continue;
            if (c >= 'a' && c <= 'e') found_[c - 'a']++;
            vis[nx][ny] = true;
            qx[tail] = nx; qy[tail] = ny; tail++;
        }
    }
    return goal;
}

int main()
{
    while (scanf("%d %d", &m, &n) == 2 && (m != 0 || n != 0)) {
        int i, j, sx = -1, sy = -1;
        bool ans = false;

        int rows = m, cols = n;
        if (rows < 0) rows = 0;
        if (cols < 0) cols = 0;

        for (i = 0; i < 5; i++) { total[i] = 0; opened[i] = false; }

        /* Rows past the array (never happens within the stated 1<M<20, but a
           malformed case must not corrupt memory) are read and dropped. */
        for (i = 0; i < rows; i++) {
            char dump[MAXD + 2];
            char *row = (i < MAXD) ? g[i] : dump;
            if (scanf("%63s", row) != 1) row[0] = '\0';
        }
        if (rows > MAXD) rows = MAXD;
        if (cols > MAXD) cols = MAXD;
        m = rows; n = cols;

        for (i = 0; i < m; i++) {
            bool ended = false;
            for (j = 0; j < n; j++) {
                char c = ended ? '\0' : g[i][j];
                if (c == '\0') { ended = true; g[i][j] = 'X'; continue; }
                if (c >= 'a' && c <= 'e') total[c - 'a']++;
                if (c == 'S') { sx = i; sy = j; }
            }
        }

        if (sx >= 0) {
            bool changed = true;
            while (changed) {
                changed = false;
                if (flood(sx, sy)) { ans = true; break; }
                for (i = 0; i < 5; i++) {
                    if (!opened[i] && total[i] > 0 && found_[i] == total[i]) {
                        opened[i] = true;
                        changed = true;
                    }
                }
            }
        }

        printf(ans ? "YES\n" : "NO\n");
    }
    return 0;
}
