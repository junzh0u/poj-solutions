// POJ 2273 - An Excel-lent Problem
// Model: claude-opus-5
//
// Convert (row, column) into a spreadsheet cell designation: the column index
// is written in *bijective* base 26 (digits 'A'..'Z' standing for 1..26, no
// zero digit), the row index is appended verbatim in decimal.
//   while (m) { --m; digit = m % 26; m /= 26; }   emits digits least
// significant first; reverse them.  m <= 300000000 < 26^6 = 308915776, so at
// most 6 letters and plain int suffices everywhere.
//
// Input framing: the statement claims "There will be no leading zeroes or
// extra spaces in the input", but the discuss board (message 139049,
// "坐标有前导0") reports that the real data does contain leading zeroes.
// scanf(" R%dC%d") absorbs both leading zeroes and any whitespace/CR, and the
// terminator is detected on the parsed *values* being 0 rather than by
// comparing the line against the literal string "R0C0" -- a string compare is
// exactly what leading zeroes break.  The row is echoed as the parsed integer
// (leading zeroes dropped), which is what the board's Accepted %d-based
// solution does.

#include <cstdio>

int main() {
    int n, m;
    while (scanf(" R%dC%d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        char buf[16];
        int len = 0;
        int c = m;
        while (c > 0) {
            --c;
            buf[len++] = (char)('A' + c % 26);
            c /= 26;
        }
        while (len > 0) putchar(buf[--len]);
        printf("%d\n", n);
    }
    return 0;
}
