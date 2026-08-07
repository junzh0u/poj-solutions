// POJ 1120 - A New Growth Industry
// Model: claude-opus-5
//
// Direct simulation of a 20x20 cellular automaton.  For each square let K be
// the sum of its own density and those of its four orthogonal neighbours
// (squares outside the dish count as 0, so 0 <= K <= 15); the next day's
// density is clamp(old + D[K], 0, 3).
//
// Two things the plain prose does not pin down:
//
//  * Update order.  "by the next day" is simultaneous: every square reads
//    yesterday's grid.  The official sample separates the two readings -- an
//    in-place sequential update gets a different day-2 grid -- so this one is
//    settled by the sample rather than assumed.
//  * The density-to-character table lives only in images/1120/1120_1.gif and
//    is NOT monotone-looking: 0='.', 1='!', 2='X', 3='#'.  Swapping 'X' and
//    '#' is the natural guess from the prose alone and is wrong (the sample
//    catches it, but the figure is the authority).
//
// Input framing per the discuss board: POJ feeds exactly ONE data set (unlike
// the ZOJ version of this problem), so no read-to-EOF loop -- looping would
// print more than the required 20 lines.  Output is 20 lines of exactly 20
// characters with no padding; stray spaces draw a Presentation Error.
//
// The day count is unbounded by the statement.  A ring buffer of the last 16
// grids detects any cycle of period <= 16 and skips ahead by whole periods,
// so a pathological day count costs nothing; without it 10^6 days would still
// run in well under the limit anyway.

#include <cstdio>
#include <cstring>

static const int N = 20;
static const char SYM[4] = {'.', '!', 'X', '#'};

int main() {
    int days;
    if (scanf("%d", &days) != 1) return 0;

    int D[16];
    for (int i = 0; i < 16; ++i)
        if (scanf("%d", &D[i]) != 1) return 0;

    /* one-cell border of zeros so out-of-dish squares read as density 0 */
    unsigned char cur[N + 2][N + 2], nxt[N + 2][N + 2];
    memset(cur, 0, sizeof(cur));
    memset(nxt, 0, sizeof(nxt));
    for (int r = 1; r <= N; ++r)
        for (int c = 1; c <= N; ++c) {
            int v;
            if (scanf("%d", &v) != 1) return 0;
            cur[r][c] = (unsigned char)v;
        }

    static const int RING = 16;
    unsigned char hist[RING][N + 2][N + 2];
    int histDay[RING];
    for (int i = 0; i < RING; ++i) histDay[i] = -1;

    int day = 0;
    while (day < days) {
        int slot = day % RING;
        memcpy(hist[slot], cur, sizeof(cur));
        histDay[slot] = day;

        for (int r = 1; r <= N; ++r)
            for (int c = 1; c <= N; ++c) {
                int k = cur[r][c] + cur[r - 1][c] + cur[r + 1][c]
                        + cur[r][c - 1] + cur[r][c + 1];
                int v = (int)cur[r][c] + D[k];
                if (v > 3) v = 3;
                if (v < 0) v = 0;
                nxt[r][c] = (unsigned char)v;
            }
        memcpy(cur, nxt, sizeof(cur));
        ++day;

        for (int i = 0; i < RING; ++i) {
            if (histDay[i] < 0) continue;
            if (memcmp(hist[i], cur, sizeof(cur)) == 0) {
                int period = day - histDay[i];
                int left = (days - day) % period;
                day = days - left;
                for (int j = 0; j < RING; ++j) histDay[j] = -1;
                break;
            }
        }
    }

    for (int r = 1; r <= N; ++r) {
        char line[N + 1];
        for (int c = 1; c <= N; ++c) line[c - 1] = SYM[cur[r][c]];
        line[N] = '\0';
        puts(line);
    }
    return 0;
}
