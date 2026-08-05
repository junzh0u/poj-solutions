// POJ 1743 - Musical Theme
//
// A "theme" is a subsequence of at least 5 notes that recurs (possibly
// transposed) somewhere else in the melody, disjoint from that other
// occurrence. Three things make this different from plain longest
// repeated substring:
//   1. Transposition invariance: work over the sequence of first
//      differences d[i] = a[i+1]-a[i] (length n-1), since adding a
//      constant to every note leaves the differences unchanged. Values
//      are shifted by +88 to keep suffix-array ranks non-negative.
//   2. Non-overlap: for a repeated diff-substring of length L found at
//      starting positions p and q, the corresponding note windows
//      [p, p+L] and [q, q+L] are disjoint only when |p-q| > L, which is
//      stricter than the plain "longest repeated substring" answer.
//   3. Length reporting: the answer is a note count, one more than the
//      matched diff-length L, and the L>=4 minimum (theme >= 5 notes)
//      must be checked after converting, not before.
//
// Approach: build the suffix array (O(n log^2 n) doubling, fine for
// n<=20000) and LCP array (Kasai) of the shifted diff sequence, then
// binary search on L: group adjacent suffixes in sorted order whose
// pairwise LCP >= L, and check whether any group's max start position
// minus its min start position exceeds L. The largest such L (if >=4)
// gives answer L+1; otherwise the answer is 0.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 20005;
int notesArr[MAXN];
int s[MAXN];
int N;
int sa[MAXN], rnk[MAXN], tmpr[MAXN], lcpArr[MAXN];
int k;

bool compareSA(int a, int b) {
    if (rnk[a] != rnk[b]) return rnk[a] < rnk[b];
    int ra = (a + k < N) ? rnk[a + k] : -1;
    int rb = (b + k < N) ? rnk[b + k] : -1;
    return ra < rb;
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

bool check(int L) {
    int mn = sa[0], mx = sa[0];
    for (int i = 1; i < N; i++) {
        if (lcpArr[i] >= L) {
            if (sa[i] < mn) mn = sa[i];
            if (sa[i] > mx) mx = sa[i];
        } else {
            mn = sa[i];
            mx = sa[i];
        }
        if (mx - mn > L) return true;
    }
    return false;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) scanf("%d", &notesArr[i]);
        N = n - 1;
        if (N < 4) {
            printf("0\n");
            continue;
        }
        for (int i = 0; i < N; i++) s[i] = (notesArr[i + 1] - notesArr[i]) + 88; // shift to keep non-negative
        buildSA();
        buildLCP();
        int lo = 1, hi = N - 1, best = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (check(mid)) { best = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        if (best >= 4) printf("%d\n", best + 1);
        else printf("0\n");
    }
    return 0;
}
