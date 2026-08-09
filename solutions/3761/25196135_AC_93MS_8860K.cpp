// POJ 3761 - Bubble Sort
// Model: gpt-5.6-terra
// Approach: represent a permutation by the independent inversion-table entries.
// A bubble pass decreases every positive entry once, so the number of rounds is
// their maximum. Count tables whose maximum is exactly K by subtracting the
// counts bounded by K and K-1.
#include <cstdio>

const unsigned long long MOD = 20100713ULL;
unsigned long long fact[1000001];

unsigned long long mod_pow(unsigned long long base, int exponent) {
    unsigned long long result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

int main() {
    fact[0] = 1;
    for (int i = 1; i <= 1000000; ++i) fact[i] = fact[i - 1] * i % MOD;

    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int n, k;
        std::scanf("%d%d", &n, &k);
        if (k == 0) {
            std::puts("1");
            continue;
        }
        const int exponent = n - k;
        const unsigned long long at_most_k = fact[k] * mod_pow(k + 1, exponent) % MOD;
        const unsigned long long at_most_before = fact[k] * mod_pow(k, exponent) % MOD;
        std::printf("%llu\n", (at_most_k + MOD - at_most_before) % MOD);
    }
    return 0;
}
