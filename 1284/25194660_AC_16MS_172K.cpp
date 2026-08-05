// POJ 1284 - Primitive Roots
// Model: claude-sonnet-5
// Approach: The number of primitive roots modulo an odd prime p equals
// phi(p-1) (Euler's totient of p-1), since the multiplicative group
// mod p is cyclic of order p-1, and a cyclic group of order n has
// exactly phi(n) generators. p < 65536, so p-1 < 65536; factorize
// p-1 by trial division up to sqrt(p-1) (~256) and apply the standard
// product formula for phi. No statement ambiguities noted.
#include <cstdio>

int main() {
    unsigned long p;
    while (scanf("%lu", &p) == 1) {
        unsigned long n = p - 1;
        unsigned long result = n;
        unsigned long m = n;
        for (unsigned long d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                result -= result / d;
                while (m % d == 0) m /= d;
            }
        }
        if (m > 1) result -= result / m;
        printf("%lu\n", result);
    }
    return 0;
}
