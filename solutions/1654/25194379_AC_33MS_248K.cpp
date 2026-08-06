// POJ 1654 - Area
//
// The walk is a closed simple lattice polygon: each digit is a keypad
// direction (8/2/6/4 = N/S/E/W, 9/7/3/1 = NE/NW/SE/SW, 5 = stop), so every
// vertex lands on an integer lattice point. The area is therefore the
// shoelace sum over the vertices, halved.
//
// Keeping twice the area as an exact integer is what makes this clean:
// 2A = sum of (x_i * y_{i+1} - x_{i+1} * y_i) is always an integer, so the
// answer is either an integer or an integer plus exactly .5 - print
// |2A| / 2 and append ".5" when |2A| is odd, never touching floating point
// (a double would lose the last digits well before 1e12).
//
// The shoelace term collapses: with (nx, ny) = (x + dx, y + dy),
//   x*ny - nx*y = x*(y+dy) - (x+dx)*y = x*dy - y*dx
// so each step costs a couple of adds, no multiplication of large values.
// x and y stay within +-1e6 and fit in int; only the running sum needs
// 64 bits (it reaches ~2e12 for a path that spirals out to the limit,
// far past what 32 bits holds).
//
// A line can be 1000000 digits and there are up to 20 of them - 20 MB of
// input against a 10000K memory limit - so the digits are consumed through
// a 64K fread buffer and never stored. The output is written digit by digit
// rather than with %lld, whose support on POJ's compilers is unreliable.
//
// Statement notes: the sign of the shoelace sum depends on whether the walk
// runs clockwise or counter-clockwise, so take the absolute value. A path
// that encloses nothing - the bare "5", or a straight out-and-back like
// "645" - has 2A = 0 and prints "0", not "0.0"; the sample's first two
// lines pin that down. Diagonal steps move both coordinates by one, so a
// diagonal side spans a full lattice cell, not half of one.
#include <cstdio>

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* dx/dy indexed by digit 1..9, laid out like a numeric keypad */
static const int DX[10] = {0, -1, 0, 1, -1, 0, 1, -1, 0, 1};
static const int DY[10] = {0, -1, -1, -1, 0, 0, 0, 1, 1, 1};

static void putull(unsigned long long v) {
    char tmp[24];
    int n = 0;
    if (v == 0) {
        putchar('0');
        return;
    }
    while (v > 0) {
        tmp[n++] = (char)('0' + (int)(v % 10ULL));
        v /= 10ULL;
    }
    while (n > 0) putchar(tmp[--n]);
}

int main() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    long long t = 0;
    while (c >= '0' && c <= '9') {
        t = t * 10 + (c - '0');
        c = gc();
    }

    for (long long k = 0; k < t; ++k) {
        /* skip to the start of the next digit run */
        while (c != -1 && (c < '1' || c > '9')) c = gc();

        int x = 0, y = 0;
        long long s = 0;
        while (c >= '1' && c <= '9') {
            int d = c - '0';
            int dx = DX[d], dy = DY[d];
            /* shoelace term x*ny - nx*y collapses to x*dy - y*dx */
            s += x * dy - y * dx;
            x += dx;
            y += dy;
            c = gc();
        }

        unsigned long long a2 = (unsigned long long)(s < 0 ? -s : s);
        putull(a2 / 2ULL);
        if (a2 & 1ULL) {
            putchar('.');
            putchar('5');
        }
        putchar('\n');
    }
    return 0;
}
