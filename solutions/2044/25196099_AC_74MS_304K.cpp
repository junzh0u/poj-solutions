// POJ 2044 - Weather Forecast
// Model: claude-fable-5
//
// A 2x2 cloud moves on a 4x4 grid: day 1 it is forced onto the four central
// areas (and, as sample 1 shows, a market scheduled there that day makes the
// answer 0); on each later day it moves 0, 1 or 2 squares in one cardinal
// direction. Rain must never fall on a market/festival cell, and no cell may
// stay dry more than 6 consecutive days (it rains everywhere before and after
// the period).
//
// State reduction: each corner cell (0, 3, 12, 15 in 0-based numbering) is
// covered by exactly one cloud position (the matching corner position), and
// those four positions jointly cover all 16 cells; every non-corner cell is
// covered whenever its corner is, so its dry counter never exceeds the
// corner's. Hence it suffices to track (cloud position, dry counters of the
// four corners), 9 * 7^4 = 21609 states, swept forward one day at a time.
#include <cstdio>
#include <cstring>

static bool cur[9][7][7][7][7], nxt[9][7][7][7][7];
static int sch[365][16];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < 16; j++)
                scanf("%d", &sch[i][j]);
        memset(cur, 0, sizeof(cur));
        /* day 1: cloud at position (1,1) covering cells 5,6,9,10 (0-based) */
        bool any = !(sch[0][5] || sch[0][6] || sch[0][9] || sch[0][10]);
        if (any)
            cur[4][1][1][1][1] = true;
        static const int dr[9] = { 0, -1, -2, 1, 2, 0, 0, 0, 0 };
        static const int dc[9] = { 0, 0, 0, 0, 0, -1, -2, 1, 2 };
        for (int day = 1; day < n && any; day++) {
            any = false;
            memset(nxt, 0, sizeof(nxt));
            for (int p = 0; p < 9; p++) {
                int r = p / 3, c = p % 3;
                for (int a = 0; a < 7; a++)
                for (int b = 0; b < 7; b++)
                for (int e = 0; e < 7; e++)
                for (int d = 0; d < 7; d++) {
                    if (!cur[p][a][b][e][d])
                        continue;
                    for (int m = 0; m < 9; m++) {
                        int nr = r + dr[m], nc = c + dc[m];
                        if (nr < 0 || nr > 2 || nc < 0 || nc > 2)
                            continue;
                        int base = nr * 4 + nc;
                        if (sch[day][base] || sch[day][base + 1] ||
                            sch[day][base + 4] || sch[day][base + 5])
                            continue;
                        int np = nr * 3 + nc;
                        int na = (np == 0) ? 0 : a + 1;
                        int nb = (np == 2) ? 0 : b + 1;
                        int ne = (np == 6) ? 0 : e + 1;
                        int nd = (np == 8) ? 0 : d + 1;
                        if (na > 6 || nb > 6 || ne > 6 || nd > 6)
                            continue;
                        nxt[np][na][nb][ne][nd] = true;
                        any = true;
                    }
                }
            }
            memcpy(cur, nxt, sizeof(cur));
        }
        printf("%d\n", any ? 1 : 0);
    }
    return 0;
}
