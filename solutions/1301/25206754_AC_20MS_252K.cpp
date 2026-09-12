// POJ 1301 - The Umbrella Problem: 2054
// Model: claude-sonnet-5
//
// Approach: simulate turn by turn. Turn t (1-indexed) moves the Lemming
// from row t-1 to row t. The laser direction cycles with period 4:
// turn%4==1 -> up, 2 -> right, 3 -> down, 0 -> left. For each turn,
// recompute (fresh, not accumulated) the set of squares swept by every
// gun's beam in that turn's direction. Before moving, a surviving state
// must not currently sit on a beam square (per the board's own note:
// "before jumping, first check whether the current position is safe" -
// a beam fired into the Lemming's occupied square kills him even though
// he is about to move away). Then each of the up-to-three destination
// columns (c-1, c, c+1) is valid only if in bounds, not a gun square,
// not a lava square, and not swept by this turn's beam. Reaching the
// last row on a grass square that survives this check is a win.
// Track reachable columns per row as a boolean array; grid is tiny
// (x,y < 10, <=100 datasets) so brute-force simulation is trivial.
//
// Statement ambiguity: none beyond the "check current square before
// moving" rule above, which the discuss board explicitly confirms is
// the common WA pitfall. The worked example in the statement itself
// (gun at (3,2) in a 5x7 grid) was used to verify the beam-direction
// formulas exactly (up/right/down/left square sets) before coding.

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string tok;
    while (cin >> tok) {
        if (tok == "ENDOFINPUT") break;
        // tok == "START"
        int x, y;
        cin >> x >> y;
        vector<string> grid(y);
        for (int i = 0; i < y; i++) cin >> grid[i];
        string endtok;
        cin >> endtok; // "END"

        int lc = -1;
        for (int c = 0; c < x; c++)
            if (grid[0][c] == 'L') lc = c;

        vector<pair<int, int> > guns; // (col, row)
        for (int r = 0; r < y; r++)
            for (int c = 0; c < x; c++)
                if (grid[r][c] == 'S') guns.push_back(make_pair(c, r));

        vector<bool> reach(x, false);
        reach[lc] = true;
        bool success = false;

        for (int t = 1; t <= y - 1 && !success; t++) {
            int dir = (t - 1) % 4; // 0 up, 1 right, 2 down, 3 left
            vector<vector<bool> > unsafe(y, vector<bool>(x, false));
            for (size_t i = 0; i < guns.size(); i++) {
                int gc = guns[i].first, gr = guns[i].second;
                if (dir == 0) {
                    for (int r = 0; r < gr; r++) unsafe[r][gc] = true;
                } else if (dir == 1) {
                    for (int c = gc + 1; c < x; c++) unsafe[gr][c] = true;
                } else if (dir == 2) {
                    for (int r = gr + 1; r < y; r++) unsafe[r][gc] = true;
                } else {
                    for (int c = 0; c < gc; c++) unsafe[gr][c] = true;
                }
            }

            int r = t - 1;
            int r2 = t;
            vector<bool> newreach(x, false);
            for (int c = 0; c < x; c++) {
                if (!reach[c]) continue;
                if (unsafe[r][c]) continue; // dies this turn regardless of move
                for (int dc = -1; dc <= 1; dc++) {
                    int c2 = c + dc;
                    if (c2 < 0 || c2 >= x) continue;
                    if (unsafe[r2][c2]) continue;
                    if (grid[r2][c2] == 'S' || grid[r2][c2] == 'P') continue;
                    if (r2 == y - 1 && grid[r2][c2] == 'G') {
                        success = true;
                    }
                    newreach[c2] = true;
                }
            }
            reach = newreach;
        }

        cout << (success ? "FERRET" : "GARRET") << "\n";
    }
    return 0;
}
