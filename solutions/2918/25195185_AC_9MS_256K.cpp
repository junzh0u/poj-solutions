// POJ 2918 - Tudoku
// Model: claude-opus-5
//
// Plain Sudoku solving.  The statement promises every board is reducible by the
// "a unit already holds eight numbers, so fill the ninth" rule, but a general
// solver subsumes that rule and costs nothing here, so it is what we run:
// backtracking over bitmask candidate sets with minimum-remaining-values cell
// selection.  On a board that the naked-single rule alone solves, MRV always
// finds a cell with exactly one candidate and the search never branches; on
// anything harder it still terminates with a correct grid.
//
// Sets rowMask/colMask/boxMask hold the digits already used in each unit.  A
// cell's candidates are the complement of the union of its three units, so the
// search picks the empty cell with the fewest candidates and tries each.
//
// I/O: first line is the scenario count; each scenario is nine 9-digit lines
// with 0 for an empty cell, separated by blank lines.  Reading with scanf("%s")
// skips the blank lines for free.  Output is "Scenario #i:", the nine solved
// lines, then a blank line after every scenario including the last.

#include <cstdio>

static int grid[9][9];
static int rowMask[9], colMask[9], boxMask[9];

static int popcount9(int m) {
    int c = 0;
    while (m) { m &= m - 1; ++c; }
    return c;
}

static bool solve() {
    int bestR = -1, bestC = -1, bestCand = 0, bestCount = 10;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (grid[r][c] != 0) continue;
            int b = (r / 3) * 3 + c / 3;
            int used = rowMask[r] | colMask[c] | boxMask[b];
            int cand = (~used) & 0x3FE; /* bits 1..9 */
            int n = popcount9(cand);
            if (n == 0) return false;
            if (n < bestCount) {
                bestCount = n;
                bestCand = cand;
                bestR = r;
                bestC = c;
                if (n == 1) goto chosen;
            }
        }
    }
chosen:
    if (bestR < 0) return true; /* no empty cell left */

    {
        int b = (bestR / 3) * 3 + bestC / 3;
        for (int d = 1; d <= 9; ++d) {
            int bit = 1 << d;
            if (!(bestCand & bit)) continue;
            grid[bestR][bestC] = d;
            rowMask[bestR] |= bit;
            colMask[bestC] |= bit;
            boxMask[b] |= bit;
            if (solve()) return true;
            rowMask[bestR] &= ~bit;
            colMask[bestC] &= ~bit;
            boxMask[b] &= ~bit;
            grid[bestR][bestC] = 0;
        }
    }
    return false;
}

int main() {
    int scenarios;
    if (scanf("%d", &scenarios) != 1) return 0;
    for (int t = 1; t <= scenarios; ++t) {
        for (int i = 0; i < 9; ++i) rowMask[i] = colMask[i] = boxMask[i] = 0;
        char line[64];
        for (int r = 0; r < 9; ++r) {
            if (scanf("%s", line) != 1) return 0;
            for (int c = 0; c < 9; ++c) {
                int d = line[c] - '0';
                grid[r][c] = d;
                if (d != 0) {
                    int bit = 1 << d;
                    rowMask[r] |= bit;
                    colMask[c] |= bit;
                    boxMask[(r / 3) * 3 + c / 3] |= bit;
                }
            }
        }
        solve();
        printf("Scenario #%d:\n", t);
        for (int r = 0; r < 9; ++r) {
            char out[10];
            for (int c = 0; c < 9; ++c) out[c] = (char)('0' + grid[r][c]);
            out[9] = '\0';
            printf("%s\n", out);
        }
        printf("\n");
    }
    return 0;
}
