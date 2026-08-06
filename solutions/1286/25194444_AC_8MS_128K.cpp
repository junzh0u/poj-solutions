// POJ 1286 - Necklace of Beads
//
// Count the 3-colourings (red / blue / green) of a ring of n beads (n < 24)
// that are distinct up to rotation AND reflection.  The symmetry group is the
// dihedral group D_n of order 2n, so Burnside's lemma gives the answer as the
// average number of colourings fixed by a group element:
//
//   Rotations: rotation by k splits the ring into gcd(k, n) cycles and so
//   fixes 3^gcd(k,n) colourings.  Summing k = 0 .. n-1 covers all n rotations
//   (k = 0 is the identity, contributing 3^n).
//
//   Reflections: the two cases differ by the parity of n.
//     n odd  - all n axes run through one bead and the midpoint of the
//              opposite edge, giving (n+1)/2 cycles each.
//     n even - n/2 axes run through two opposite beads (n/2 + 1 cycles) and
//              n/2 axes through two opposite edge midpoints (n/2 cycles).
//
// Divide the total by |D_n| = 2n; Burnside guarantees the quotient is exact,
// so plain integer division suffices.
//
// Sizes: the largest legal case is n = 23, whose unaveraged sum is about
// 3^23 = 9.4e10 - well past 32 bits - and whose answer, 2046856566, only just
// fits in a signed 32-bit int.  So the accumulator is "long long"; the result
// is printed digit by digit rather than with a format specifier, because POJ's
// compilers disagree on whether that is %lld or %I64d.  Submitted under
// language 0 (G++), which accepts "long long" happily.
//
// Statement notes:
//   - Input is one n per line and terminates on -1 (not EOF, and not 0).
//   - n = 0 is a legal input line and gets its own answer, 0: the Burnside
//     formula would divide by 2n = 0, and the judge wants 0 rather than the
//     empty necklace's arguable 1.
//
// Verified against a brute force that enumerates all 3^n colourings and
// canonicalises each under D_n, for every n from 1 to 12.

#include <cstdio>

static long long ipow(long long b, int e) {
    long long r = 1;
    for (int i = 0; i < e; ++i) r *= b;
    return r;
}

static int gcd(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

static void printLL(long long v) {
    char buf[32];
    int p = 0;
    if (v == 0) { putchar('0'); putchar('\n'); return; }
    while (v > 0) { buf[p++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (p-- > 0) putchar(buf[p]);
    putchar('\n');
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n == -1) break;
        if (n == 0) { printLL(0); continue; }

        long long total = 0;
        for (int k = 0; k < n; ++k) total += ipow(3, gcd(k, n));

        if (n % 2 == 1) {
            total += (long long)n * ipow(3, (n + 1) / 2);
        } else {
            total += (long long)(n / 2) * ipow(3, n / 2 + 1);
            total += (long long)(n / 2) * ipow(3, n / 2);
        }

        total /= (long long)(2 * n);
        printLL(total);
    }
    return 0;
}
