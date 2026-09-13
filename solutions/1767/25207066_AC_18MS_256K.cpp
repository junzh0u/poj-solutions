// POJ 1767 - Which is Next
// Model: claude-sonnet-5
//
// Key fact (verified by expanding the given recursive formula): N(t1,t2) =
// 2^(|t1|+|t2|) + 2^|t2|*N(t1) + N(t2) is exactly the preorder traversal
// of t read as a binary number, emitting a 1 bit at every internal node
// and a 0 bit at every leaf. A tree of size k therefore corresponds to a
// length-k "Lukasiewicz"/Dyck-style bit string: track a pending-slot
// counter starting at 1, each '1' bit (+1 slot) each '0' bit (-1 slot,
// consumes a leaf), the counter must stay >=1 through every bit except
// the last, where it must hit exactly 0.
//
// The statement's order relation (leaf smallest; compare t1 first then
// t2) looks, at first glance, like it should decompose by splitting size
// a=|t1| first -- but that is a trap: for two same-total-size trees whose
// splits differ, comparing t1 vs u1 uses the SAME order across different
// sizes, which is equivalent to comparing them as binary fractions
// N/2^size (pad the shorter preorder string with trailing zeros before
// comparing). A first attempt built the successor by carrying along the
// (a,b) split explicitly and got this wrong (e.g. for a size-9 case the
// "advance t1 within its own split, reset t2" step skipped over trees
// that belong to a *different* split but sort in between). The fix is
// to stop thinking about splits altogether: since every tree compared
// here has the *same* total size k, the fraction order degenerates to
// plain numeric order on the k-bit preorder string, i.e. plain lexicographic
// order on same-length bit strings. So the successor of t in B(k) is just
// "the lexicographically next valid length-k Dyck/Lukasiewicz bit string",
// solved with the classical next-permutation-style algorithm: find the
// rightmost position holding a 0 that can be flipped to 1 while the
// remainder is still completable, then fill the remainder with the
// lexicographically smallest valid completion (greedily prefer 0, i.e.
// closing a leaf, whenever that keeps the rest completable). If no such
// position exists, t is already the maximum of B(k) and the answer wraps
// to the minimum of B(k) (computed by the same greedy fill from scratch).
//
// Verified against a from-scratch brute force (enumerate all of B(k) for
// odd k up to 21, sort by N, compare successors) over 23714 cases, plus
// the sample.
//
// All arithmetic fits in 32 bits (N <= 2^30, k <= 31), so plain `long`
// suffices; no 64-bit types are needed.

#include <cstdio>
#include <vector>
using namespace std;

typedef long LL;

int bitlen(LL x) { // x > 0
    int b = 0;
    while (x) { x >>= 1; b++; }
    return b;
}

// Can we go from `p` pending slots to exactly 0 in exactly `r` more steps
// (each step +1 or -1), staying >=1 before every step except possibly
// reaching 0 only at the very last one?
bool feasible(int p, int r) {
    if (r == 0) return p == 0;
    if (p < 1) return false;
    return (r >= p) && ((r - p) % 2 == 0);
}

// Greedily fill bits[pos..k-1] (0-indexed, length k total) starting from
// pending count p, choosing the lexicographically smallest valid
// continuation (prefer bit 0 whenever the remainder stays completable).
void fillGreedy(vector<int> &bits, int pos, int k, int p) {
    for (; pos < k; pos++) {
        int rem = k - 1 - pos; // positions remaining AFTER this one
        int newp0 = p - 1;
        if (feasible(newp0, rem)) {
            bits[pos] = 0;
            p = newp0;
        } else {
            bits[pos] = 1;
            p = p + 1;
        }
    }
}

LL bitsToVal(const vector<int> &bits, int k) {
    LL v = 0;
    for (int j = 0; j < k; j++) v = (v << 1) | bits[j];
    return v;
}

LL minN(int k) {
    vector<int> bits(k);
    fillGreedy(bits, 0, k, 1);
    return bitsToVal(bits, k);
}

LL successor(LL n, int k) {
    vector<int> b(k), P(k + 1);
    for (int j = 0; j < k; j++) b[j] = (int)((n >> (k - 1 - j)) & 1);
    P[0] = 1;
    for (int j = 0; j < k; j++) P[j + 1] = P[j] + (b[j] ? 1 : -1);

    int flipPos = -1;
    for (int i = k - 1; i >= 0; i--) {
        if (b[i] == 0) {
            int newp = P[i] + 1;
            int rem = k - 1 - i;
            if (feasible(newp, rem)) { flipPos = i; break; }
        }
    }

    if (flipPos == -1) return minN(k);

    vector<int> nb(k);
    for (int j = 0; j < flipPos; j++) nb[j] = b[j];
    nb[flipPos] = 1;
    fillGreedy(nb, flipPos + 1, k, P[flipPos] + 1);
    return bitsToVal(nb, k);
}

int main() {
    LL n;
    while (scanf("%ld", &n) == 1) {
        int k = (n == 0) ? 1 : bitlen(n);
        printf("%ld\n", successor(n, k));
    }
    return 0;
}
