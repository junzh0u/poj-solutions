// POJ 2917 - Diophantus of Alexandria
// Model: claude-opus-5
//
// Count pairs (x, y) of positive integers with x <= y and 1/x + 1/y = 1/n.
//
// 1/x + 1/y = 1/n  <=>  n(x + y) = xy  <=>  (x - n)(y - n) = n^2.
// x, y > n necessarily, so the solutions are exactly the factorizations
// n^2 = a * b with a = x - n, b = y - n positive; x <= y <=> a <= b.
// Divisors of n^2 pair up as (a, n^2/a) and n^2 is a perfect square, so
// d(n^2) is odd and the number of pairs with a <= b is (d(n^2) + 1) / 2.
// (n = 1 needs no special case: d(1) = 1 gives 1, i.e. 1/2 + 1/2 = 1/1.)
//
// d(n^2) = prod (2 * e_i + 1) over the prime factorization n = prod p_i^e_i,
// obtained by trial division over primes up to sqrt(10^9) < 31623; any
// remaining factor > 1 is a single prime with exponent 1.
//
// Bounds: n <= 10^9 fits in int; max d(n^2) in range is ~25515, so the
// answer fits in int too -- no 64-bit arithmetic anywhere.
//
// Output: "Scenario #i:", the count, then a blank line after each scenario
// (the discuss board notes the judge's own expected output lacks the final
// blank line, but POJ ignores trailing whitespace).

#include <cstdio>

const int LIM = 31623; /* > sqrt(10^9) */

int primes[3450];
int pcnt = 0;
bool composite[LIM + 1];

static void sieve() {
    int i, j;
    for (i = 2; i <= LIM; ++i) {
        if (!composite[i]) {
            primes[pcnt++] = i;
            for (j = i + i; j <= LIM; j += i) composite[j] = true;
        }
    }
}

/* number of divisors of n^2 */
static int divisors_of_square(int n) {
    int res = 1;
    int i;
    for (i = 0; i < pcnt; ++i) {
        int p = primes[i];
        if (p > n / p) break; /* p * p > n, avoids overflow */
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            res *= 2 * e + 1;
        }
    }
    if (n > 1) res *= 3; /* leftover prime, exponent 1 */
    return res;
}

int main() {
    int t, i, n;
    sieve();
    if (scanf("%d", &t) != 1) return 0;
    for (i = 1; i <= t; ++i) {
        if (scanf("%d", &n) != 1) break;
        printf("Scenario #%d:\n%d\n\n", i, (divisors_of_square(n) + 1) / 2);
    }
    return 0;
}
