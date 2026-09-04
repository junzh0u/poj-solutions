// POJ 2901 - Hotel
// Model: claude-sonnet-5
//
// Approach: choose, for each room, one of {unused, male-room, female-room,
// couple-room}. A male/female room contributes up to its capacity toward the
// male/female headcount at its flat price (paid once regardless of fill, so
// WLOG an opened room is credited its full capacity -- overfilling costs
// nothing extra and is always at least as good). A couple-room (capacity
// >= 2) houses exactly one married pair, contributing 1 to each of the
// male/female headcount. Identity of who is married never matters: it is
// optional to combine a couple, so only the count c of available couples
// matters, matching the sample input, which gives no per-couple data.
//
// Key simplification (confirmed on the discuss board, message 128363/176070):
// an optimal solution never needs more than ONE room used as a couple-room.
// Splitting demand across two couple-rooms is always dominated by folding
// one of the two couples' people back into ordinary male/female rooms
// instead (their male can join whichever male-room grouping is being used,
// same for the female), so the couple option only ever needs a single
// use/no-use bit, not an exact count against c.
//
// This gives a 0/1 knapsack DP with state (male credit achieved, capped at
// m; female credit achieved, capped at f; whether a couple-room has been
// used yet) over the r rooms, each usable in at most one role, in
// O(r*m*f) time and O(m*f) memory. The couple transition is only enabled
// when c >= 1. Overshooting m or f is free, so credits are clamped.
//
// Ambiguity: the statement never lists which participants are married; the
// sample confirms there is no such list, consistent with couples being
// anonymous/interchangeable and combination being optional.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static const int MAXN = 505;
static const int INF = 1000000000;

static int cost0[MAXN][MAXN]; // no couple-room used yet
static int cost1[MAXN][MAXN]; // exactly one couple-room already used

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int m, f, r, c;
        scanf("%d %d %d %d", &m, &f, &r, &c);
        vector<pair<int,int> > rooms(r);
        for (int i = 0; i < r; i++) scanf("%d %d", &rooms[i].first, &rooms[i].second);

        for (int j = 0; j <= m; j++)
            for (int k = 0; k <= f; k++) { cost0[j][k] = INF; cost1[j][k] = INF; }
        cost0[0][0] = 0;

        bool coupleAllowed = (c >= 1);

        for (int idx = 0; idx < r; idx++) {
            int bi = rooms[idx].first, pi = rooms[idx].second;
            bool couple = coupleAllowed && bi >= 2;
            for (int j = m; j >= 0; j--) {
                int rowM = j - bi; if (rowM < 0) rowM = 0;
                int jj = j - 1; if (jj < 0) jj = 0;
                int *c0j = cost0[j], *c1j = cost1[j];
                int *c0m = cost0[rowM], *c1m = cost1[rowM];
                int *c0jj = cost0[jj];
                for (int k = f; k >= 0; k--) {
                    int colF = k - bi; if (colF < 0) colF = 0;
                    int kk = k - 1; if (kk < 0) kk = 0;

                    int best0 = c0j[k];
                    if (c0m[k] < INF) best0 = min(best0, c0m[k] + pi);
                    if (c0j[colF] < INF) best0 = min(best0, c0j[colF] + pi);

                    int best1 = c1j[k];
                    if (c1m[k] < INF) best1 = min(best1, c1m[k] + pi);
                    if (c1j[colF] < INF) best1 = min(best1, c1j[colF] + pi);
                    if (couple && c0jj[kk] < INF) best1 = min(best1, c0jj[kk] + pi);

                    c0j[k] = best0;
                    c1j[k] = best1;
                }
            }
        }

        int ans = min(cost0[m][f], cost1[m][f]);
        if (ans >= INF) printf("Impossible\n");
        else printf("%d\n", ans);
    }
    return 0;
}
