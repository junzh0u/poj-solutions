// POJ 3928 - Ping pong
// Model: claude-opus-5
//
// Players sit on a line west to east; player i has distinct skill a[i].
// A game is a pair of contestants plus a referee, where
//   * the referee's rank is neither higher than both nor lower than both
//     contestants' -- with all ranks distinct that means strictly between them;
//   * walking distance |p-r| + |q-r| <= |p-q|, which for points on a line means
//     the referee sits positionally between the two contestants.
// So the answer counts index triples i < j < k with
//   a[i] < a[j] < a[k]   or   a[i] > a[j] > a[k].
//
// Fix the referee j and multiply: (#smaller to the left)*(#larger to the right)
// + (#larger to the left)*(#smaller to the right).  Two Fenwick sweeps over the
// value range 1..100000 (forward, then backward) give all four counts in
// O(n log V) per test case.
//
// The result needs 64 bits: the maximum is C(20000,3) = 1333133340000, reached
// by a strictly increasing input.  POJ's C++ compiler is unreliable with
// "%lld", so the answer is printed with std::cout instead of printf.

#include <cstdio>
#include <iostream>

static const int MAXV = 100000;
static const int MAXN = 20005;

static int bit[MAXV + 2];
static int a[MAXN];
static int leftLess[MAXN];
static int rightLess[MAXN];

static void bitClear() {
    for (int i = 0; i <= MAXV + 1; ++i) bit[i] = 0;
}

static void bitAdd(int i) {
    for (; i <= MAXV; i += i & (-i)) ++bit[i];
}

/* number of inserted values in [1, i] */
static int bitSum(int i) {
    int s = 0;
    for (; i > 0; i -= i & (-i)) s += bit[i];
    return s;
}

int main() {
    int T;
    while (scanf("%d", &T) == 1) {
        for (int tc = 0; tc < T; ++tc) {
            int n;
            if (scanf("%d", &n) != 1) return 0;
            for (int i = 0; i < n; ++i) scanf("%d", &a[i]);

            bitClear();
            for (int i = 0; i < n; ++i) {
                leftLess[i] = bitSum(a[i] - 1);
                bitAdd(a[i]);
            }
            bitClear();
            for (int i = n - 1; i >= 0; --i) {
                rightLess[i] = bitSum(a[i] - 1);
                bitAdd(a[i]);
            }

            long long ans = 0;
            for (int i = 0; i < n; ++i) {
                long long lLess = leftLess[i];
                long long lMore = (long long)i - lLess;
                long long rLess = rightLess[i];
                long long rMore = (long long)(n - 1 - i) - rLess;
                ans += lLess * rMore + lMore * rLess;
            }
            std::cout << ans << "\n";
        }
    }
    return 0;
}
