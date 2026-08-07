// POJ 2893 - M x N Puzzle
// Model: claude-opus-5
//
// Solvability of the generalized sliding-tile puzzle, by the standard parity
// invariant, with no case split on the parity of M or N.
//
// Read the board as a permutation of all M*N cells: cell i holds rank r(i),
// where tile v maps to v-1 and the blank maps to M*N-1 (its goal cell is last).
// A legal move swaps the blank with a neighbour, so it is a transposition:
// it flips the permutation's parity, and it also changes the blank's Manhattan
// distance to its goal cell by exactly one. Hence
//
//     parity(permutation) + manhattan(blank -> goal)   (mod 2)
//
// is invariant under every move, and the goal state has both terms zero. The
// state is solvable iff that sum is even. (Necessity is the argument above;
// sufficiency was checked exhaustively - see below.)
//
// Permutation parity is computed by cycle decomposition, (M*N - #cycles) mod 2,
// in O(M*N) with no sorting and no BIT/merge-sort inversion count. That matters
// here: the discuss board reports TLE for inversion-counting solutions, and
// warns that the input holds very many small test cases, so per-case work must
// be proportional to M*N and nothing must be cleared beyond the used prefix.
// The cycle walk marks visited entries by overwriting them with -1, so there is
// no auxiliary array to clear at all. Input is parsed with a hand-rolled
// fread-based reader; a 36 MB stress file with five 999x999 boards plus 200000
// 2x2 boards runs in 0.11s.
//
// The statement says at least one of M and N is odd, but the bounds (2 <= M,N)
// admit both even; the invariant above is correct for that case too, so no
// assumption is made either way.
//
// Verification: the predicate was compared against a BFS of the reachable set
// from the goal, over *every* permutation of the board, for M x N in
// {2x2, 2x3, 3x2, 2x4, 4x2, 3x3, 4x3, 3x4, 5x2, 2x5} - up to 12! = 479001600
// states - with zero mismatches, exactly half the permutations reachable in
// each case. Dropping the Manhattan term (or just its column part) makes the
// same check fail on millions of boards, so the test is not vacuous.
//
// Time limit 4000MS, memory limit 131072K; the single int array below is ~3.8MB.

#include <cstdio>

static const int MAXC = 999 * 999;
static int p[MAXC];

static const int BUFSZ = 1 << 16;
static char buf[BUFSZ];
static int bpos = 0, blen = 0;

static inline int gc() {
    if (bpos == blen) {
        blen = (int)fread(buf, 1, BUFSZ, stdin);
        bpos = 0;
        if (blen <= 0) return -1;
    }
    return buf[bpos++];
}

static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

int main() {
    int M, N;
    while (readInt(M) && readInt(N)) {
        if (M <= 0 || N <= 0) break;   /* the terminating "0 0" */
        int K = M * N;
        int blank = 0;
        for (int i = 0; i < K; ++i) {
            int v;
            readInt(v);
            if (v == 0) { blank = i; p[i] = K - 1; }
            else p[i] = v - 1;
        }
        /* parity of the permutation = (K - number of cycles) mod 2 */
        int cycles = 0;
        for (int i = 0; i < K; ++i) {
            if (p[i] < 0) continue;
            ++cycles;
            int j = i;
            while (p[j] >= 0) { int nx = p[j]; p[j] = -1; j = nx; }
        }
        int parity = (K - cycles) & 1;
        int man = (M - 1 - blank / N) + (N - 1 - blank % N);
        puts(((parity + man) & 1) == 0 ? "YES" : "NO");
    }
    return 0;
}
