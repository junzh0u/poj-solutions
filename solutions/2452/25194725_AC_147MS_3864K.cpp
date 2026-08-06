// POJ 2452 - Sticks Problem
// Model: claude-sonnet-5
//
// Approach:
//   For a pair i<j, the condition "every stick between S_i and S_j is longer
//   than S_i but shorter than S_j" plus the (implicit, confirmed by sample 2
//   which decreases monotonically and answers -1) requirement S_i<S_j means
//   S_i must be the minimum and S_j the maximum of the subarray S[i..j].
//   For each i, let R[i] be the next index to the right with a smaller value
//   (n+1 if none). Any valid j for this i must satisfy i<j<R[i] (so every
//   element strictly between stays > S_i, and S_j>S_i too). Within the
//   window (i, R[i]), the largest valid j is exactly the position of the
//   maximum value in that window: it is greater than everything before it in
//   the window (by definition of being the max), and any j after it can't be
//   the running max since values are distinct. So for each i we only need
//   the argmax of a[i+1 .. R[i]-1], found via a sparse table (RMQ), and take
//   the best j-i over all i. R[] is built with a standard O(n) monotonic
//   stack (next-smaller-to-the-right). Multiple test cases until EOF.
//
// Ambiguity: the statement's "each stick placed between Si and Sj is longer
// than Si but shorter than Sj" is vacuously true when j=i+1 (nothing is
// between them). Sample 2 (6 5 4 3, strictly decreasing) answers -1 rather
// than 1, so S_i<S_j must ALSO hold explicitly, even for adjacent pairs with
// nothing between; the R[i] boundary above already encodes exactly that.

#include <cstdio>

static const int MAXN = 50005;
static const int LOG = 17;

static int a[MAXN];
static int R[MAXN];
static int st[LOG][MAXN]; // st[k][i] = index of max value in [i, i+2^k-1]
static int lg[MAXN];
static int stk[MAXN];

static int n;

static inline int cmpIdx(int i, int j) {
    return a[i] > a[j] ? i : j;
}

static void buildSparse() {
    for (int i = 1; i <= n; ++i) st[0][i] = i;
    int maxK = lg[n] + 1;
    for (int k = 1; k < maxK && k < LOG; ++k) {
        int len = 1 << k;
        int half = len >> 1;
        for (int i = 1; i + len - 1 <= n; ++i) {
            st[k][i] = cmpIdx(st[k - 1][i], st[k - 1][i + half]);
        }
    }
}

static inline int queryMaxIdx(int l, int r) { // inclusive
    int k = lg[r - l + 1];
    return cmpIdx(st[k][l], st[k][r - (1 << k) + 1]);
}

int main() {
    lg[1] = 0;
    for (int i = 2; i < MAXN; ++i) lg[i] = lg[i / 2] + 1;

    while (scanf("%d", &n) == 1) {
        for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);

        int top = 0;
        for (int i = 1; i <= n; ++i) {
            while (top > 0 && a[stk[top]] > a[i]) {
                R[stk[top]] = i;
                --top;
            }
            stk[++top] = i;
        }
        while (top > 0) {
            R[stk[top]] = n + 1;
            --top;
        }

        if (n >= 2) buildSparse();

        int ans = -1;
        for (int i = 1; i <= n; ++i) {
            int lo = i + 1, hi = R[i] - 1;
            if (lo <= hi) {
                int j = queryMaxIdx(lo, hi);
                int cand = j - i;
                if (cand > ans) ans = cand;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
