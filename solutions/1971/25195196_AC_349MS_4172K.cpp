// POJ 1971 - Parallelogram Counting
// Model: claude-opus-5
//
// Approach: a quadrilateral ABCD is a parallelogram exactly when its diagonals
// bisect each other, i.e. A + C == B + D.  So enumerate all C(n,2) point pairs,
// key each by the *doubled* midpoint (x_i + x_j, y_i + y_j) -- doubling keeps
// everything integral, no floating point -- sort the keys, and add C(k,2) for
// every group of k pairs sharing a key.  n <= 1000 gives 499500 keys per test
// case, and 10 test cases sort ~5M 64-bit keys, comfortably inside 5s.
//
// Each unordered pair {A,C},{B,D} with a common midpoint yields one
// parallelogram, and no pair is double counted: two pairs sharing a key cannot
// share a point (that would force two input points to coincide), and a single
// 4-subset cannot be a parallelogram under two different diagonal pairings
// (A+C == B+D together with A+B == C+D forces B == C).  The statement's
// guarantee "No four points are in a straight line" (confirmed on the discuss
// board to be a promise about the input data) is what rules out degenerate
// "parallelograms" with all four vertices collinear, so no filtering is needed.
//
// Coordinates are up to 1e9 in magnitude, so a coordinate sum lies in
// [-2e9, 2e9]; shifting by 2e9 puts it in [0, 4e9], which fits an unsigned
// 32-bit field, and the two shifted sums pack losslessly into one unsigned
// 64-bit key.  Answers exceed 5e7 on grid-like inputs, hence long long.
//
// Submitted under language 0 (G++): the discuss board reports this problem
// timing out under C++ while the same code is accepted under G++.

#include <cstdio>
#include <iostream>
#include <algorithm>

static const int MAXN = 1000;
static int X[MAXN + 5], Y[MAXN + 5];
static unsigned long long key[(size_t)MAXN * (MAXN - 1) / 2 + 1];

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n;
        if (scanf("%d", &n) != 1) return 0;
        for (int i = 0; i < n; ++i) {
            if (scanf("%d %d", &X[i], &Y[i]) != 2) return 0;
        }

        size_t m = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                unsigned long long ux =
                    (unsigned long long)((long long)X[i] + X[j] + 2000000000LL);
                unsigned long long uy =
                    (unsigned long long)((long long)Y[i] + Y[j] + 2000000000LL);
                key[m++] = (ux << 32) | uy;
            }
        }
        std::sort(key, key + m);

        long long ans = 0;
        size_t i = 0;
        while (i < m) {
            size_t j = i;
            while (j < m && key[j] == key[i]) ++j;
            long long k = (long long)(j - i);
            ans += k * (k - 1) / 2;
            i = j;
        }
        std::cout << ans << "\n";
    }
    return 0;
}
