// POJ 2992 - Divisors
// Model: gpt-5.6-terra
// For every prime p <= 431, precompute its exponent in each factorial.  The
// exponent in C(n, k) is then e(n!) - e(k!) - e((n-k)!), and the divisor
// count is the product of one plus those exponents.  Cache only queried pairs:
// the promised input values fit in signed 64 bits, while arbitrary pairs in
// the stated n range need not.
#include <cstdio>
#include <cstring>

const int LIMIT = 431;
const int MAX_PRIMES = 100;

int primes[MAX_PRIMES];
int prime_count;
int factorial_exponent[LIMIT + 1][MAX_PRIMES];
long long cached[LIMIT + 1][LIMIT + 1];

void build_factorial_exponents() {
    bool composite[LIMIT + 1];
    int n;
    int p;
    int i;

    std::memset(composite, 0, sizeof(composite));
    for (n = 2; n <= LIMIT; ++n) {
        if (!composite[n]) {
            primes[prime_count++] = n;
        }
        for (p = 0; p < prime_count && n * primes[p] <= LIMIT; ++p) {
            composite[n * primes[p]] = true;
            if (n % primes[p] == 0) {
                break;
            }
        }
    }

    for (n = 1; n <= LIMIT; ++n) {
        int value = n;
        std::memcpy(factorial_exponent[n], factorial_exponent[n - 1],
                    sizeof(factorial_exponent[n]));
        for (i = 0; i < prime_count; ++i) {
            while (value % primes[i] == 0) {
                ++factorial_exponent[n][i];
                value /= primes[i];
            }
            if (value == 1) {
                break;
            }
        }
    }
}

long long divisor_count(int n, int k) {
    long long answer = cached[n][k];
    int i;

    if (answer != 0) {
        return answer;
    }

    answer = 1;
    for (i = 0; i < prime_count; ++i) {
        const int exponent = factorial_exponent[n][i]
                           - factorial_exponent[k][i]
                           - factorial_exponent[n - k][i];
        answer *= exponent + 1;
    }
    cached[n][k] = answer;
    cached[n][n - k] = answer;
    return answer;
}

int main() {
    int n;
    int k;

    build_factorial_exponents();
    while (std::scanf("%d %d", &n, &k) == 2) {
        std::printf("%lld\n", divisor_count(n, k));
    }
    return 0;
}
