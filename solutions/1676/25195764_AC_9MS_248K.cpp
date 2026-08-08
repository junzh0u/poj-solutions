// POJ 1676 - What time is it?
// Model: claude-opus-5
//
// Two seven-segment clocks share one 3x25 character grid: the accurate time in
// columns 0..11, a blank separator column 12, the same clock 15 minutes late in
// columns 13..24.  Segments may be *missing*, never spurious, so an observed
// cell is legal iff it is blank or equals the true cell.  Only 1440 times exist,
// so enumerate them all: for each, the late clock is (t - 15 + 1440) % 1440 --
// the wrap through midnight is real (00:00 pairs with 23:45).  Print the time
// only when exactly one candidate survives, else "Not Sure" (that exact case).
//
// Ambiguity: a fully lit display is *not* self-identifying, because a lit digit
// is also a subset of any digit containing it (a lit '1' fits 0,1,3,4,7,8,9);
// only 239 of the 1440 fully lit displays pin their time down.  The statement
// never says what to print when *no* time fits; "Not Sure" is used, and the
// case cannot occur in the judge data (two accepted programs on the discuss
// board disagree on it -- one prints 0000, one prints uninitialised memory).
//
// Rows are read with fgets and padded to 25 columns, so stripped trailing
// blanks and CRLF endings are harmless; the leading case count is honoured
// exactly rather than reading to EOF.

#include <cstdio>
#include <cstring>

static const char *FONT[10][3] = {
    {" _ ", "| |", "|_|"},
    {"   ", "  |", "  |"},
    {" _ ", " _|", "|_ "},
    {" _ ", " _|", " _|"},
    {"   ", "|_|", "  |"},
    {" _ ", "|_ ", " _|"},
    {" _ ", "|_ ", "|_|"},
    {" _ ", "  |", "  |"},
    {" _ ", "|_|", "|_|"},
    {" _ ", "|_|", " _|"}
};

/* grid[r][c], r in 0..2, c in 0..24; clock A at cols 0..11, blank col 12,
   clock B at cols 13..24. */
static char grid[3][64];

static bool fits(int digit, int col)
{
    for (int r = 0; r < 3; ++r)
        for (int k = 0; k < 3; ++k) {
            char o = grid[r][col + k];
            if (o != ' ' && o != FONT[digit][r][k]) return false;
        }
    return true;
}

static bool showsTime(int minutes, int base)
{
    int h = minutes / 60, m = minutes % 60;
    int d[4];
    d[0] = h / 10; d[1] = h % 10; d[2] = m / 10; d[3] = m % 10;
    for (int i = 0; i < 4; ++i)
        if (!fits(d[i], base + 3 * i)) return false;
    return true;
}

static void readRow(char *dst)
{
    char buf[512];
    buf[0] = '\0';
    if (!fgets(buf, sizeof(buf), stdin)) buf[0] = '\0';
    int n = (int)strlen(buf);
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) buf[--n] = '\0';
    for (int i = 0; i < 25; ++i) dst[i] = (i < n) ? buf[i] : ' ';
    dst[25] = '\0';
}

int main()
{
    char line[512];
    if (!fgets(line, sizeof(line), stdin)) return 0;
    int t = 0;
    sscanf(line, "%d", &t);
    while (t-- > 0) {
        for (int r = 0; r < 3; ++r) readRow(grid[r]);
        int count = 0, answer = -1;
        for (int now = 0; now < 1440; ++now) {
            int late = (now - 15 + 1440) % 1440;
            if (showsTime(now, 0) && showsTime(late, 13)) {
                ++count;
                answer = now;
                if (count > 1) break;
            }
        }
        if (count == 1)
            printf("%02d%02d\n", answer / 60, answer % 60);
        else
            printf("Not Sure\n");
    }
    return 0;
}
