// POJ 3455 - Cheesy Chess
// Model: claude-sonnet-5
//
// Approach: the state (king x,y ; pawn x,y ; whose turn) forms a finite,
// acyclic game graph -- the pawn's row strictly decreases every Black move,
// so there can be no repetition/draw and a memoized minimax over the
// roughly 8*8*8*8*2 states decides the winner directly.
//
// Rules encoded from the statement:
//  - A field is Forbidden (neither piece may stand there), fixed Dangerous
//    (pawn may stand there, king may never move there), or open.
//  - The two cells diagonally below the pawn's CURRENT position are also
//    "floating" dangerous for the king (unless they are forbidden), even if
//    they were open on the static board. This is recomputed every time the
//    king is about to move, from the pawn's position at that moment.
//  - The king must move to one of its 8 neighbors that is on the board, not
//    forbidden, and not dangerous (fixed or floating) -- EXCEPT capturing:
//    the capture square is the pawn's own static square, which is only
//    ever fixed-Dangerous or open (never floating-dangerous, since the two
//    floating squares are strictly the pawn's below-left/below-right
//    neighbors, not the pawn's own square). So a capture is legal iff the
//    pawn's square is not fixed-Dangerous. If the king has no legal move at
//    all, Black wins immediately.
//  - The pawn is forced to move straight down one row. It may move onto an
//    open or (fixed or floating -- moot, since it isn't adjacent to itself)
//    dangerous field, but not onto a forbidden field, and not onto the
//    king's current square (blocking). If it cannot move: Black wins if it
//    was already on the bottom row, otherwise White wins.
//  - The king merely standing on a square that later becomes dangerous
//    (because the pawn moved next to it) is harmless -- it only matters
//    when the king next has to choose a destination.
//
// The statement's own note (king's square may become dangerous after a
// pawn move, "not a problem") together with a board-posted example
// (king placed exactly on what becomes a floating-dangerous square next to
// the pawn) confirms the current-square-doesn't-eliminate-you reading.
//
// No ambiguity was found that the sample cases fail to resolve; the
// literal statement was implemented directly (this problem has no
// meaningfully different "brute force" to differential test against, since
// the full game tree here already IS the direct/naive solution).

#include <cstdio>
#include <cstring>

static bool forbidden[9][9];
static bool fixedDanger[9][9];
static int memo[9][9][9][9][2]; // 0 unknown, 1 White, 2 Black

static bool inBoard(int x, int y) {
    return x >= 1 && x <= 8 && y >= 1 && y <= 8;
}

static bool isDangerNow(int x, int y, int px, int py) {
    if (fixedDanger[x][y]) return true;
    if (y == py - 1 && (x == px - 1 || x == px + 1)) {
        if (!forbidden[x][y]) return true;
    }
    return false;
}

// turn: 0 = White(king) to move, 1 = Black(pawn) to move.
// returns 1 for White win, 2 for Black win.
static int solve(int kx, int ky, int px, int py, int turn) {
    int &m = memo[kx][ky][px][py][turn];
    if (m) return m;
    // mark a sentinel to be safe against accidental cycles (shouldn't happen)
    m = 0;

    if (turn == 0) {
        static const int dx[8] = {-1,-1,-1,0,0,1,1,1};
        static const int dy[8] = {-1,0,1,-1,1,-1,0,1};
        bool anyMove = false;
        for (int d = 0; d < 8; ++d) {
            int nx = kx + dx[d], ny = ky + dy[d];
            if (!inBoard(nx, ny)) continue;
            if (forbidden[nx][ny]) continue;
            if (isDangerNow(nx, ny, px, py)) continue;
            anyMove = true;
            if (nx == px && ny == py) {
                // capture -> immediate White win
                m = 1;
                return m;
            }
            int r = solve(nx, ny, px, py, 1);
            if (r == 1) { m = 1; return m; }
        }
        if (!anyMove) { m = 2; return m; }
        m = 2; // no move led to a White win
        return m;
    } else {
        if (py == 1) { m = 2; return m; } // already at bottom line -> Black wins
        int ny = py - 1;
        if (forbidden[px][ny] || (px == kx && ny == ky)) {
            // cannot move, not at bottom line -> White wins
            m = 1;
            return m;
        }
        int r = solve(kx, ky, px, ny, 0);
        m = r;
        return m;
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        char line[16];
        for (int row = 8; row >= 1; --row) {
            scanf("%s", line);
            for (int col = 1; col <= 8; ++col) {
                char c = line[col - 1];
                forbidden[col][row] = (c == 'F');
                fixedDanger[col][row] = (c == 'D');
            }
        }
        int xk, yk, xp, yp;
        scanf("%d %d", &xk, &yk);
        scanf("%d %d", &xp, &yp);
        memset(memo, 0, sizeof(memo));
        int r = solve(xk, yk, xp, yp, 0);
        printf("%s\n", r == 1 ? "White" : "Black");
    }
    return 0;
}
