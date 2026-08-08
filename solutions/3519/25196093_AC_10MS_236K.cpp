// POJ 3519 - Minimal Backgammon
// Model: claude-opus-5
//
// Forward probability DP over (square, lost-turn flag).  State (p,s) holds the
// probability that after k turns the checker sits on square p (p < N, the goal
// being absorbing) with s=1 meaning the next turn is forfeited.  Each turn every
// non-forfeit state spreads 1/6 to each roll r=1..6: q=p+r, and if q>N the
// checker retreats from the goal by the excess, i.e. q=2N-q.  Landing on N is
// success and is accumulated out of the chain; landing on a "back" square resets
// to (0,0); landing on a "lose" square gives (q,1).  A forfeit state simply
// consumes the turn and clears its flag.  Answer = success mass after T turns.
//
// Two rules the prose leaves easy to misread, both settled by Figure 2, which
// animates exactly the sample's fourth dataset (N=6, L={2}, B={5}, T=6): its
// "Turn 5 (skip)" frame shows a forfeited turn consuming one of the T turns
// rather than being free, and its Turn 2 frame moves 3 -> 4 on a roll of 5,
// confirming the retreat is measured from the goal (2N-q).  A discuss-board post
// claims the figure contradicts the text there; comparing the frames shows it
// does not, and a later reply on the board says the same.
//
// Everything is double arithmetic; no 64-bit integers are involved.

#include <cstdio>
#include <cstring>

int main() {
    int n, t, l, b;
    while (scanf("%d %d %d %d", &n, &t, &l, &b) == 4 && (n || t || l || b)) {
        bool lose[105], back[105];
        memset(lose, 0, sizeof(lose));
        memset(back, 0, sizeof(back));
        int i, j, r, x;
        for (i = 0; i < l; ++i) { scanf("%d", &x); lose[x] = true; }
        for (i = 0; i < b; ++i) { scanf("%d", &x); back[x] = true; }

        static double cur[105][2], nxt[105][2];
        memset(cur, 0, sizeof(cur));
        cur[0][0] = 1.0;
        double succ = 0.0;

        for (i = 0; i < t; ++i) {
            memset(nxt, 0, sizeof(nxt));
            for (j = 0; j < n; ++j) {
                if (cur[j][1] > 0.0) nxt[j][0] += cur[j][1];
                double p = cur[j][0];
                if (p <= 0.0) continue;
                for (r = 1; r <= 6; ++r) {
                    int q = j + r;
                    if (q > n) q = 2 * n - q;
                    double s = p / 6.0;
                    if (q == n) succ += s;
                    else if (back[q]) nxt[0][0] += s;
                    else if (lose[q]) nxt[q][1] += s;
                    else nxt[q][0] += s;
                }
            }
            memcpy(cur, nxt, sizeof(cur));
        }
        printf("%.6f\n", succ);
    }
    return 0;
}
