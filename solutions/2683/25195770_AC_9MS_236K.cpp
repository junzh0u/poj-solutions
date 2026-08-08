// POJ 2683 - Ohgas' Fortune
// Model: claude-opus-5
//
// Direct simulation: for each operation, run the stated year loop and keep the
// best final amount.  Per year the interest is floor(balance * rate); compound
// folds it into the balance (bal += interest - charge), simple banks it aside
// (acc += interest; bal -= charge) and the final amount is bal + acc.
//
// "Rounding off its fractional part" is truncation, not round-to-nearest: the
// statement's own worked table has 1028250 * 0.03125 = 32132.8125 -> 32132.
//
// The rate is guaranteed to be an integral multiple of 1/8192, so the whole
// computation is done in exact integers rather than floating point.  A double
// is used only to parse the decimal, then k = round(rate * 8192) recovers the
// exact numerator.  floor(bal * k / 8192) is then split as
// (bal/8192)*k + ((bal%8192)*k)/8192 because bal*k overflows a 32-bit long
// (bal reaches ~3.2e8 and k reaches 1024).  Every term of the split, and every
// value carried, stays under 2^31: the largest final amount reachable under the
// input bounds is 1e8 * 1.125^10 = 324732096.

#include <cstdio>
#include <cmath>

/* floor(a * k / 8192) without overflowing a 32-bit long */
static long mulShift(long a, long k) {
    return (a / 8192L) * k + ((a % 8192L) * k) / 8192L;
}

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;
    while (m-- > 0) {
        long fund;
        int years, n;
        if (scanf("%ld %d %d", &fund, &years, &n) != 3) return 0;
        long best = 0;
        for (int i = 0; i < n; ++i) {
            int kind;
            double rate;
            long charge;
            scanf("%d %lf %ld", &kind, &rate, &charge);
            long k = (long)floor(rate * 8192.0 + 0.5);
            long bal = fund, acc = 0;
            for (int y = 0; y < years; ++y) {
                long interest = mulShift(bal, k);
                if (kind == 1) {
                    bal = bal + interest - charge;
                } else {
                    acc += interest;
                    bal -= charge;
                }
            }
            long total = bal + acc;
            if (i == 0 || total > best) best = total;
        }
        printf("%ld\n", best);
    }
    return 0;
}
