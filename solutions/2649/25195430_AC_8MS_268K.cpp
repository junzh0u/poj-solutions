// POJ 2649 - Factovisors
// Model: claude-sonnet-5
// Approach: factorize m via trial division over primes up to sqrt(2^31)
// (sieved with Eratosthenes up front), then for each prime factor p^e
// of m, check whether its exponent in n! (via Legendre's formula,
// iterative, no recursion -- the board's RE reports trace to recursive
// factorization blowing the stack) is >= e. Board also flags m==0 and
// m==1 as the traps: m==0 never divides n! (n! is never zero, since
// 0!=1 and it only grows), and m==1 always divides. n,m fit in 32 bits
// (< 2^31), so unsigned int + %u sidesteps POJ's old MinGW needing
// %I64d for 64-bit i/o; internal accumulators use unsigned long long.
#include <cstdio>
#include <vector>

int main() {
    const int LIMIT = 46341; // > sqrt(2^31-1)
    std::vector<bool> sieve(LIMIT + 1, true);
    std::vector<unsigned int> primes;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            primes.push_back((unsigned int)i);
            for (long long j = (long long)i * i; j <= LIMIT; j += i)
                sieve[(size_t)j] = false;
        }
    }

    unsigned int n, m;
    while (scanf("%u %u", &n, &m) == 2) {
        bool divides;
        if (m == 0) {
            divides = false;
        } else if (m == 1) {
            divides = true;
        } else {
            unsigned int rem = m;
            divides = true;
            for (size_t i = 0; i < primes.size() &&
                 (unsigned long long)primes[i] * primes[i] <= rem; ++i) {
                unsigned int p = primes[i];
                if (rem % p == 0) {
                    unsigned int e = 0;
                    while (rem % p == 0) { rem /= p; ++e; }
                    unsigned long long cnt = 0, temp = n;
                    while (temp) { temp /= p; cnt += temp; }
                    if (cnt < e) { divides = false; break; }
                }
            }
            if (divides && rem > 1) {
                unsigned int p = rem; // leftover prime factor, exponent 1
                unsigned long long cnt = 0, temp = n;
                while (temp) { temp /= p; cnt += temp; }
                if (cnt < 1) divides = false;
            }
        }
        if (divides)
            printf("%u divides %u!\n", m, n);
        else
            printf("%u does not divide %u!\n", m, n);
    }
    return 0;
}
