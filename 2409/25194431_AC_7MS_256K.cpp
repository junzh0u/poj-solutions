// POJ 2409 - Let it Bead
//
// Count the c-colourings of a ring of s beads that are distinct up to rotation
// AND reflection: the symmetry group is the dihedral group D_s of order 2s, so
// Burnside's lemma gives the answer as the average number of colourings fixed
// by a group element.
//
//   Rotations: rotation by k splits the ring into gcd(k, s) cycles, so it
//   fixes c^gcd(k,s) colourings.  Summing over k = 0 .. s-1 covers all s of
//   them (k = 0 is the identity, contributing c^s).
//
//   Reflections: the two cases differ by the parity of s.
//     s odd  - all s axes run through one bead and the midpoint of the
//              opposite edge, giving (s+1)/2 cycles each.
//     s even - s/2 axes run through two opposite beads (s/2 + 1 cycles) and
//              s/2 axes run through two opposite edge midpoints (s/2 cycles).
//
// The total is then divided by |D_s| = 2s.  Burnside guarantees the quotient
// is exact, so plain integer division suffices - no modular inverse.
//
// Statement notes:
//   - "cs<=32" is the product c*s, as the statement itself spells out ("their
//     product does not exceed 32"), not c^s: the samples 2 6 and 6 2 both have
//     c^s > 32.  Under that bound c^s peaks at 65536, so the answer fits in 32
//     bits comfortably; the bignum below is insurance against test data that
//     ignores the stated bound, and it costs nothing at these sizes.
//   - Input terminates on the line "0 0"; s = 0 is a sentinel, never a case to
//     count (which matters, since the division by 2s would be by zero).
//
// The bignum is base 10^4 deliberately: every intermediate product then stays
// inside a 32-bit int, so the source needs no "long long", which POJ's C++
// compiler predates.
//
// Verified against a brute force that enumerates all c^s colourings and
// canonicalises each under D_s, and against a reference for every (c, s) with
// c*s <= 32.

#include <cstdio>
#include <vector>

using std::vector;

/* Little-endian base-10^4 non-negative bignum.  Base 10^4 keeps every
   intermediate product inside a 32-bit int, so the code needs no 64-bit type
   (POJ's C++ compiler predates "long long"). */
static const int BASE = 10000;

typedef vector<int> Big;

static Big fromInt(int v) {
    Big a;
    if (v == 0) { a.push_back(0); return a; }
    while (v > 0) { a.push_back(v % BASE); v /= BASE; }
    return a;
}

static void mulSmall(Big &a, int m) {
    int carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int cur = a[i] * m + carry;
        a[i] = cur % BASE;
        carry = cur / BASE;
    }
    while (carry > 0) { a.push_back(carry % BASE); carry /= BASE; }
}

static void addTo(Big &a, const Big &b) {
    int carry = 0;
    size_t n = a.size() > b.size() ? a.size() : b.size();
    a.resize(n, 0);
    for (size_t i = 0; i < n; ++i) {
        int cur = carry + a[i] + (i < b.size() ? b[i] : 0);
        a[i] = cur % BASE;
        carry = cur / BASE;
    }
    if (carry > 0) a.push_back(carry);
}

/* Exact division by a small positive divisor. */
static void divSmall(Big &a, int d) {
    int rem = 0;
    for (size_t i = a.size(); i-- > 0;) {
        int cur = rem * BASE + a[i];
        a[i] = cur / d;
        rem = cur % d;
    }
    while (a.size() > 1 && a[a.size() - 1] == 0) a.pop_back();
}

static Big powBig(int c, int e) {
    Big r = fromInt(1);
    for (int i = 0; i < e; ++i) mulSmall(r, c);
    return r;
}

static void printBig(const Big &a) {
    printf("%d", a[a.size() - 1]);
    for (size_t i = a.size() - 1; i-- > 0;) printf("%04d", a[i]);
    printf("\n");
}

static int gcd(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

int main() {
    int c, s;
    while (scanf("%d %d", &c, &s) == 2) {
        if (c == 0 && s == 0) break;

        Big total = fromInt(0);

        /* Rotations: rotation by k fixes c^gcd(k,s) colourings. */
        for (int k = 0; k < s; ++k) {
            Big t = powBig(c, gcd(k, s));
            addTo(total, t);
        }

        /* Reflections. */
        if (s % 2 == 1) {
            /* s axes, each through one bead and the opposite edge midpoint:
               (s+1)/2 cycles each. */
            Big t = powBig(c, (s + 1) / 2);
            mulSmall(t, s);
            addTo(total, t);
        } else {
            /* s/2 axes through two opposite beads: s/2 + 1 cycles each.
               s/2 axes through two opposite edge midpoints: s/2 cycles each. */
            Big t1 = powBig(c, s / 2 + 1);
            mulSmall(t1, s / 2);
            addTo(total, t1);
            Big t2 = powBig(c, s / 2);
            mulSmall(t2, s / 2);
            addTo(total, t2);
        }

        /* Burnside: the group has 2s elements and the quotient is exact. */
        divSmall(total, 2 * s);
        printBig(total);
    }
    return 0;
}
