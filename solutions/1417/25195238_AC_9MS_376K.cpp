// POJ 1417 - True Liars
// Model: claude-opus-5
//
// Divine members always tell the truth, devilish ones always lie, and question i
// is "x said y is divine", answered yes/no.  Case analysis on x's tribe:
//   x divine  -> he says yes iff y is divine
//   x devilish-> he lies, so he says yes iff y is NOT divine
// Both collapse to a pure parity fact: "yes" <=> x and y are in the same tribe,
// "no" <=> different tribes.  There is no other reading of the statement.
//
// So: weighted union-find with a parity bit relative to the root.  Every
// component then splits into two sides (parity 0 / parity 1) whose tribes are
// opposite; the only remaining freedom is which side of each component is the
// divine one.  People named in no question are singleton components (side sizes
// 1 and 0), and they must be included -- the answer is only determined when the
// whole population is pinned down.
//
// Choosing one side per component to be divine, with the total forced to p1, is
// a 0/1 knapsack.  dp[i][j] = number of ways to reach j divine using the first i
// components, saturated at 2 since only "exactly one way" matters (the true
// count can be as large as 2^598).  The data is guaranteed consistent, so
// dp[m][p1] == 1 means the classification is unique -> print it; dp[m][p1] == 0
// (impossible) or >= 2 (ambiguous) both print "no".  Backtracking is
// unambiguous: since the total is 1, exactly one of the two branches out of any
// state on the path carries the count 1.
//
// Note the terminator is the line "0 0 0" specifically, not merely n == 0:
// "0 1 0" is a legal data set (one inhabitant, no questions, answer "1 end"),
// and so is "0 0 1" (answer is just "end", with no ids printed).
//
// n < 1000, p1, p2 < 300, so at most 598 people/components and the knapsack is
// 598 x 300 -- trivially inside the limits.

#include <cstdio>

const int MAXP = 2005;   /* ids are stated to be 1..p1+p2 (<600); padded */
const int MAXQ = 1005;   /* n < 1000 */
const int MAXC = 605;    /* components <= number of people */
const int MAXD = 305;    /* p1 < 300 */

int par[MAXP], rel[MAXP], compOf[MAXP];

static int find_(int x) {
    if (par[x] == x) return x;
    int r = find_(par[x]);
    rel[x] ^= rel[par[x]];
    par[x] = r;
    return r;
}

int qx[MAXQ], qy[MAXQ], qd[MAXQ];
int l0[MAXC], l1[MAXC];
int dp[MAXC][MAXD];
int pick[MAXC];

int main() {
    int n, p1, p2;
    while (scanf("%d %d %d", &n, &p1, &p2) == 3) {
        if (n == 0 && p1 == 0 && p2 == 0) break;
        int tot = p1 + p2;
        int hi = tot, i, j;
        for (i = 0; i < n; i++) {
            char w[16];
            scanf("%d %d %15s", &qx[i], &qy[i], w);
            qd[i] = (w[0] == 'n') ? 1 : 0;          /* 1 = different tribes */
            if (qx[i] < 0 || qx[i] >= MAXP) qx[i] = 0;
            if (qy[i] < 0 || qy[i] >= MAXP) qy[i] = 0;
            if (qx[i] > hi) hi = qx[i];
            if (qy[i] > hi) hi = qy[i];
        }
        for (i = 0; i <= hi; i++) { par[i] = i; rel[i] = 0; compOf[i] = 0; }
        for (i = 0; i < n; i++) {
            int a = find_(qx[i]), b = find_(qy[i]);
            if (a != b) { par[b] = a; rel[b] = rel[qx[i]] ^ rel[qy[i]] ^ qd[i]; }
            /* a == b: the fact is already implied (input is consistent) */
        }
        int m = 0;
        for (i = 1; i <= tot; i++) {
            int r = find_(i);
            int c = compOf[r];
            if (c == 0) { c = ++m; compOf[r] = c; l0[c] = l1[c] = 0; }
            if (rel[i] == 0) l0[c]++; else l1[c]++;
        }
        for (j = 0; j <= p1; j++) dp[0][j] = 0;
        dp[0][0] = 1;
        for (i = 1; i <= m; i++) {
            for (j = 0; j <= p1; j++) {
                int v = 0;
                if (j >= l0[i]) v += dp[i - 1][j - l0[i]];
                if (j >= l1[i]) v += dp[i - 1][j - l1[i]];
                dp[i][j] = (v > 2) ? 2 : v;
            }
        }
        if (dp[m][p1] != 1) { printf("no\n"); continue; }
        j = p1;
        for (i = m; i >= 1; i--) {
            if (j >= l0[i] && dp[i - 1][j - l0[i]] == 1) { pick[i] = 0; j -= l0[i]; }
            else { pick[i] = 1; j -= l1[i]; }
        }
        for (i = 1; i <= tot; i++)
            if (pick[compOf[find_(i)]] == rel[i]) printf("%d\n", i);
        printf("end\n");
    }
    return 0;
}
