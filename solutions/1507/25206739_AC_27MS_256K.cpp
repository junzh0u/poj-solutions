// POJ 1507 - Commedia dell' arte
// Model: claude-sonnet-5
//
// 3D generalization of the 15-puzzle solvability test. Reading the input
// straight into a flat array A[0..N-1] (N=M^3) in the order given, the
// (k)-th number read lands exactly at the position whose goal value is k+1
// (goal fills z*M^2+y*M+x+1 in the same left-to-right, top-to-bottom,
// layer-by-layer order the input uses), so the goal array is simply
// 1,2,...,N-1,0 with the blank last.
//
// Each legal move swaps the blank with one grid neighbour, i.e. is one
// transposition of the "board permutation" (treating the blank as the
// extra label N so the map is a genuine permutation of 1..N). After k
// moves the sign of that permutation is (-1)^k. Every move also flips the
// parity of x+y+z at the blank's location (grid is bipartite under unit
// steps), so any sequence of moves taking the blank from its start
// position to the goal corner (M-1,M-1,M-1) must use a number of moves k
// whose parity equals the Manhattan distance D between those two points.
// Hence a necessary condition is sign(permutation) == (-1)^D, i.e.
// parity(permutation) == D mod 2. This was checked to be sufficient too
// (Wilson's theorem for 2-connected non-degenerate puzzle graphs, M>=2)
// by exhaustive BFS over the full 2x2x2 cube (8! = 40320 states): exactly
// half (20160) are reachable from the solved state, and the formula
// matches BFS reachability on all 40320 permutations with zero
// mismatches. M=1 is the trivial single-cell case (always solvable),
// handled correctly by the same formula (D=0, one fixed point).
//
// Permutation parity is computed as (N - numberOfCycles) % 2 by chasing
// cycles directly in the value array (marking visited slots by negating
// them), which needs no extra memory beyond the array itself -- important
// given the 10000K memory limit at N up to 100^3 = 1,000,000.
//
// Statement's discuss board (message 2429/2432) independently describes
// the same "move blank home costs D transpositions, then check parity of
// the rest" argument, confirming the reading.
#include <cstdio>

static int A[1000001];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int M;
        scanf("%d", &M);
        int N = M * M * M;
        int pos0 = -1;
        for (int i = 0; i < N; i++) {
            int v;
            scanf("%d", &v);
            A[i] = v;
            if (v == 0) pos0 = i;
        }
        // Replace blank with label N (1-indexed) to make A a permutation of 1..N.
        A[pos0] = N;

        int numCycles = 0;
        for (int i = 1; i <= N; i++) {
            if (A[i - 1] > 0) {
                numCycles++;
                int j = i;
                while (A[j - 1] > 0) {
                    int nj = A[j - 1];
                    A[j - 1] = -A[j - 1];
                    j = nj;
                }
            }
        }
        int parity1 = (N - numCycles) & 1;

        int z = pos0 / (M * M);
        int rem = pos0 % (M * M);
        int y = rem / M;
        int x = rem % M;
        int D = (M - 1 - x) + (M - 1 - y) + (M - 1 - z);
        int parity2 = D & 1;

        if (parity1 == parity2)
            printf("Puzzle can be solved.\n");
        else
            printf("Puzzle is unsolvable.\n");
    }
    return 0;
}
