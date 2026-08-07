// POJ 2585 - Window Pains
// Model: claude-opus-5
//
// Nine 2x2 windows sit on a 4x4 screen; window k (1..9) has its top-left
// corner at row (k-1)/3, column (k-1)%3, so it covers the four cells
// (r,c),(r,c+1),(r+1,c),(r+1,c+1).  Bringing a window to the front puts it
// above every window it overlaps, so any sequence of raises leaves a total
// z-order, and each cell displays the topmost window covering it.
//
// Hence the screen is realizable exactly when the constraints it imposes on
// that z-order are consistent: for every cell, the window w printed there is
// above every other window v that also covers the cell.  Add an edge v -> w
// ("v is below w") for each such pair and the screen is CLEAN iff the
// resulting 9-node digraph is acyclic (a topological order is a valid
// z-order, bottom to top, and raising the windows in that order builds it).
// A cycle means no ordering can satisfy all cells.  Cycle detection here is
// Floyd-Warshall transitive closure over 9 nodes.
//
// Note: a pairwise "two windows show through each other's overlap" test is
// NOT enough - the discuss board's 1 2 2 3 / 4 2 6 6 / 4 4 8 6 / 7 8 8 9 is
// broken only through the 4-cycle 2 > 4 > 8 > 6 > 2, with no 2-cycle.
//
// Input is read to EOF-style sentinel: blocks of START, four rows of four
// numbers, END, terminated by a line ENDOFINPUT.  Defensively, a cell whose
// number belongs to a window that cannot cover it is reported BROKEN (the
// statement promises this never happens).

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

int main() {
    string tok;
    while (cin >> tok) {
        if (tok == "ENDOFINPUT") break;
        if (tok != "START") continue;

        int g[4][4];
        int i, j, k;
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j) cin >> g[i][j];
        cin >> tok; /* END */

        bool below[9][9];
        memset(below, 0, sizeof(below));
        bool ok = true;

        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                int w = g[i][j] - 1;
                if (w < 0 || w > 8) { ok = false; continue; }
                int wr = w / 3, wc = w % 3;
                if (!(wr == i || wr + 1 == i) || !(wc == j || wc + 1 == j)) {
                    ok = false;
                    continue;
                }
                for (k = 0; k < 9; ++k) {
                    if (k == w) continue;
                    int kr = k / 3, kc = k % 3;
                    if ((kr == i || kr + 1 == i) && (kc == j || kc + 1 == j))
                        below[k][w] = true;
                }
            }
        }

        if (ok) {
            for (k = 0; k < 9; ++k)
                for (i = 0; i < 9; ++i)
                    if (below[i][k])
                        for (j = 0; j < 9; ++j)
                            if (below[k][j]) below[i][j] = true;
            for (i = 0; i < 9; ++i)
                if (below[i][i]) ok = false;
        }

        printf(ok ? "THESE WINDOWS ARE CLEAN\n" : "THESE WINDOWS ARE BROKEN\n");
    }
    return 0;
}
