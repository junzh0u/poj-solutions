// POJ 3693 - Maximum repetition substring
// Model: claude-sonnet-5
// Approach: two suffix arrays (forward, on s, and backward, on reverse(s)),
// each with its own height array + sparse table for O(1)/O(log n) LCP
// queries, plus a third sparse table doing a range-argmin over the forward
// suffix array's rank (indexed by ORIGINAL position) so "which position in
// [l, r] starts the lexicographically smallest suffix" is also O(1)/O(log n).
//
// For every period length d, walk checkpoints i = 0, d, 2d, ... (i+d<n).
// Lf = LCP(suffix_s(i), suffix_s(i+d)) extends the periodic block forward
// from i; Lb = the longest match going backward from position i (comparing
// s[i-1] vs s[i+d-1], s[i-2] vs s[i+d-2], ...), obtained as an LCP query on
// the reversed string, extends it backward. Together [i-Lb, i+d+Lf-1] is the
// TRUE maximal period-d run containing checkpoint i (independent of which
// checkpoint inside the run we happened to probe from), so every checkpoint
// inside one run recomputes the same run redundantly -- harmless, just
// wasted work bounded by the O(n log n) total checkpoint count. R = floor of
// the run length over d is the true best repetition count for this run, and
// every start position in [p, p+rem] (rem = run length mod d) yields a
// distinct but equally maximal substring: the range-argmin sparse table
// picks the lexicographically smallest one among them directly, which is
// necessary -- restricting to just the checkpoint itself, or to a single
// hand-adjusted position, provably misses valid smaller-lexicographic phases
// (confirmed by hand against "baccdbaccdbacbdbacbd", where the true winning
// phase starts strictly inside the run, at neither the checkpoint nor either
// endpoint of its reachable range).
//
// A separate baseline candidate (the single smallest character anywhere in
// the string) covers strings with no internal repetition at all and n==1,
// since the checkpoint scan requires i+d<n and can never place the very
// last character as a length-1 start on its own.
//
// No statement ambiguity beyond the usual "repetition number of the chosen
// substring itself" reading (confirmed against the discuss board).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

int n;
char s[MAXN], t[MAXN];

// ---- generic suffix array machinery (used twice: forward and reversed) ----
struct SuffixArray {
    int sa[MAXN], rnk[MAXN], height[MAXN];
    int tmp_rnk[MAXN], sa_tmp[MAXN], cnt[MAXN];
    int LOG[MAXN];
    int sparse[18][MAXN];
    int len;

    void build(const char *str, int n_) {
        len = n_;
        int m = 256;
        int *x = tmp_rnk, *y = sa_tmp;
        for (int i = 0; i < m; i++) cnt[i] = 0;
        for (int i = 0; i < len; i++) cnt[x[i] = (unsigned char)str[i]]++;
        for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];
        for (int i = len-1; i >= 0; i--) sa[--cnt[x[i]]] = i;

        for (int k = 1; k <= len; k <<= 1) {
            int p = 0;
            for (int i = len - k; i < len; i++) y[p++] = i;
            for (int i = 0; i < len; i++) if (sa[i] >= k) y[p++] = sa[i] - k;

            for (int i = 0; i < m; i++) cnt[i] = 0;
            for (int i = 0; i < len; i++) cnt[x[y[i]]]++;
            for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];
            for (int i = len-1; i >= 0; i--) sa[--cnt[x[y[i]]]] = y[i];

            swap(x, y);
            x[sa[0]] = 0;
            p = 1;
            for (int i = 1; i < len; i++) {
                int a = sa[i-1], b = sa[i];
                bool same = (y[a] == y[b]) &&
                            ((a + k < len ? y[a+k] : -1) == (b + k < len ? y[b+k] : -1));
                x[sa[i]] = same ? p - 1 : p++;
            }
            if (p >= len) { for (int i = 0; i < len; i++) rnk[i] = x[i]; break; }
            m = p;
            for (int i = 0; i < len; i++) rnk[i] = x[i];
        }

        int k = 0;
        height[0] = 0;
        for (int i = 0; i < len; i++) {
            if (rnk[i] == 0) { k = 0; continue; }
            if (k > 0) k--;
            int j = sa[rnk[i] - 1];
            while (i + k < len && j + k < len && str[i+k] == str[j+k]) k++;
            height[rnk[i]] = k;
        }

        LOG[1] = 0;
        for (int i = 2; i <= len; i++) LOG[i] = LOG[i/2] + 1;
        for (int i = 0; i < len; i++) sparse[0][i] = height[i];
        for (int kk = 1; (1 << kk) <= len; kk++) {
            for (int i = 0; i + (1 << kk) <= len; i++) {
                sparse[kk][i] = min(sparse[kk-1][i], sparse[kk-1][i + (1 << (kk-1))]);
            }
        }
    }

    int rmq(int l, int r) const { // inclusive over height indices
        if (l > r) return 1 << 30;
        int k = LOG[r - l + 1];
        return min(sparse[k][l], sparse[k][r - (1 << k) + 1]);
    }

    int lcp(int x, int y) const {
        if (x == y) return len - x;
        int rx = rnk[x], ry = rnk[y];
        if (rx > ry) swap(rx, ry);
        return rmq(rx + 1, ry);
    }
};

