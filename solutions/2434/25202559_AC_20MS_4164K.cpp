// POJ 2434 - Waves
// Model: claude-sonnet-5
//
// Each pebble at (X,Y) dropped at time T emits a "raised" wave whose
// diamond (L1) ring of radius (t-T) has depth +1 at time t, and a
// "lowered" wave two seconds later with radius (t-(T+2)) at depth -1.
// The x-axis is bounded by (up to) two vertical bank lines; a wave
// reflects off a bank instead of crossing it.  Because L1 distance
// splits additively into |dx|+|dy|, the y-component is completely free
// (no walls in y), and the reflected x-component of each of the two
// "L1 ring extremes" (one that initially heads left, one that heads
// right, from the pebble) can be simulated as a single point bouncing
// at speed 1, turning around whenever it would land exactly on a bank
// cell (banks are solid single columns, never occupied by water).
//
// So for each pebble we simulate, step by step up to the needed
// elapsed time e (<=R<=500000), the position of the "left mover" and
// "right mover" trajectories.  For a target cell (tx,ty) we need
// d = e - |ty-Y| (the x-distance still to be covered); if d==0 the two
// movers coincide at the pebble itself (count once), otherwise each
// mover independently may or may not be sitting at tx (count each
// match, they are physically distinct ring points that can coincide).
//
// Reflection rule per step: a mover's tentative next cell can land
// exactly on a bank column; it then bounces, staying on the side it
// approached from (direction reverses, position becomes bank+1 if
// coming from the right / channel side, or bank-1 if coming from the
// left / outer side) -- this must check the *approach* direction, not
// just which bank was hit, since a pebble outside both banks must
// bounce back into its own outer region rather than cross into the
// channel between them.
//
// This was cross-checked against the worked "wave hitting a bank"
// diagram in the statement (single-bank case, 5 explicit time steps,
// all 7 rows) and matches exactly, including the one-step "reflection
// pause" the diagram shows adjacent to the bank.  A first version only
// handled the "approaching from the channel side" case for each bank
// (the only case the single-bank diagram exercises, since the example
// pebble always sits on the channel side of its one bank); an outer-
// zone differential test caught the missing symmetric case, where a
// wave from outside both banks must reflect back into its own outer
// region instead of leaking into the channel. After the fix, a
// pebble strictly between the banks bounces with the expected period
// 2*(B2-B1-1), and an outer-zone pebble bounces at most once and then
// escapes to infinity, confirmed by direct trajectory inspection.
//
// Complexity: O(P*R) <= 5*500000 simulation steps, trivial.

#include <cstdio>
#include <cstdlib>

static const int MAXE = 500001;
static int leftPos[MAXE];
static int rightPos[MAXE];
static int grid[9][9]; // grid[x+4][y+4]

int main(){
    int P, B1, B2, R;
    if (scanf("%d %d %d %d", &P, &B1, &B2, &R) != 4) return 0;
    int lo = B1 < B2 ? B1 : B2;
    int hi = B1 < B2 ? B2 : B1;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = 0;

    for (int p = 0; p < P; p++) {
        int X, Y, T;
        scanf("%d %d %d", &X, &Y, &T);
        int eRaise = R - T;
        if (eRaise < 0) continue; // pebble not dropped yet, no effect at all
        int eLower = eRaise - 2; // may be negative -> lowered wave hasn't started

        int maxE = eRaise;
        leftPos[0] = X;
        rightPos[0] = X;
        int lpos = X, ldir = -1;
        int rpos = X, rdir = 1;
        for (int s = 1; s <= maxE; s++) {
            int ltent = lpos + ldir;
            if (ltent == lo) {
                if (ldir == -1) { lpos = lo + 1; ldir = 1; }  // approached from the channel/right side: stay right
                else { lpos = lo - 1; ldir = -1; }            // approached from the outer/left side: stay left
            } else if (ltent == hi) {
                if (ldir == 1) { lpos = hi - 1; ldir = -1; }  // approached from the channel/left side: stay left
                else { lpos = hi + 1; ldir = 1; }             // approached from the outer/right side: stay right
            } else {
                lpos = ltent;
            }
            leftPos[s] = lpos;

            int rtent = rpos + rdir;
            if (rtent == lo) {
                if (rdir == -1) { rpos = lo + 1; rdir = 1; }
                else { rpos = lo - 1; rdir = -1; }
            } else if (rtent == hi) {
                if (rdir == 1) { rpos = hi - 1; rdir = -1; }
                else { rpos = hi + 1; rdir = 1; }
            } else {
                rpos = rtent;
            }
            rightPos[s] = rpos;
        }

        for (int tx = -4; tx <= 4; tx++) {
            for (int ty = -4; ty <= 4; ty++) {
                int dy0 = ty - Y;
                if (dy0 < 0) dy0 = -dy0;

                int d = eRaise - dy0;
                if (d >= 0) {
                    if (d == 0) {
                        if (X == tx) grid[tx+4][ty+4] += 1;
                    } else {
                        if (leftPos[d] == tx) grid[tx+4][ty+4] += 1;
                        if (rightPos[d] == tx) grid[tx+4][ty+4] += 1;
                    }
                }
                if (eLower >= 0) {
                    int d2 = eLower - dy0;
                    if (d2 >= 0) {
                        if (d2 == 0) {
                            if (X == tx) grid[tx+4][ty+4] -= 1;
                        } else {
                            if (leftPos[d2] == tx) grid[tx+4][ty+4] -= 1;
                            if (rightPos[d2] == tx) grid[tx+4][ty+4] -= 1;
                        }
                    }
                }
            }
        }
    }

    for (int ty = 4; ty >= -4; ty--) {
        for (int tx = -4; tx <= 4; tx++) {
            if (tx == lo || tx == hi) {
                putchar('X');
            } else {
                int d = grid[tx+4][ty+4];
                putchar(d < 0 ? 'o' : (d == 0 ? '-' : '*'));
            }
        }
        putchar('\n');
    }
    return 0;
}
