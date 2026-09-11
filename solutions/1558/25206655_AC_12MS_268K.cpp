// POJ 1558 - Board Silly
// Model: claude-sonnet-5
//
// Approach: for each occupied square holding the target player's piece,
// examine each of the 4 lines through it (row, column, and the two
// diagonals). For a given line, the required move distance N is the total
// count of occupied squares (either player) anywhere on that entire line
// (computed by walking to one end of the line and counting forward). The
// piece may move exactly N squares in either of the line's two directions
// (e.g. left/right for the row). A destination is valid if it stays on the
// board, no opponent piece lies strictly between origin and destination
// (own pieces may be jumped), and the destination is not occupied by the
// mover's own piece (an opponent's piece there is a capture). Collect all
// resulting "R#-R#" strings, sort lexicographically, and print, separating
// boards by a blank line and printing "No moves are possible" when a board
// yields none.
//
// No statement ambiguity found beyond the routine "count includes the
// moving piece itself" reading, confirmed against the sample: A1's row
// count is 1 (itself only) giving A1-A2, its "\" diagonal count is 2
// (A1 and G7) giving A1-C3, and its column count is 4 giving A1-E1 -
// exactly the sample's first three moves. The discuss board (message
// 5086-5088) only documents a 2004 data-correction long since applied;
// no other pitfalls reported.

#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int main(){
    string rows[8];
    string playerTok;
    bool first = true;
    while (cin >> rows[0]) {
        for (int i = 1; i < 8; i++) cin >> rows[i];
        cin >> playerTok;
        char player = playerTok[0];
        char opp = (player == 'X') ? 'O' : 'X';

        char board[8][8];
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = rows[i][j];

        // For each axis, dirs[axis][0] and dirs[axis][1] are the two
        // opposite directions along that line.
        static const int dirs[4][2][2] = {
            {{0,-1},{0,1}},   // row: left, right
            {{-1,0},{1,0}},   // column: up, down
            {{-1,-1},{1,1}},  // diagonal \: up-left, down-right
            {{-1,1},{1,-1}}   // diagonal /: up-right, down-left
        };

        vector<string> moves;

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c] != player) continue;
                for (int axis = 0; axis < 4; axis++) {
                    int dr = dirs[axis][1][0];
                    int dc = dirs[axis][1][1];
                    // Walk to the start of the line.
                    int sr = r, sc = c;
                    while (sr - dr >= 0 && sr - dr < 8 && sc - dc >= 0 && sc - dc < 8) {
                        sr -= dr; sc -= dc;
                    }
                    // Count occupied squares along the whole line.
                    int N = 0;
                    int tr = sr, tc = sc;
                    while (tr >= 0 && tr < 8 && tc >= 0 && tc < 8) {
                        if (board[tr][tc] != '.') N++;
                        tr += dr; tc += dc;
                    }
                    for (int d = 0; d < 2; d++) {
                        int ddr = dirs[axis][d][0];
                        int ddc = dirs[axis][d][1];
                        int nr = r + ddr * N;
                        int nc = c + ddc * N;
                        if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) continue;
                        bool blocked = false;
                        for (int k = 1; k < N; k++) {
                            int mr = r + ddr * k, mc = c + ddc * k;
                            if (board[mr][mc] == opp) { blocked = true; break; }
                        }
                        if (blocked) continue;
                        if (board[nr][nc] == player) continue;
                        char buf[8];
                        sprintf(buf, "%c%d-%c%d", 'A' + r, c + 1, 'A' + nr, nc + 1);
                        moves.push_back(string(buf));
                    }
                }
            }
        }

        sort(moves.begin(), moves.end());
        moves.erase(unique(moves.begin(), moves.end()), moves.end());

        if (!first) printf("\n");
        first = false;
        if (moves.empty()) {
            printf("No moves are possible\n");
        } else {
            for (size_t i = 0; i < moves.size(); i++)
                printf("%s\n", moves[i].c_str());
        }
    }
    return 0;
}
