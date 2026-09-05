// POJ 3058 - Decompression
// Model: claude-sonnet-5
// Approach: The compressed string is a run-length encoding of the
// Burrows-Wheeler-like "last column" L of the original string T (T is
// uppercase letters followed by a single '.' sentinel, which is unique and
// lexicographically smallest). Decompress the RLE to recover L. Then invert
// the BWT with the standard LF-mapping: counting-sort L to get, for each
// symbol, the start offset of its block in the sorted first column F; scan L
// left to right assigning LF[i] = (running count of L[i] so far) + start
// offset, which is exactly the row that character maps to in F. Since F[0]
// is the unique '.', start at r = 0 (ans[n-1] = '.') and repeatedly set
// ans[i] = L[r]; r = LF[r] walking i from n-2 down to 0. This is O(n) per
// test case with only 27 symbols, well within the 1e6-length bound.
// No statement ambiguity found; verified against sample and a differential
// brute force (build BWT directly from rotations, RLE-encode, decompress,
// compare to the original).
#include <cstdio>
#include <cstring>

const int MAXN = 1000005;
static char comp[MAXN];
static unsigned char L[MAXN];
static int LF[MAXN];
static char ans[MAXN];

inline int symIndex(unsigned char c) {
    if (c == '.') return 0;
    return c - 'A' + 1;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        scanf("%s", comp);
        int clen = (int)strlen(comp);
        int n = 0;
        int i = 0;
        while (i < clen) {
            char c = comp[i++];
            int cnt = 0;
            bool hasDigit = false;
            while (i < clen && comp[i] >= '0' && comp[i] <= '9') {
                hasDigit = true;
                cnt = cnt * 10 + (comp[i] - '0');
                i++;
            }
            if (!hasDigit) cnt = 1;
            for (int k = 0; k < cnt; k++) {
                L[n++] = (unsigned char)c;
            }
        }

        int count[27];
        memset(count, 0, sizeof(count));
        for (int j = 0; j < n; j++) count[symIndex(L[j])]++;
        int start[27];
        start[0] = 0;
        for (int s = 1; s < 27; s++) start[s] = start[s - 1] + count[s - 1];
        int cur[27];
        memcpy(cur, start, sizeof(start));
        for (int j = 0; j < n; j++) {
            int s = symIndex(L[j]);
            LF[j] = cur[s]++;
        }

        ans[n - 1] = '.';
        int r = 0;
        for (int j = n - 2; j >= 0; j--) {
            ans[j] = (char)L[r];
            r = LF[r];
        }
        ans[n] = '\0';

        fwrite(ans, 1, n, stdout);
        fputc('\n', stdout);
    }
    return 0;
}
