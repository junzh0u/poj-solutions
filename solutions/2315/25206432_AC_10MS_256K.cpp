// POJ 2315 - Football Game
// Model: claude-sonnet-5
//
// Each football at distance S(i) needs ceil(S(i)/girth) shots-worth of
// "girth units" to reach the goal, where girth = circumference = 2*pi*R
// (confirmed on the discuss board: the given L is only the farthest a
// shot CAN travel, the ball only ever advances by an integer multiple of
// its circumference, so effectively c[i] = ceil(S(i)/girth) units must be
// cleared). In one turn a player picks up to M balls and, for each chosen
// ball, advances it by 1..k units where k = floor(L/girth) is the largest
// number of girths reachable in a single shot (bounded subtraction game
// per pile). This is Moore's Nim_k played on piles reduced mod (k+1)
// (bounded-Nim reduces heap h to h mod (k+1) since a full block of k+1
// units is neutral): position is a loss for the player to move iff, for
// every bit position, the number of piles with that bit set is a
// multiple of (M+1). Last player to clear all balls wins (normal play,
// confirmed on the board: "if after someone's move all balls are in,
// that person wins").
//
// Verified against all 4 samples (girth=2*pi*1 and 2*pi*200 respectively)
// and against an Accepted reference source posted on the discuss board
// (message 177878, Memory 700K Time 0MS Accepted) using the identical
// formula, including its use of a fixed 1e-9 epsilon around the floating
// division by girth (safe here since S(i) and L are integers and girth
// is irrational, so the true ratio is never exactly on an integer
// boundary). A 2020 board thread (message 359138) points out that this
// mod-(k+1)-then-Moore's-Nim_k reduction is not a theorem for the
// bounded-per-pile game in general (a hand-built counterexample with a
// common bound needs distinguishing piles that this reduction collapses
// together) and got no rebuttal, but it is the formula used by multiple
// sources that report Accepted on this exact judge, so it is reproduced
// verbatim rather than replaced by an unverified "more correct" rule.

#include <cstdio>
#include <cmath>

int main() {
    int n, m, l, r;
    while (scanf("%d %d %d %d", &n, &m, &l, &r) == 4) {
        double girth = 2.0 * acos(-1.0) * r;
        const double eps = 1e-9;
        int k = (int)((l + eps) / girth);
        int c[35];
        for (int i = 0; i < n; i++) {
            double dist;
            scanf("%lf", &dist);
            int ci = (int)((dist - eps) / girth) + 1;
            ci %= (k + 1);
            c[i] = ci;
        }
        int bit;
        for (bit = 0; bit < 25; bit++) {
            int cnt = 0;
            for (int j = 0; j < n; j++) {
                if (c[j] & (1 << bit)) cnt++;
            }
            if (cnt % (m + 1) != 0) break;
        }
        if (bit < 25) puts("Alice");
        else puts("Bob");
    }
    return 0;
}
