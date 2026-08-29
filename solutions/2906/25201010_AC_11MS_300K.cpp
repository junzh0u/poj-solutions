// POJ 2906 - Moving Pianos
// Model: claude-sonnet-5
//
// Each piano move order [b_i, e_i] must be assigned to exactly one day in
// that range; each day has a capacity cap[d] = floor(p/2) piano moves (two
// tuners per piano, at most p tuners per day). This is bipartite b-matching
// between jobs and days where each job's feasible-day set is a contiguous
// interval, so the incidence matrix has the consecutive-ones property and
// Hall's theorem reduces to checking only contiguous day windows [l,r]:
// feasible iff for every 1<=l<=r<=100,
//     sum_{d=l}^{r} cap[d]  >=  #{jobs with b_i>=l and e_i<=r}.
// (Any job not fully contained in [l,r] has an alternative day outside the
// window, so only fully-contained jobs are forced demand on the window.)
// This matches the board's confirmed max-flow model (source -> day with
// capacity p/2, day chain edges for [b_i,e_i), day -> sink for the job's
// e_i) since max-flow == m iff this Hall condition holds for all windows.
//
// We check the condition twice: once with cap[d]=p/2 for every day 1..100
// (all tuners work every day) -> if infeasible: "serious trouble"; then
// with cap[d]=0 on Saturdays/Sundays (day 1 is Monday, so weekend days are
// d%7==6 or d%7==0) -> if infeasible: "weekend work", else "fine".
//
// f(l,r) = #{jobs with b_i>=l, e_i<=r} is computed once per scenario via a
// cntEq[b][e] histogram and byB[l][r] = #{jobs with b_i=l, e_i<=r}
// (row-prefix sums over e). Then for fixed r, summing byB[l][r] for l from
// r down to 1 gives f(l,r) incrementally, so checking a whole cap array is
// O(D^2) with D=100 -- independent of m and n's sizes.

#include <cstdio>
#include <cstring>

static const int D = 100;
int byB[D + 2][D + 2];      // byB[l][r] = #jobs with b=l, e<=r  (1<=l<=r<=D)
int cntEq[D + 2][D + 2];    // cntEq[b][e]

bool isWeekend(int d) {
    int w = d % 7; // day1=Mon(1) ... day6=Sat(6), day7=Sun(0)
    return w == 6 || w == 0;
}

// checks Hall condition for the given per-day capacity array (1-indexed 1..D)
bool feasible(int cap[D + 2]) {
    int capPrefix[D + 2];
    capPrefix[0] = 0;
    for (int d = 1; d <= D; d++) capPrefix[d] = capPrefix[d - 1] + cap[d];

    for (int r = 1; r <= D; r++) {
        int sumF = 0;
        for (int l = r; l >= 1; l--) {
            sumF += byB[l][r];
            int capSum = capPrefix[r] - capPrefix[l - 1];
            if (sumF > capSum) return false;
        }
    }
    return true;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int m, p;
        scanf("%d %d", &m, &p);
        memset(cntEq, 0, sizeof(cntEq));
        for (int i = 0; i < m; i++) {
            int b, e;
            scanf("%d %d", &b, &e);
            cntEq[b][e]++;
        }
        // build byB[l][r] = sum_{e=l}^{r} cntEq[l][e]
        for (int l = 1; l <= D; l++) {
            int running = 0;
            for (int r = l; r <= D; r++) {
                running += cntEq[l][r];
                byB[l][r] = running;
            }
        }

        int cap = p / 2;

        int capAll[D + 2];
        for (int d = 1; d <= D; d++) capAll[d] = cap;

        if (!feasible(capAll)) {
            printf("serious trouble\n");
            continue;
        }

        int capWeekday[D + 2];
        for (int d = 1; d <= D; d++) capWeekday[d] = isWeekend(d) ? 0 : cap;

        if (!feasible(capWeekday)) {
            printf("weekend work\n");
        } else {
            printf("fine\n");
        }
    }
    return 0;
}
