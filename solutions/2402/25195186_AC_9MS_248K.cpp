// POJ 2402 - Palindrome Numbers
// Model: claude-opus-5
//
// Print the i-th palindrome number (no leading zero allowed), 1 <= i <= 2*10^9,
// for each input line until a line holding 0.
//
// Approach: a d-digit palindrome is determined by its leading h = ceil(d/2)
// digits, whose first digit must be nonzero, so there are exactly 9*10^(h-1)
// of them.  Accumulate those counts to find the digit length d that the index
// falls in, then the rank j inside that block gives the free half directly as
// 10^(h-1) + j; mirroring the half's first floor(d/2) digits completes the
// number.  O(1) per query after a 19-entry table.
//
// Sizing: cum[18] = 1999999998 < 2^31-1 and cum[19] already exceeds the largest
// legal index, so a sentinel at d = 19 keeps every intermediate value in 32-bit
// int -- deliberately, since POJ's compilers are ancient and its runtime is the
// classic %lld trap.  Nothing here is wider than int, and the answer (up to 19
// digits, e.g. i = 2*10^9 -> 1000000001000000001) is emitted as text, never as
// a 64-bit integer.
//
// No statement ambiguity: index 1 is the palindrome 1, and 10 is not a
// palindrome because the leading zero of "010" is not allowed.
//
// Verified: exhaustive agreement with a from-the-statement brute force
// (enumerate 1,2,3,... keeping the ones that read the same backwards) over
// indices 1..19998, i.e. every palindrome below 10^8; and for 20000+ larger
// indices -- every digit-length boundary plus randoms up to 2*10^9 -- an
// independent counting function certifies that the emitted value is a
// palindrome whose rank is exactly the requested index.

#include <cstdio>

int main() {
    // cnt(d) = # of d-digit palindromes = 9 * 10^(ceil(d/2)-1)
    // cum[d] = # of palindromes with at most d digits
    int cum[20];
    cum[0] = 0;
    for (int d = 1; d <= 18; ++d) {
        int h = (d + 1) / 2;
        int c = 9;
        for (int k = 1; k < h; ++k) c *= 10;
        cum[d] = cum[d - 1] + c;
    }
    cum[19] = 2147483647;               // sentinel, see header

    int i;
    while (scanf("%d", &i) == 1 && i != 0) {
        int d = 1;
        while (cum[d] < i) ++d;
        int j = i - cum[d - 1] - 1;     // 0-based rank among the d-digit ones
        int h = (d + 1) / 2;            // length of the free half
        int half = 1;
        for (int k = 1; k < h; ++k) half *= 10;
        half += j;                      // h digits, leading digit nonzero

        char buf[32];
        char tmp[32];
        int len = 0;
        int t = 0;
        for (int v = half; v > 0; v /= 10) tmp[t++] = (char)('0' + v % 10);
        while (t > 0) buf[len++] = tmp[--t];
        for (int k = d - h - 1; k >= 0; --k) buf[len++] = buf[k];
        buf[len] = '\0';
        puts(buf);
    }
    return 0;
}
