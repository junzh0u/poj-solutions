// POJ 2379 - ACM Rank Table
// Model: claude-opus-5
//
// Straight ICPC scoreboard simulation.
//
// For each (team, problem): find the first accepted run in *time* order and
// charge t_ac seconds plus 20 minutes (1200 s) for every run of that problem
// submitted before it.  Runs at or after the first accept are ignored entirely
// (a team may keep submitting after an AC), and an unsolved problem contributes
// nothing at all -- neither time nor penalty.
//
// Ranking: more problems first, then smaller total time, then smaller team
// number.  The statement's rule 6 ("the time shown is in minutes, the actual
// time is measured to the precision of 1 second, and the seconds are taken into
// account when ranking teams") is what settles the units: accumulate raw
// seconds and never truncate to whole minutes.  The official sample cannot
// settle it -- it gives 2 1 3 under both readings -- but a case like
//   2 2 / 1 1 119 1 / 2 1 65 1
// does: seconds rank team 2 first, minute truncation ties both at minute 1 and
// would print team 1 first.  Rule 6 says seconds win.
//
// Ambiguity: the input is NOT sorted by submission time (confirmed on the
// discuss board with a reordered copy of the official sample that keeps the
// same answer), so the runs are sorted before scanning.  Since the input order
// therefore carries no meaning, a rejected run sharing a timestamp with the
// accepting one is taken literally as not "before" it: ties on t put accepted
// runs first, so such a run is ignored rather than charged.  That makes the
// result independent of the order the runs happen to be listed in.
//
// Bounds: total <= 1000*36000 + 1000*1200 < 4e7, so plain int suffices.
// Input is read to EOF so a concatenation of data sets would also work.

#include <cstdio>
#include <algorithm>

struct Run {
    int c, p, t, r;
};

static bool byTime(const Run &a, const Run &b) {
    if (a.t != b.t) return a.t < b.t;
    return a.r > b.r;
}

static const int MAXC = 1005;
static const int MAXP = 25;
static const int MAXRUNS = 1005;

static Run runs[MAXRUNS];
static bool solved[MAXC][MAXP];
static int wrong[MAXC][MAXP];
static int nsolved[MAXC];
static int total[MAXC];
static int order[MAXC];

static bool byRank(int a, int b) {
    if (nsolved[a] != nsolved[b]) return nsolved[a] > nsolved[b];
    if (total[a] != total[b]) return total[a] < total[b];
    return a < b;
}

int main() {
    int C, N;
    while (scanf("%d %d", &C, &N) == 2) {
        int i, j;
        for (i = 0; i < MAXC; ++i) {
            nsolved[i] = 0;
            total[i] = 0;
            for (j = 0; j < MAXP; ++j) {
                solved[i][j] = false;
                wrong[i][j] = 0;
            }
        }
        int n = 0;
        for (i = 0; i < N; ++i) {
            Run cur;
            if (scanf("%d %d %d %d", &cur.c, &cur.p, &cur.t, &cur.r) != 4) break;
            if (n < MAXRUNS &&
                cur.c >= 1 && cur.c < MAXC && cur.p >= 1 && cur.p < MAXP) {
                runs[n++] = cur;
            }
        }
        std::stable_sort(runs, runs + n, byTime);
        for (i = 0; i < n; ++i) {
            int c = runs[i].c, p = runs[i].p;
            if (solved[c][p]) continue;          /* runs after the first AC do not count */
            if (runs[i].r == 1) {
                solved[c][p] = true;
                ++nsolved[c];
                total[c] += runs[i].t + 1200 * wrong[c][p];
            } else {
                ++wrong[c][p];                   /* charged only if the problem is later solved */
            }
        }
        for (i = 0; i < C; ++i) order[i] = i + 1;
        std::stable_sort(order, order + C, byRank);
        for (i = 0; i < C; ++i) {
            if (i) putchar(' ');
            printf("%d", order[i]);
        }
        putchar('\n');
    }
    return 0;
}
