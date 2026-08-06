// POJ 2559 - Largest Rectangle in a Histogram
//
// Given n bars of width 1 and heights h[0..n-1] sharing a base line, find the
// largest axis-aligned rectangle that fits inside the histogram. Several test
// cases, each starting with n; a line holding just "0" ends the input.
//
// Approach: the classic monotonic-stack sweep, O(n) per case. Keep a stack of
// bars with strictly increasing heights; alongside each bar store left_[i],
// the leftmost column the bar can still extend to. Arriving at bar i, pop
// every stacked bar at least as tall as h[i]: such a bar can reach no further
// right than i-1, so its maximal rectangle is h[j] * (i - left_[j]). Each pop
// hands its own left_ down to the bar being pushed, which is how a short bar
// inherits the span of the taller ones it just cut off. After the sweep the
// bars still on the stack reach the right end, so they are flushed with width
// n - left_[j]. Every bar is pushed and popped once, so the whole case is
// linear and no explicit sentinel bar is needed.
//
// Notes on the statement:
//  - n <= 100000 and h <= 1e9, so the answer reaches 1e14 and must be 64-bit;
//    32-bit arithmetic silently wraps here.
//  - Heights may be 0, which simply makes the answer 0 for an all-zero case.
//    A 0 height inside a case is *not* a terminator: only a case count of 0 is.
//  - "Huge input, scanf is recommended" — input runs to megabytes, so this
//    reads through an fread buffer rather than scanf.
//  - The result is printed digit by digit instead of with printf("%lld"):
//    POJ's "C++" (language 4) is the MSVC-flavoured compiler, where %lld is
//    not portable (%I64d is its spelling), and hand-printing sidesteps it.

#include <cstdio>

static const int MAXN = 100005;
static const int BUF = 1 << 16;

static char ibuf[BUF];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUF, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

/* Reads a non-negative integer; returns false at end of input. */
static bool readInt(long long &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    long long v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10 + (c - '0');
        c = gc();
    }
    out = v;
    return true;
}

/* Print a non-negative long long by hand: POJ's "C++" compiler is the
   MSVC-flavoured one, where printf("%lld") is not reliable (%I64d there),
   so avoid the format specifier altogether. */
static void writeLL(long long v) {
    char tmp[24];
    int k = 0;
    if (v == 0) tmp[k++] = '0';
    while (v > 0) {
        tmp[k++] = (char)('0' + (int)(v % 10));
        v /= 10;
    }
    while (k > 0) putchar(tmp[--k]);
    putchar('\n');
}

static long long h[MAXN];
static int stk[MAXN];   /* indices, heights strictly increasing */
static int left_[MAXN]; /* first index of the run this bar can span left to */

int main() {
    long long n;
    while (readInt(n) && n > 0) {
        int i, top = 0;
        for (i = 0; i < (int)n; ++i) readInt(h[i]);

        long long best = 0;
        for (i = 0; i < (int)n; ++i) {
            int start = i;
            while (top > 0 && h[stk[top - 1]] >= h[i]) {
                --top;
                long long width = (long long)(i - left_[stk[top]]);
                long long area = h[stk[top]] * width;
                if (area > best) best = area;
                start = left_[stk[top]];
            }
            stk[top] = i;
            left_[i] = start;
            ++top;
        }
        while (top > 0) {
            --top;
            long long width = (long long)((int)n - left_[stk[top]]);
            long long area = h[stk[top]] * width;
            if (area > best) best = area;
        }
        writeLL(best);
    }
    return 0;
}
