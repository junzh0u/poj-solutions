// POJ 3911 - Internet Service Providers
// Model: claude-sonnet-5
// Each dataset gives N, C. Profit is proportional to f(T) = T*(C - T*N),
// a downward parabola in T (since N>=0) with real-valued vertex at
// T* = C/(2N). The integer optimum is one of floor(T*) or ceil(T*);
// evaluate both with 64-bit arithmetic (T*N can reach ~5e17) and take
// whichever yields the larger f(T), preferring the smaller T on a tie
// (smallest T achieving the maximum, per the statement).
// Special case N == 0: f(T) = T*C is monotonic in T (unbounded profit
// for C>0), so there is no true maximum; the accepted convention
// (confirmed by the sample "0 1" -> 0) is T_optim = 0.
#include <cstdio>

int main() {
    long long n, c;
    while (scanf("%lld %lld", &n, &c) == 2) {
        if (n == 0) {
            printf("0\n");
            continue;
        }
        long long t0 = c / (2 * n);
        long long t1 = t0 + 1;
        long long f0 = t0 * (c - t0 * n);
        long long f1 = t1 * (c - t1 * n);
        long long best = (f1 > f0) ? t1 : t0;
        printf("%lld\n", best);
    }
    return 0;
}
