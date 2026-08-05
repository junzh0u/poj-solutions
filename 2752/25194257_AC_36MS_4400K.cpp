// POJ 2752 - Seek the Name, Seek the Fame
//
// For each input string S, print every length L (1 <= L <= |S|) such that the
// prefix of length L equals the suffix of length L -- i.e. every border of S,
// including the whole string itself -- in increasing order on one line.
//
// The KMP failure function gives all of them for free: fail[i] is the length of
// the longest proper border of S[1..i], so the borders of S are exactly the
// chain n, fail[n], fail[fail[n]], ... down to 0. Building fail is O(n) and the
// chain is walked once, so the whole answer is O(n) per test case.
//
// Notes on the statement:
//  - "prefix-suffix string" includes S itself (the sample confirms: 'alala'
//    lists 'alala', and 'ababcababababcabab' ends with 18), but not the empty
//    string, so the chain is walked while len > 0.
//  - The number of test cases is unbounded; read with scanf("%s") until EOF.
//  - The worst case, a string of 400000 identical letters, has 400000 answers
//    on a single line (~2.7 MB of digits). Formatting them with printf one at a
//    time is what makes the obvious solution slow, so the line is rendered into
//    one static buffer and written with a single fwrite.
//
#include <cstdio>
#include <cstring>

const int MAXN = 400005;

char s[MAXN];
int fail[MAXN];
int stk[MAXN];
char out[MAXN * 8];

int main() {
    while (scanf("%s", s + 1) == 1) {
        int n = (int)strlen(s + 1);
        fail[1] = 0;
        int k = 0;
        for (int i = 2; i <= n; ++i) {
            while (k > 0 && s[k + 1] != s[i]) k = fail[k];
            if (s[k + 1] == s[i]) ++k;
            fail[i] = k;
        }
        int top = 0;
        for (int len = n; len > 0; len = fail[len]) stk[top++] = len;
        int p = 0;
        for (int i = top - 1; i >= 0; --i) {
            if (i != top - 1) out[p++] = ' ';
            int v = stk[i];
            char tmp[12];
            int t = 0;
            while (v > 0) { tmp[t++] = (char)('0' + v % 10); v /= 10; }
            while (t > 0) out[p++] = tmp[--t];
        }
        out[p++] = '\n';
        fwrite(out, 1, (size_t)p, stdout);
    }
    return 0;
}
