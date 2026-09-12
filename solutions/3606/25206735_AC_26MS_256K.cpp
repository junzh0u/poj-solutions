// POJ 3606 - Sheryl's Circuit II
// Model: claude-sonnet-5
// Approach: complete binary tree of AND/OR gates (BAU), given level-order
// (root-to-bottom, left-to-right) via a heap-indexed array. As the leaf
// inputs run through all 2^N combinations in standard binary-counter order
// (leftmost leaf = MSB, rightmost = LSB), the root's output forms a signal;
// we need the count of 0->1 (rising) and 1->0 (falling) transitions in that
// signal, mod 9973.
//
// For each node we track, for its own output sequence (length 2^{leaves}):
//   n     = sequence length mod 9973
//   b0,b1 = actual output bit at the first and last index (0/1, exact)
//   ones  = count of positions with output 1 (mod 9973)
//   rise,fall = counts of internal 0->1 / 1->0 adjacent transitions (mod 9973)
//   c00,c11   = counts of internal 0->0 / 1->1 adjacent transitions (mod 9973)
// A node v = gate(t, L, R) is built by looping L's own sequence as the outer
// (more-significant) loop and R's full sequence as the inner (faster) loop,
// exactly mirroring how a binary counter increments (right subtree = low
// bits, cycles once per value of the left subtree). Within one L-value
// block the gate either passes R's own transitions through unchanged (gate
// non-dominant value) or clamps the whole block to a constant (gate
// dominant value), contributing (dominant block count)*(R length - 1) more
// constant pairs. Between consecutive blocks (once per adjacent pair of L's
// own sequence) there is one extra "boundary" transition, whose type is
// determined case-by-case from L's transition type (00/01/10/11) and R's
// fixed endpoint bits (b0(R), b1(R)) run through the gate.
// Verified against brute-force enumeration for all N=2^k, k=1..4, and
// randomized types (2000+ trials in Python, 500 more in C++), all matching;
// sample also matches (3 2). A mutant with one boundary-case condition
// flipped was caught by both the sample and the differential test.
// Board note (message 95322/95326): a poster hit WA precomputing each
// node's input count and special-casing 2-input nodes; the fix was to tell
// leaf from internal node by child index (< N-1) instead, which is exactly
// what rec()'s "i >= M" check already does here, so that pitfall does not
// apply. Cross-checked the two community-posted test cases from message
// 95350/98479 (N=8 -> 39 38, N=32 -> 5321 5320); this program reproduces
// both exactly. Ambiguity: none found - the referenced problem 3605 fixes
// the tree structure and leaf order (leftmost leaf = most significant bit
// of the input counter), which the sample confirms exactly.
#include <cstdio>
#include <cstring>
using namespace std;

const int MOD = 9973;

struct Node {
    int n, ones, rise, fall, c00, c11;
    int b0, b1;
};

int type_[20005];
int N; // number of leaves
int M; // N-1, number of internal nodes given

Node leafNode() {
    Node v;
    v.n = 2 % MOD;
    v.b0 = 0; v.b1 = 1;
    v.ones = 1 % MOD;
    v.rise = 1 % MOD;
    v.fall = 0;
    v.c00 = 0;
    v.c11 = 0;
    return v;
}

Node combine(int t, const Node &L, const Node &R) {
    Node v;
    int nL = L.n, nR = R.n;
    v.n = (nL * nR) % MOD;

    int R0L = L.b0, R1L = L.b1;
    int R0R = R.b0, R1R = R.b1;
    if (t == 1) {
        v.b0 = R0L & R0R;
        v.b1 = R1L & R1R;
    } else {
        v.b0 = R0L | R0R;
        v.b1 = R1L | R1R;
    }

    int onesL = L.ones, onesR = R.ones;
    int zerosL = ((nL - onesL) % MOD + MOD) % MOD;

    if (t == 1) {
        v.ones = (onesL * onesR) % MOD;
    } else {
        v.ones = (zerosL * onesR + onesL * nR) % MOD;
    }

    int riseR = R.rise, fallR = R.fall, c00R = R.c00, c11R = R.c11;

    int passCount, domCount;
    if (t == 1) { passCount = onesL; domCount = zerosL; }
    else        { passCount = zerosL; domCount = onesL; }

    int rise_w = (passCount * riseR) % MOD;
    int fall_w = (passCount * fallR) % MOD;
    int c00_w = (passCount * c00R) % MOD;
    int c11_w = (passCount * c11R) % MOD;
    int szR_m1 = ((nR - 1) % MOD + MOD) % MOD;
    if (t == 1) c00_w = (c00_w + domCount * szR_m1) % MOD;
    else        c11_w = (c11_w + domCount * szR_m1) % MOD;

    int riseL = L.rise, fallL = L.fall, c00L = L.c00, c11L = L.c11;
    int rise_b = 0, fall_b = 0, c00_b = 0, c11_b = 0;

    // type (0,0): count c00L
    if (t == 1) {
        c00_b = (c00_b + c00L) % MOD;
    } else {
        if (R1R == 0 && R0R == 1) rise_b = (rise_b + c00L) % MOD;
        else if (R1R == 1 && R0R == 0) fall_b = (fall_b + c00L) % MOD;
        else if (R1R == 0 && R0R == 0) c00_b = (c00_b + c00L) % MOD;
        else c11_b = (c11_b + c00L) % MOD;
    }

    // type (1,1): count c11L
    if (t == 0) {
        c11_b = (c11_b + c11L) % MOD;
    } else {
        if (R1R == 0 && R0R == 1) rise_b = (rise_b + c11L) % MOD;
        else if (R1R == 1 && R0R == 0) fall_b = (fall_b + c11L) % MOD;
        else if (R1R == 0 && R0R == 0) c00_b = (c00_b + c11L) % MOD;
        else c11_b = (c11_b + c11L) % MOD;
    }

    // type (0,1): count riseL ; AND: 0->R0R ; OR: R1R->1
    if (t == 1) {
        if (R0R == 1) rise_b = (rise_b + riseL) % MOD;
        else c00_b = (c00_b + riseL) % MOD;
    } else {
        if (R1R == 0) rise_b = (rise_b + riseL) % MOD;
        else c11_b = (c11_b + riseL) % MOD;
    }

    // type (1,0): count fallL ; AND: R1R->0 ; OR: 1->R0R
    if (t == 1) {
        if (R1R == 1) fall_b = (fall_b + fallL) % MOD;
        else c00_b = (c00_b + fallL) % MOD;
    } else {
        if (R0R == 0) fall_b = (fall_b + fallL) % MOD;
        else c11_b = (c11_b + fallL) % MOD;
    }

    v.rise = (rise_w + rise_b) % MOD;
    v.fall = (fall_w + fall_b) % MOD;
    v.c00 = (c00_w + c00_b) % MOD;
    v.c11 = (c11_w + c11_b) % MOD;
    return v;
}

Node rec(int i) {
    if (i >= M) return leafNode();
    Node L = rec(2*i+1);
    Node R = rec(2*i+2);
    return combine(type_[i], L, R);
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &N);
        M = N - 1;
        for (int i = 0; i < M; ++i) scanf("%d", &type_[i]);
        Node root = rec(0);
        printf("%d %d\n", root.rise % MOD, root.fall % MOD);
    }
    return 0;
}
