// POJ 3786 - Adjacent Bit Counts
// Model: claude-sonnet-5
//
// For a bit string x1..xn, AdjBC(x) = sum x_i*x_{i+1}. Count n-bit strings
// with AdjBC = k. n <= 100 so intermediate DP counts can reach 2^100, far
// beyond 32-bit/64-bit range, even though the queried (n,k) answer is
// guaranteed to fit a signed 32-bit int. So the DP is carried out with a
// small fixed-size bignum (base 1e9, addition only - no multiplication is
// ever needed), and only the final requested value is printed.
//
// dp[c][last] = number of strings of the current length with adjacent-bit
// count c and last bit `last`. Extending by one bit: appending 0 keeps c
// the same; appending 1 adds 1 to c iff the previous bit was 1.

#include <cstdio>
#include <cstring>

const long long BASE = 1000000000LL;
const int LIMBS = 6; // 6*9 = 54 decimal digits, far more than 2^100 needs (~31 digits)

struct Big {
    long long d[LIMBS];
};

Big makeZero() {
    Big b;
    for (int i = 0; i < LIMBS; ++i) b.d[i] = 0;
    return b;
}

Big makeOne() {
    Big b = makeZero();
    b.d[0] = 1;
    return b;
}

Big add(const Big &a, const Big &b) {
    Big r;
    long long carry = 0;
    for (int i = 0; i < LIMBS; ++i) {
        long long s = a.d[i] + b.d[i] + carry;
        r.d[i] = s % BASE;
        carry = s / BASE;
    }
    return r;
}

void printBig(const Big &a) {
    int hi = LIMBS - 1;
    while (hi > 0 && a.d[hi] == 0) --hi;
    printf("%lld", a.d[hi]);
    for (int i = hi - 1; i >= 0; --i) {
        printf("%09lld", a.d[i]);
    }
}

static Big dp[105][2];
static Big ndp[105][2];

int main() {
    int P;
    if (scanf("%d", &P) != 1) return 0;
    for (int t = 0; t < P; ++t) {
        int idx, n, k;
        scanf("%d %d %d", &idx, &n, &k);

        for (int c = 0; c < 105; ++c) {
            dp[c][0] = makeZero();
            dp[c][1] = makeZero();
        }

        if (n >= 1) {
            dp[0][0] = makeOne();
            dp[0][1] = makeOne();
        }

        for (int i = 2; i <= n; ++i) {
            for (int c = 0; c < 105; ++c) {
                ndp[c][0] = makeZero();
                ndp[c][1] = makeZero();
            }
            int maxc = i - 2; // max adjBC achievable with i-1 bits
            for (int c = 0; c <= maxc; ++c) {
                for (int last = 0; last < 2; ++last) {
                    // append 0
                    ndp[c][0] = add(ndp[c][0], dp[c][last]);
                    // append 1
                    int delta = (last == 1) ? 1 : 0;
                    ndp[c + delta][1] = add(ndp[c + delta][1], dp[c][last]);
                }
            }
            for (int c = 0; c < 105; ++c) {
                dp[c][0] = ndp[c][0];
                dp[c][1] = ndp[c][1];
            }
        }

        Big ans;
        if (n >= 1 && k >= 0 && k <= n - 1) {
            ans = add(dp[k][0], dp[k][1]);
        } else if (n == 0 && k == 0) {
            ans = makeOne(); // empty string, degenerate, not expected by constraints
        } else {
            ans = makeZero();
        }

        printf("%d ", idx);
        printBig(ans);
        printf("\n");
    }
    return 0;
}
