// POJ 3261 - Milk Patterns
//
// Find the longest substring of the sample sequence that occurs at least
// K times, occurrences may overlap (e.g. "1 2 3 2 3 2 3 1" has "2 3 2 3"
// twice, sharing the middle "2 3"). Unlike POJ 1743 (Musical Theme), no
// transposition is involved and overlap is explicitly allowed, so the
// plain "K suffixes share a long-enough prefix" suffix-array check applies
// directly.
//
// Approach: values are 0..1,000,000 so they are coordinate-compressed to
// 0..M-1 first, then a suffix array (O(n log^2 n) doubling) and its LCP
// array (Kasai) are built over the compressed sequence. Binary search on
// answer length L: check(L) scans the LCP array for a run of consecutive
// suffixes (in sorted order) with pairwise LCP >= L of length >= K - 1,
// i.e. a group of >= K suffixes sharing a common prefix of length >= L.
// Any such L is achievable (those K suffixes' starting positions are K
// occurrences of that length-L pattern, overlap allowed), so the largest
// feasible L is the answer. n <= 20000, so O(n log^2 n) is comfortably
// within the 2s per-case limit.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 20005;
int a[MAXN];      // raw values
int comp[MAXN];   // sorted unique values for compression
int s[MAXN];       // compressed ranks used as suffix "characters"
int N, K, M;       // M = number of distinct values
int sa[MAXN], rnk[MAXN], tmpr[MAXN], lcpArr[MAXN];
int k;

bool compareSA(int x, int y) {
    if (rnk[x] != rnk[y]) return rnk[x] < rnk[y];
    int rx = (x + k < N) ? rnk[x + k] : -1;
    int ry = (y + k < N) ? rnk[y + k] : -1;
    return rx < ry;
}

void buildSA() {
    for (int i = 0; i < N; i++) { sa[i] = i; rnk[i] = s[i]; }
    for (k = 1; k < N; k <<= 1) {
        sort(sa, sa + N, compareSA);
        tmpr[sa[0]] = 0;
        for (int i = 1; i < N; i++)
            tmpr[sa[i]] = tmpr[sa[i - 1]] + (compareSA(sa[i - 1], sa[i]) ? 1 : 0);
        for (int i = 0; i < N; i++) rnk[i] = tmpr[i];
        if (rnk[sa[N - 1]] == N - 1) break;
    }
}

void buildLCP() {
    int h = 0;
    for (int i = 0; i < N; i++) {
        if (rnk[i] > 0) {
            int j = sa[rnk[i] - 1];
            while (i + h < N && j + h < N && s[i + h] == s[j + h]) h++;
            lcpArr[rnk[i]] = h;
            if (h > 0) h--;
        } else {
            h = 0;
        }
    }
}

// True if some group of >= K suffixes, consecutive in SA order, share a
// common prefix of length >= L (i.e. a run of lcpArr[i] >= L of length
// >= K-1, giving K suffixes in that run).
bool check(int L) {
    int run = 1; // number of suffixes in current group (starts with sa[0])
    for (int i = 1; i < N; i++) {
        if (lcpArr[i] >= L) {
            run++;
        } else {
            run = 1;
        }
        if (run >= K) return true;
    }
    return false;
}

int main() {
    while (scanf("%d %d", &N, &K) == 2) {
        for (int i = 0; i < N; i++) scanf("%d", &a[i]);

        // Coordinate-compress the values (0..1,000,000) before building
        // the suffix structure.
        for (int i = 0; i < N; i++) comp[i] = a[i];
        sort(comp, comp + N);
        M = (int)(unique(comp, comp + N) - comp);
        for (int i = 0; i < N; i++)
            s[i] = (int)(lower_bound(comp, comp + M, a[i]) - comp);

        buildSA();
        buildLCP();

        int lo = 1, hi = N, best = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (check(mid)) { best = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        printf("%d\n", best);
    }
    return 0;
}
