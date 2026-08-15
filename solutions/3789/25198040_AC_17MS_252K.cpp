// POJ 3789 - Binary Clock
// Model: claude-sonnet-5
// Approach: For each of H, M, S compute their 6-bit binary representation
// (bit 5 = 32's place down to bit 0 = 1's place). Horizontal format is the
// straightforward concatenation of H's 6 bits, then M's 6 bits, then S's
// 6 bits (each MSB-first) -- this matches "read top to bottom, each row
// left to right". Vertical format instead concatenates by bit-position:
// for bit position from 5 down to 0, emit H's bit, M's bit, S's bit -- this
// matches "read top to bottom" over rows that each contain one bit of H,
// one of M, one of S (columns), read left to right within a row.
// Verified against both sample cases by hand before coding.
// No statement ambiguity beyond decoding the figure, which the two worked
// examples pin down unambiguously (both readings of vertical/horizontal
// were checked and only this pairing reproduces both sample lines).
#include <cstdio>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 1; tc <= n; ++tc) {
        int h, m, s;
        scanf("%d:%d:%d", &h, &m, &s);
        int bitsH[6], bitsM[6], bitsS[6];
        for (int b = 5; b >= 0; --b) {
            bitsH[b] = (h >> b) & 1;
            bitsM[b] = (m >> b) & 1;
            bitsS[b] = (s >> b) & 1;
        }
        char vert[19], horiz[19];
        int vi = 0;
        for (int b = 5; b >= 0; --b) {
            vert[vi++] = '0' + bitsH[b];
            vert[vi++] = '0' + bitsM[b];
            vert[vi++] = '0' + bitsS[b];
        }
        vert[vi] = '\0';
        int hi = 0;
        for (int b = 5; b >= 0; --b) horiz[hi++] = '0' + bitsH[b];
        for (int b = 5; b >= 0; --b) horiz[hi++] = '0' + bitsM[b];
        for (int b = 5; b >= 0; --b) horiz[hi++] = '0' + bitsS[b];
        horiz[hi] = '\0';
        printf("%d %s %s\n", tc, vert, horiz);
    }
    return 0;
}
