// POJ 1920 - Towers of Hanoi
// Model: claude-opus-5
//
// A legal Hanoi configuration is given (each peg sorted, largest at the bottom);
// gather every disk on one peg in the fewest moves. Report the peg and the move
// count mod 1000000.
//
// Let cost(i, t) be the minimum number of moves that puts disks 1..i on peg t.
// Disk i is the largest of those, so nothing may ever sit under it and nothing
// constrains the disks above it:
//   - disk i already on t: it never has to move, so cost(i, t) = cost(i-1, t).
//   - disk i on p != t: it must move, and to move it every smaller disk has to
//     be off both p and t, i.e. all on the third peg o. Optimal is to move it
//     exactly once: cost(i, t) = cost(i-1, o) + 1 + (2^(i-1) - 1)
//                              = cost(i-1, o) + 2^(i-1).
//
// The target peg needs no comparison of huge numbers: aiming at the peg holding
// the largest disk N costs at most 2^0 + ... + 2^(N-2) = 2^(N-1) - 1, while
// either other peg costs 2^(N-1) plus more. So that peg is the strict optimum
// (confirmed unique in all 177147 configurations up to N = 11 by BFS).
//
// So: walk disks from largest down to fix each one's target peg, then sum
// 2^(i-1) mod 1000000 over the disks that sit off their target. O(N), no
// bignum, no recursion (the board reports recursive solutions overflowing the
// stack; this uses none).
//
// Hardening beyond the statement: cases are read to EOF, disks are indexed by
// their rank among the sizes actually present rather than assuming exactly
// {1..N}, and out-of-range sizes are dropped instead of scribbling on memory.
//
// Verification: the emitted binary was compared against a BFS over the raw move
// rules for every one of the 29523 configurations with N <= 9, fed as
// consecutive cases in one file, and against exact bignum arithmetic on
// N = 100000 inputs. Five mutants (swapped target branches, wrong third peg,
// 2^i instead of 2^(i-1), dropped +1, peg of the smallest disk) are all killed
// by the official sample as well as by the exhaustive check.

#include <cstdio>

const int CAP = 200000;   /* highest disk size accepted, 2x the stated bound */
const int MOD = 1000000;

int pos[CAP + 2];
char present[CAP + 2];
int order[CAP + 2];
char tgt[CAP + 2];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n <= 0) break;
        int s[3];
        if (scanf("%d %d %d", &s[0], &s[1], &s[2]) != 3) break;
        int maxsize = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < s[i]; ++j) {
                int m;
                if (scanf("%d", &m) != 1) { m = 0; }
                if (m >= 1 && m <= CAP) {
                    pos[m] = i + 1;
                    present[m] = 1;
                    if (m > maxsize) maxsize = m;
                }
            }
        }
        int k = 0;
        for (int v = 1; v <= maxsize; ++v)
            if (present[v]) order[k++] = v;

        if (k == 0) {
            printf("1\n0\n");
            continue;
        }
        int best = pos[order[k - 1]];
        tgt[k - 1] = (char)best;
        for (int i = k - 2; i >= 0; --i) {
            int above = pos[order[i + 1]];
            if (above == tgt[i + 1]) tgt[i] = tgt[i + 1];
            else tgt[i] = (char)(6 - above - tgt[i + 1]);
        }
        int ans = 0, p = 1;
        for (int i = 0; i < k; ++i) {
            if (pos[order[i]] != tgt[i]) ans = (ans + p) % MOD;
            p = (p * 2) % MOD;
        }
        printf("%d\n%d\n", best, ans);

        for (int v = 1; v <= maxsize; ++v) present[v] = 0;
    }
    return 0;
}
