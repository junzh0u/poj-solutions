// POJ 3882 - Stammering Aliens
// Model: claude-sonnet-5
// Approach: suffix array (O(n log n) radix-sort doubling on cyclic shifts of
// s + a sentinel smaller than any letter) + Kasai's LCP array.
// Binary search on the answer length L (feasibility is monotone: if some
// substring of length L occurs >= m times, so does one of length L-1, by
// simply truncating). For a fixed L, the suffix array splits into maximal
// runs of consecutive suffixes whose adjacent LCP values are all >= L; each
// such run is exactly the set of suffixes sharing the same L-character
// prefix (a standard suffix-array fact), so a run of size >= m witnesses a
// substring occurring >= m times. Any run of size >= 2 automatically has
// every member of length >= L, because LCP is capped by the shorter of the
// two suffixes being compared, so the rightmost starting position (the max
// SA value inside the run) is always a genuine occurrence.
// Special case m == 1: flagged on the problem's discuss board (thread "pay
// attention to when m=1"). With m=1 every substring trivially occurs once
// (itself), so the unique longest one is the whole string, at position 0.
// The general run-scan would otherwise have to worry about a singleton run
// whose lone suffix is shorter than L (no neighboring LCP entry to bound
// it), so m=1 is special-cased directly instead of patched into the scan.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 40010;
char s[MAXN];
int n;                 // length of s (without sentinel)
int m;

int p_[MAXN + 1], c_[MAXN + 1], cnt_[MAXN + 1];
int pn_[MAXN + 1], cn_[MAXN + 1];
int sa[MAXN];           // suffix array of s (n entries), suffixes of s only
int rnk[MAXN];           // rank of each suffix start in sa
int lcpArr[MAXN];        // lcpArr[i] = LCP(sa[i-1], sa[i]) for i in [1, n-1]
unsigned char t_[MAXN + 1]; // s + sentinel, length n+1

void buildSuffixArray() {
    int total = n + 1; // s + sentinel
    t_[n] = 1; // sentinel value, smaller than any lowercase letter (>=97)
    for (int i = 0; i < n; i++) t_[i] = (unsigned char)s[i];

    int alphabet = 256;
    for (int i = 0; i < alphabet; i++) cnt_[i] = 0;
    for (int i = 0; i < total; i++) cnt_[t_[i]]++;
    for (int i = 1; i < alphabet; i++) cnt_[i] += cnt_[i - 1];
    for (int i = 0; i < total; i++) p_[--cnt_[t_[i]]] = i;
    c_[p_[0]] = 0;
    int classes = 1;
    for (int i = 1; i < total; i++) {
        if (t_[p_[i]] != t_[p_[i - 1]]) classes++;
        c_[p_[i]] = classes - 1;
    }
    for (int h = 0; (1 << h) < total; ++h) {
        int shift = 1 << h;
        for (int i = 0; i < total; i++) {
            int v = p_[i] - shift;
            if (v < 0) v += total;
            pn_[i] = v;
        }
        for (int i = 0; i < classes; i++) cnt_[i] = 0;
        for (int i = 0; i < total; i++) cnt_[c_[pn_[i]]]++;
        for (int i = 1; i < classes; i++) cnt_[i] += cnt_[i - 1];
        for (int i = total - 1; i >= 0; i--) p_[--cnt_[c_[pn_[i]]]] = pn_[i];
        cn_[p_[0]] = 0;
        classes = 1;
        for (int i = 1; i < total; i++) {
            int a = p_[i], b = p_[i - 1];
            int a2 = a + shift; if (a2 >= total) a2 -= total;
            int b2 = b + shift; if (b2 >= total) b2 -= total;
            if (c_[a] != c_[b] || c_[a2] != c_[b2]) classes++;
            cn_[p_[i]] = classes - 1;
        }
        for (int i = 0; i < total; i++) c_[i] = cn_[i];
        if (classes >= total) break;
    }
    // p_ now holds sorted cyclic shifts of t_ (length total); drop the one
    // starting at the sentinel position n to get the suffix array of s.
    int idx = 0;
    for (int i = 0; i < total; i++) {
        if (p_[i] != n) sa[idx++] = p_[i];
    }
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
}

void buildLCP() {
    int h = 0;
    lcpArr[0] = 0;
    for (int i = 0; i < n; i++) {
        if (rnk[i] > 0) {
            int j = sa[rnk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcpArr[rnk[i]] = h;
            if (h > 0) h--;
        } else {
            h = 0;
        }
    }
}

bool check(int L) {
    if (L == 0) return true;
    int run = 1;
    for (int i = 1; i < n; i++) {
        if (lcpArr[i] >= L) run++;
        else run = 1;
        if (run >= m) return true;
    }
    return run >= m;
}

int main() {
    while (scanf("%d", &m) == 1) {
        if (m == 0) break;
        scanf("%s", s);
        n = (int)strlen(s);
        if (m == 1) {
            printf("%d %d\n", n, 0);
            continue;
        }
        buildSuffixArray();
        buildLCP();
        int lo = 0, hi = n;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (check(mid)) lo = mid; else hi = mid - 1;
        }
        int bestL = lo;
        if (bestL == 0) {
            printf("none\n");
        } else {
            int bestPos = -1;
            int start = 0;
            for (int i = 1; i <= n; i++) {
                if (i < n && lcpArr[i] >= bestL) continue;
                int runLen = i - start;
                if (runLen >= m) {
                    int mx = 0;
                    for (int j = start; j < i; j++) if (sa[j] > mx) mx = sa[j];
                    if (mx > bestPos) bestPos = mx;
                }
                start = i;
            }
            printf("%d %d\n", bestL, bestPos);
        }
    }
    return 0;
}
