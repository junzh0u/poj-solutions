// POJ 3181 - Dollar Dayz
//
// Count the number of ways to write N as an unordered sum of positive
// integers each at most K (i.e. partitions of N into parts of size <= K).
// This is the classic unbounded-knapsack "coin change (count ways)" DP:
// dp[0] = 1, and for each part size p = 1..K, for each sum s = p..N,
// dp[s] += dp[s - p]. Iterating the part size in the OUTER loop and the
// running sum in the INNER loop (increasing) is what makes each multiset
// of parts get counted exactly once instead of once per permutation: by
// the time part p is processed, dp[s - p] already reflects every way to
// build s - p using parts <= p, so adding it to dp[s] only ever appends
// more copies of p, never reorders parts already chosen.
//
// The number of parts is unbounded (up to N of them, if K allows part
// size 1) while the part size is capped at K <= 100 -- that asymmetry is
// exactly why the loop order above is safe: the DP never needs to track
// "how many parts used", only "largest part size considered so far".
//
// The answer overflows every fixed-width integer type long before N and K
// reach their limits (e.g. for N = K = 1000 the count equals the ordinary
// partition function p(1000) = 24061467864032622473692149727991, already
// 32 digits, and it keeps growing with N up to 1000). Since the DP only
// ever adds partial counts together, a minimal bignum supporting just
// addition is enough: a little-endian vector of base-10^9 limbs.
//
// Nothing in the statement was ambiguous; it matches the sample (5 3 -> 5)
// and was verified against a Python big-integer reference over small N, K,
// against the exact value of p(1000), and at the largest legal input
// (N = 1000, K = 100).
#include <cstdio>
#include <vector>
#include <string>
using namespace std;

typedef unsigned int u32;
typedef unsigned long long u64;
static const u64 BASE = 1000000000ULL;

struct Big {
    vector<u32> d; // base 1e9, least-significant limb first
    Big() : d(1, 0) {}
    void addInPlace(const Big &o) {
        size_t n = d.size() > o.d.size() ? d.size() : o.d.size();
        if (d.size() < n) d.resize(n, 0);
        u64 carry = 0;
        for (size_t i = 0; i < n; ++i) {
            u64 sum = (u64)d[i] + (i < o.d.size() ? o.d[i] : 0) + carry;
            d[i] = (u32)(sum % BASE);
            carry = sum / BASE;
        }
        if (carry) d.push_back((u32)carry);
    }
    string toString() const {
        char buf[16];
        string s;
        size_t i = d.size();
        sprintf(buf, "%u", d[i - 1]);
        s += buf;
        while (i > 1) {
            --i;
            sprintf(buf, "%09u", d[i - 1]);
            s += buf;
        }
        return s;
    }
};

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    vector<Big> dp(n + 1);
    dp[0].d[0] = 1;

    for (int part = 1; part <= k; ++part) {
        for (int s = part; s <= n; ++s) {
            dp[s].addInPlace(dp[s - part]);
        }
    }

    printf("%s\n", dp[n].toString().c_str());
    return 0;
}
