// POJ 1647 - One-move checkmate
// Model: claude-sonnet-5
//
// Approach: brute force over all 64 squares as a candidate destination for
// the white queen. For each candidate that is a legal queen move from its
// current square (same row/column/diagonal, path not blocked by the white
// king or the black king, destination not occupied by either king), we
// simulate the resulting position and test for checkmate:
//   1. The moved queen must attack the black king's square (a line with no
//      white king in between).
//   2. Every one of the black king's up to 8 neighboring squares must be an
//      illegal king move: off board, adjacent to (or equal to) the white
//      king's square, or attacked by the queen. The neighbor equal to the
//      queen's new square is a capture attempt, legal only if that square is
//      undefended (i.e. not adjacent to the white king).
// Among all destinations that produce checkmate we take the smallest column
// then smallest row (per the statement's output tie-break), or print "no".
//
// Statement ambiguity: the problem only wants true checkmate, not stalemate
// (the discuss board repeatedly confirms "bei-he"/stalemate does not count
// as an answer here, and that the black king is free to leave its own
// square attacked as long as it isn't check, i.e. stalemate is simply not a
// winning move). The queen must actually move (cannot stay in place), and
// the white king never moves. Both the white king and the black king can
// block the queen's line of sight; the queen itself can be captured by the
// black king if undefended by the white king.
#include <cstdio>

static int wkx, wky, wqx, wqy, bkx, bky;

// True if (qx,qy) "sees" (tx,ty) along a queen line, treating (bx,by) as a
// blocking piece (strictly between the two endpoints).
static bool attacks(int qx, int qy, int bx, int by, int tx, int ty) {
    int dx = tx - qx, dy = ty - qy;
    if (dx == 0 && dy == 0) return false;
    if (!(dx == 0 || dy == 0 || dx == dy || dx == -dy)) return false;
    int sx = (dx > 0) - (dx < 0);
    int sy = (dy > 0) - (dy < 0);
    int x = qx + sx, y = qy + sy;
    while (x != tx || y != ty) {
        if (x == bx && y == by) return false;
        x += sx; y += sy;
    }
    return true;
}

static bool onBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

static bool chebyshevAdjacentOrSame(int ax, int ay, int bx, int by) {
    int dx = ax - bx; if (dx < 0) dx = -dx;
    int dy = ay - by; if (dy < 0) dy = -dy;
    return dx <= 1 && dy <= 1;
}

// Is (qx,qy) a legal queen move from (wqx,wqy)?
static bool legalQueenMove(int qx, int qy) {
    if (qx == wqx && qy == wqy) return false; // must move
    if (qx == wkx && qy == wky) return false; // can't land on own king
    if (qx == bkx && qy == bky) return false; // can't capture the king
    int dx = qx - wqx, dy = qy - wqy;
    if (!(dx == 0 || dy == 0 || dx == dy || dx == -dy)) return false;
    int sx = (dx > 0) - (dx < 0);
    int sy = (dy > 0) - (dy < 0);
    int x = wqx + sx, y = wqy + sy;
    while (x != qx || y != qy) {
        if ((x == wkx && y == wky) || (x == bkx && y == bky)) return false;
        x += sx; y += sy;
    }
    return true;
}

// Does the queen at (qx,qy) deliver checkmate to the black king (at bkx,bky)?
static bool isCheckmate(int qx, int qy) {
    if (!attacks(qx, qy, wkx, wky, bkx, bky)) return false; // no check at all

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int ex = bkx + dx, ey = bky + dy;
            if (!onBoard(ex, ey)) continue;
            if (ex == qx && ey == qy) {
                // Attempt to capture the queen.
                if (!chebyshevAdjacentOrSame(wkx, wky, qx, qy)) {
                    return false; // undefended queen: legal escape by capture
                }
                continue; // defended: this "escape" is illegal
            }
            if (ex == wkx && ey == wky) continue; // can't land on white king
            if (chebyshevAdjacentOrSame(wkx, wky, ex, ey)) continue; // kings can't be adjacent
            if (attacks(qx, qy, wkx, wky, ex, ey)) continue; // still attacked by queen
            return false; // found a legal escape
        }
    }
    return true;
}

static bool parseSquare(char c, char d, int &x, int &y) {
    if (c < 'a' || c > 'h') return false;
    if (d < '1' || d > '8') return false;
    x = c - 'a';
    y = d - '1';
    return true;
}

int main() {
    char c1, d1, c2, d2, c3, d3;
    if (scanf(" %c%c %c%c %c%c", &c1, &d1, &c2, &d2, &c3, &d3) != 6) return 0;
    parseSquare(c1, d1, wkx, wky);
    parseSquare(c2, d2, wqx, wqy);
    parseSquare(c3, d3, bkx, bky);

    int bestX = -1, bestY = -1;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (!legalQueenMove(x, y)) continue;
            if (!isCheckmate(x, y)) continue;
            if (bestX == -1 || x < bestX || (x == bestX && y < bestY)) {
                bestX = x; bestY = y;
            }
        }
    }
    if (bestX == -1) {
        printf("no\n");
    } else {
        printf("%c%c\n", 'a' + bestX, '1' + bestY);
    }
    return 0;
}
