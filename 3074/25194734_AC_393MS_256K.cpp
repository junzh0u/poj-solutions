// POJ 3074 - Sudoku
// Model: claude-sonnet-5
// Approach: backtracking with bitmask row/col/3x3-box constraints and a
// minimum-remaining-values (MRV) heuristic: at every recursive step scan all
// still-empty cells, compute each one's candidate mask from the three
// bitmasks, and branch on the cell with the fewest candidates (stopping the
// scan early once a 0- or 1-candidate cell is found, since nothing beats
// that). This "naked singles first" ordering is what makes plain DFS fast
// enough here -- fixed row-major order backtracking is known from the
// problem's discuss board to TLE on the harder inputs. Each test line has a
// guaranteed unique solution, so the first completion found is printed.
#include <cstdio>
#include <cstring>

char grid[9][9];
int rowmask[9], colmask[9], boxmask[9];

inline int boxid(int r, int c) { return (r / 3) * 3 + (c / 3); }

inline int popcount(int x) {
    int cnt = 0;
    while (x) { x &= x - 1; cnt++; }
    return cnt;
}

bool solve() {
    int br = -1, bc = -1, bestmask = 0, bestcnt = 10;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (grid[r][c] == '.') {
                int mask = (~(rowmask[r] | colmask[c] | boxmask[boxid(r, c)])) & 0x1FF;
                int cnt = popcount(mask);
                if (cnt < bestcnt) {
                    bestcnt = cnt; br = r; bc = c; bestmask = mask;
                    if (cnt <= 1) { r = 9; break; }
                }
            }
        }
    }
    if (br == -1) return true;   // no empty cells left: solved
    if (bestcnt == 0) return false; // dead end: some cell has no candidates

    for (int d = 0; d < 9; d++) {
        int bit = 1 << d;
        if (bestmask & bit) {
            grid[br][bc] = char('1' + d);
            rowmask[br] |= bit; colmask[bc] |= bit; boxmask[boxid(br, bc)] |= bit;
            if (solve()) return true;
            grid[br][bc] = '.';
            rowmask[br] &= ~bit; colmask[bc] &= ~bit; boxmask[boxid(br, bc)] &= ~bit;
        }
    }
    return false;
}

int main() {
    char line[100];
    while (scanf("%s", line) == 1) {
        if (strcmp(line, "end") == 0) break;
        memset(rowmask, 0, sizeof(rowmask));
        memset(colmask, 0, sizeof(colmask));
        memset(boxmask, 0, sizeof(boxmask));
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                char ch = line[r * 9 + c];
                grid[r][c] = ch;
                if (ch != '.') {
                    int bit = 1 << (ch - '1');
                    rowmask[r] |= bit; colmask[c] |= bit; boxmask[boxid(r, c)] |= bit;
                }
            }
        }
        solve();
        for (int r = 0; r < 9; r++)
            for (int c = 0; c < 9; c++)
                putchar(grid[r][c]);
        putchar('\n');
    }
    return 0;
}
