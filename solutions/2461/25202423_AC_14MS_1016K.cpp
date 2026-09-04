// POJ 2461 - Magic Bitstrings
// Model: claude-sonnet-5
//
// Approach: Index the bitstring positions 1..p-1 (the appended non-bit x sits
// at position 0 of the cyclic length-p string and, because p is prime, is
// never touched by any row). Row m consists of bits at positions m, 2m, 3m,
// ... mod p, i.e. row m at column k equals s(k*m mod p). Requiring every row
// to equal s or its complement forces, writing f(k) = s(k):
//   f(k*m) = f(k) XOR f(m) XOR f(1)   for all k, m in (Z/pZ)^*.
// Substituting h(k) = f(k) XOR f(1) turns this into h(k*m) = h(k) XOR h(m),
// i.e. h is a group homomorphism from the cyclic group (Z/pZ)^* (order p-1)
// to Z/2Z. For odd primes p (p-1 even) there are exactly two such
// homomorphisms: the trivial one (giving the excluded constant string) and
// the unique nontrivial one, whose kernel is the index-2 subgroup of
// quadratic residues. So h(k) = 0 if k is a QR mod p, else 1, and
// f(k) = h(k) XOR f(1). Choosing f(1) = 0 (always valid since 1 is a QR)
// makes s(1) = 0, which is optimal for lexicographic order, giving:
//   s(k) = '0' if k is a quadratic residue mod p, else '1'.
// For p = 2 there is no nontrivial homomorphism ((Z/2Z)^* has order 1, odd),
// so only the constant string exists and the answer is Impossible.
//
// Statement/board note: the board (message 113763) independently confirms
// the derivation's consequence that the diagonal of the matrix is all 0s
// when s(1)=0, matching this construction. Verified against all four sample
// primes (5, 3, 17, 47) by hand/brute force and p=2 -> Impossible, plus a
// randomized differential check against an O(p^2) brute force that builds
// the actual matrix and checks the magic property directly.

#include <cstdio>
#include <cstring>

static bool isQR[100001];

int main() {
    int p;
    static char buf[100001];
    while (scanf("%d", &p) == 1 && p != 0) {
        if (p == 2) {
            printf("Impossible\n");
            continue;
        }
        memset(isQR, 0, sizeof(bool) * p);
        for (int i = 1; i <= (p - 1) / 2; ++i) {
            unsigned int ii = (unsigned int)i;
            isQR[(ii * ii) % (unsigned int)p] = true;
        }
        int n = p - 1;
        for (int k = 1; k <= n; ++k) {
            buf[k - 1] = isQR[k] ? '0' : '1';
        }
        buf[n] = '\0';
        printf("%s\n", buf);
    }
    return 0;
}