SuffixArray F; // forward, over s
SuffixArray B; // backward, over reversed s

// backward match length: how many of s[i-1],s[i-2],... equal s[j-1],s[j-2],...
int backLCP(int i, int j) {
    if (i == 0 || j == 0) return 0;
    return B.lcp(n - i, n - j);
}

// range-argmin over F.rnk[], indexed by original position, for the "which
// start in [l,r] gives the lexicographically smallest suffix" query.
int rmin[18][MAXN];
int RLOG[MAXN];

void buildRangeArgmin() {
    RLOG[1] = 0;
    for (int i = 2; i <= n; i++) RLOG[i] = RLOG[i/2] + 1;
    for (int i = 0; i < n; i++) rmin[0][i] = i;
    for (int k = 1; (1 << k) <= n; k++) {
        for (int i = 0; i + (1 << k) <= n; i++) {
            int a = rmin[k-1][i], b = rmin[k-1][i + (1 << (k-1))];
            rmin[k][i] = (F.rnk[a] <= F.rnk[b]) ? a : b;
        }
    }
}

int queryArgmin(int l, int r) { // inclusive, over positions [l,r]
    int k = RLOG[r - l + 1];
    int a = rmin[k][l], b = rmin[k][r - (1 << k) + 1];
    return (F.rnk[a] <= F.rnk[b]) ? a : b;
}

// Compare candidate substrings (start1,len1) vs (start2,len2) lexicographically.
// Needed because candidates achieving the same R can come from different d,
// hence different lengths -- a plain rank comparison mishandles the case
// where one is an exact prefix of the other (shorter wins there).
int cmpSub(int start1, int len1, int start2, int len2) {
    if (start1 == start2 && len1 == len2) return 0;
    int L = F.lcp(start1, start2);
    int minlen = min(len1, len2);
    if (L >= minlen) {
        if (len1 != len2) return len1 < len2 ? -1 : 1;
        return 0;
    }
    return F.rnk[start1] < F.rnk[start2] ? -1 : 1;
}

int main() {
    while (scanf("%s", s) == 1) {
        if (s[0] == '#' && s[1] == '\0') break;
        static int caseNo = 0;
        caseNo++;
        n = strlen(s);

        int bestR = 0, bestStart = 0, bestLen = 0;

        if (n == 1) {
            bestR = 1; bestStart = 0; bestLen = 1;
        } else {
            for (int i = 0; i < n; i++) t[i] = s[n - 1 - i];
            F.build(s, n);
            B.build(t, n);
            buildRangeArgmin();

            // baseline: smallest single character anywhere in the string
            int minc = 0;
            for (int i = 1; i < n; i++) if (s[i] < s[minc]) minc = i;
            bestR = 1; bestStart = minc; bestLen = 1;

            for (int d = 1; d <= n; d++) {
                for (int i = 0; i + d < n; i += d) {
                    int Lf = F.lcp(i, i + d);
                    int Lb = backLCP(i, i + d);
                    int p = i - Lb;
                    int Z = Lb + d + Lf; // bounded by ~3n, fits comfortably in int
                    int R = Z / d;
                    int rem = Z % d;
                    if (R < 1) continue;
                    int lo = p, hi = p + rem;
                    int cand = queryArgmin(lo, hi);
                    int len = R * d;
                    if (R > bestR) {
                        bestR = R; bestStart = cand; bestLen = len;
                    } else if (R == bestR && cmpSub(cand, len, bestStart, bestLen) < 0) {
                        bestStart = cand; bestLen = len;
                    }
                }
            }
        }

        printf("Case %d: ", caseNo);
        fwrite(s + bestStart, 1, bestLen, stdout);
        putchar('\n');
    }
    return 0;
}
