// POJ 3974 - Palindrome
//
// Longest palindromic substring of each input line, up to 1000000 lowercase
// characters, at most 30 lines, terminated by a line starting with "END".
//
// Manacher's algorithm, O(n) per line. The string is interleaved with '#'
// separators (t = #c0#c1#...#c(n-1)#, length 2n+1) so that even-length
// palindromes of s become odd-length palindromes of t and only one case has
// to be handled. p[i] is the largest k with t[i-k+1..i+k-1] palindromic; the
// palindrome it corresponds to in s has length k-1, so the answer is
// max(p[i]) - 1. Each i reuses its mirror's radius about the current
// rightmost-reaching palindrome, capped at that palindrome's right edge,
// which is what keeps the total expansion work linear.
//
// Notes on the statement:
//  - "print the test case number and the length" is the "Case k: len" format
//    shown in the sample; nothing else is printed.
//  - The terminator is a line that *starts with* "END", not a line equal to
//    it, so the check is on the prefix.
//  - Input is up to ~30 MB, so getchar() is too slow; a fread-backed buffer
//    is used. The 8 MB int array for p plus the 3 MB of char buffers fit the
//    65536K limit comfortably (accepted at 10968K).
#include <cstdio>
#include <cstring>

static const int MAXN = 1000005;
static char s[MAXN];
static char t[2 * MAXN];
static int p[2 * MAXN];

/* buffered reader: getchar() is far too slow for ~30 MB of input */
static const int BUFSZ = 1 << 16;
static char ibuf[BUFSZ];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* read one line into buf, return its length, or -1 at EOF */
static int readLine(char *buf) {
    int c = gc();
    if (c < 0) return -1;
    int n = 0;
    while (c >= 0 && c != '\n') {
        if (c != '\r') buf[n++] = (char)c;
        c = gc();
    }
    buf[n] = '\0';
    return n;
}

int main() {
    int caseNo = 0;
    int n;
    while ((n = readLine(s)) >= 0) {
        if (n >= 3 && s[0] == 'E' && s[1] == 'N' && s[2] == 'D') break;
        if (n == 0) continue;

        /* interleave separators so even- and odd-length palindromes are
           both odd-length in t: t = #c0#c1#...#c(n-1)# , |t| = 2n+1 */
        int m = 2 * n + 1;
        for (int i = 0; i < n; ++i) {
            t[2 * i] = '#';
            t[2 * i + 1] = s[i];
        }
        t[m - 1] = '#';

        /* Manacher: p[i] = largest k with t[i-k+1..i+k-1] a palindrome;
           the corresponding palindrome in s has length k-1. */
        int center = 0, right = -1, best = 0;
        for (int i = 0; i < m; ++i) {
            int k = 1;
            if (i <= right) {
                int mir = 2 * center - i;
                k = p[mir];
                if (k > right - i + 1) k = right - i + 1;
            }
            while (i - k >= 0 && i + k < m && t[i - k] == t[i + k]) ++k;
            p[i] = k;
            if (i + k - 1 > right) { right = i + k - 1; center = i; }
            if (k - 1 > best) best = k - 1;
        }
        printf("Case %d: %d\n", ++caseNo, best);
    }
    return 0;
}
