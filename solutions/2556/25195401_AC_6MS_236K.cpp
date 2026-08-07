// POJ 2556 - Edge
// Model: claude-opus-5
//
// Direct simulation.  The pen starts at (300,420) heading in +x and the first
// turning point is (310,420).  Each character of the input string is a 90
// degree turn taken at the current point: 'A' is a clockwise turn, 'V' is a
// counter-clockwise turn.  After turning, advance 10 units and emit the new
// point as an "x y lineto" line.  Finish each case with "stroke"/"showpage".
//
// Directions are indexed 0..3 as (+x, +y, -x, -y) in PostScript axes (+y is
// up), so counter-clockwise is dir = (dir + 1) % 4 and clockwise is
// dir = (dir + 3) % 4.  Keeping an explicit direction state is the whole
// trick: the discuss board's two posted Wrong Answer programs both try to
// recover the heading from the current coordinates (testing n > 420 against
// n < 420), which is not a function of position at all.  The official sample
// is blind to that bug -- both posted programs reproduce it exactly -- but a
// randomized differential test against an independent reference kills it on
// nearly every case.  The sample does pin the A/V orientation, since it is not
// symmetric under swapping the two.
//
// Input holds several test cases, one string per line, read to EOF.  Reading
// with >> skips the line separators (and any stray '\r'), which is what the
// board's "the input contains characters other than A and V" thread is really
// about; any other stray character is skipped defensively.
//
// Coordinates stay small (|delta| <= 10 * 199 from 300/420), so plain int is
// ample.  Output is accumulated in a buffer and flushed with fwrite rather
// than emitted with one printf per line, since a case may produce up to 200
// lines and the number of cases is not bounded by the statement.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

static char out[1 << 16];
static size_t olen = 0;

static void flushOut() {
    if (olen) {
        fwrite(out, 1, olen, stdout);
        olen = 0;
    }
}

static void emit(const char *s, size_t n) {
    if (olen + n > sizeof(out)) flushOut();
    memcpy(out + olen, s, n);
    olen += n;
}

// Appends "<x> <y> lineto\n".
static void emitPoint(int x, int y) {
    char buf[40];
    int p = 0;
    for (int k = 0; k < 2; ++k) {
        int v = (k == 0) ? x : y;
        if (v < 0) {
            buf[p++] = '-';
            v = -v;
        }
        char d[12];
        int n = 0;
        do {
            d[n++] = (char)('0' + v % 10);
            v /= 10;
        } while (v);
        while (n) buf[p++] = d[--n];
        buf[p++] = ' ';
    }
    memcpy(buf + p, "lineto\n", 7);
    p += 7;
    emit(buf, (size_t)p);
}

int main() {
    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};

    std::string s;
    while (std::cin >> s) {
        int x = 310, y = 420, dir = 0;
        emit("300 420 moveto\n310 420 lineto\n", 30);
        for (std::string::size_type i = 0; i < s.size(); ++i) {
            if (s[i] == 'V') {
                dir = (dir + 1) & 3;
            } else if (s[i] == 'A') {
                dir = (dir + 3) & 3;
            } else {
                continue;
            }
            x += 10 * dx[dir];
            y += 10 * dy[dir];
            emitPoint(x, y);
        }
        emit("stroke\nshowpage\n", 16);
    }
    flushOut();
    return 0;
}
