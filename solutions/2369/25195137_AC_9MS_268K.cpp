// POJ 2369 - Permutations
// Model: claude-opus-5
//
// The order of a permutation is the least k with P^k = E, which equals the
// least common multiple of its cycle lengths.  Decompose the permutation into
// cycles, then take the LCM.
//
// The discuss board is full of Wrong Answers caused by `a * b / gcd(a, b)`
// overflowing 32-bit int, with one report that even __int64 overflowed.  The
// statement promises the answer fits in 10^9, but Landau's function says an
// LCM of cycle lengths summing to 1000 can reach ~10^36, so instead of
// trusting either claim the LCM is accumulated exactly: factor every cycle
// length, keep the maximum exponent per prime, and multiply the result out in
// a small base-10^4 bignum.  No overflow is possible for any input, and the
// worst case for N = 1000 (about 10^34) costs microseconds.
//
// Identity permutation: every cycle has length 1, so the LCM is 1 - which is
// the answer the board insists on for input "5 / 1 2 3 4 5".
//
// The statement gives a single data set; the board disagrees about whether the
// judge data holds more.  Reading to EOF is correct under both readings, so
// the main loop consumes data sets until input runs out.

#include <cstdio>
#include <vector>

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 0) n = 0;
        std::vector<int> p(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (scanf("%d", &p[i]) != 1) p[i] = i;
            if (p[i] < 1 || p[i] > n) p[i] = i;   // defensive: never index OOB
        }

        // maxExp[q] = highest power of prime q dividing any cycle length
        std::vector<int> maxExp(n + 1, 0);
        std::vector<char> seen(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (seen[i]) continue;
            int len = 0;
            int j = i;
            while (!seen[j]) {
                seen[j] = 1;
                j = p[j];
                ++len;
            }
            int m = len;
            for (int q = 2; (long)q * q <= (long)m; ++q) {
                if (m % q) continue;
                int e = 0;
                while (m % q == 0) { m /= q; ++e; }
                if (e > maxExp[q]) maxExp[q] = e;
            }
            if (m > 1 && maxExp[m] < 1) maxExp[m] = 1;
        }

        // bignum, little-endian limbs.  Base 10^4 keeps every product below
        // 10^4 * 10^3 + carry, so plain unsigned long suffices and the code
        // stays free of the long long that C++98 does not have.
        const unsigned long BASE = 10000UL;
        std::vector<unsigned long> num;
        num.push_back(1UL);
        for (int q = 2; q <= n; ++q) {
            for (int e = 0; e < maxExp[q]; ++e) {
                unsigned long carry = 0;
                for (size_t k = 0; k < num.size(); ++k) {
                    unsigned long cur = num[k] * (unsigned long)q + carry;
                    num[k] = cur % BASE;
                    carry = cur / BASE;
                }
                while (carry) {
                    num.push_back(carry % BASE);
                    carry /= BASE;
                }
            }
        }

        printf("%lu", num[num.size() - 1]);
        for (size_t k = num.size() - 1; k > 0; --k)
            printf("%04lu", num[k - 1]);
        printf("\n");
    }
    return 0;
}
