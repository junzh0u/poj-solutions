// POJ 3595 - Full Steiner Topologies
// Model: gpt-6-astra
// Inserting the new leaf subdivides one of the previous tree's edges,
// giving (2n-5)!! topologies. Evaluate its logarithm with a corrected
// Stirling expansion in constant time; multiply directly for small n.
// Round four significant digits explicitly, including the n=8 tie,
// and carry a rounded mantissa of 10 into the exponent.
#include <cstdio>
#include <cmath>

long double correction(long double x) {
    long double t = 1 / x, t2 = t*t;
    return t * (1.0L/12 - t2*(1.0L/360 - t2*(1.0L/1260
               - t2/1680)));
}
int main() {
    int n;
    while (std::scanf("%d", &n) == 1) {
        int e = 0;
        long double m = 1;
        if (n <= 102) {
            for (int i = 3; i <= 2*n-5; i += 2) {
                m *= i;
                while (m >= 10) { m /= 10; ++e; }
            }
        } else {
            long double k = n-2;
            long double a = (k*(std::log(2*k)-1) + std::log(2.0L)/2
                            + correction(2*k)-correction(k))/std::log(10.0L);
            e = (int)std::floor(a);
            m = std::pow(10.0L, a-e);
        }
        int digits = (int)std::floor(m*1000 + 0.5L + 1e-10L);
        if (digits == 10000) { digits = 1000; ++e; }
        std::printf("%d.%03dE%d\n", digits/1000, digits%1000, e);
    }
    return 0;
}
