// POJ 1476 - Always On the Run
// Model: claude-sonnet-5
//
// Approach: Since flight schedules only exist between distinct ordered city
// pairs (n(n-1) schedules given, none for i->i), any sequence of flights
// automatically visits a different city each day than the day before -- the
// "different city than the day before" constraint in the statement is
// therefore automatically satisfied and needs no extra bookkeeping; the task
// reduces to a plain minimum-cost path of exactly k edges from city 1 to
// city n, where the cost of the i-th flight (day i, 1-indexed) on edge (u,v)
// is schedule[u][v][(i-1) mod d[u][v]], and a value of 0 means no flight
// exists that day.
//
// DP: dist[t][j] = minimum cost to be in city j after t flights, t=0..k.
// dist[0][1] = 0, all other dist[0][*] = INF (unreachable). Transition scans
// all n*n pairs per day; O(k*n^2) <= 1000*100 = 1e5, trivial for the limits.
//
// Pitfall (confirmed on the discuss board, message 70820/137038): using 0 as
// the "unreachable" sentinel is wrong, because 0 can also arise from summing
// only zero-cost pieces, or simply from never assigning the entry -- this
// silently produces a wrong "reachable" state. Use a large INF sentinel and
// only relax when both the predecessor state and the edge cost are finite
// resp. nonzero (0 cost on an edge means "no flight that day").
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

int main() {
    int n, k;
    int scenario = 0;
    while (scanf("%d %d", &n, &k) == 2) {
        if (n == 0 && k == 0) break;
        ++scenario;

        static int d[11][11];
        static vector<int> cost[11][11];
        memset(d, 0, sizeof(d));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                cost[i][j].clear();
            }
        }

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (i == j) continue;
                int di;
                scanf("%d", &di);
                d[i][j] = di;
                cost[i][j].resize(di);
                for (int t = 0; t < di; ++t) {
                    scanf("%d", &cost[i][j][t]);
                }
            }
        }

        const long INF = 2000000000L;
        static long dist[11], ndist[11];
        for (int i = 1; i <= n; ++i) dist[i] = INF;
        dist[1] = 0;

        for (int day = 1; day <= k; ++day) {
            for (int j = 1; j <= n; ++j) ndist[j] = INF;
            for (int i = 1; i <= n; ++i) {
                if (dist[i] >= INF) continue;
                for (int j = 1; j <= n; ++j) {
                    if (i == j) continue;
                    int dij = d[i][j];
                    if (dij <= 0) continue;
                    int idx = (day - 1) % dij;
                    int c = cost[i][j][idx];
                    if (c <= 0) continue; // no flight that day
                    long cand = dist[i] + c;
                    if (cand < ndist[j]) ndist[j] = cand;
                }
            }
            for (int j = 1; j <= n; ++j) dist[j] = ndist[j];
        }

        printf("Scenario #%d\n", scenario);
        if (n >= 1 && dist[n] < INF) {
            printf("The best flight costs %ld.\n", dist[n]);
        } else {
            printf("No flight possible.\n");
        }
        printf("\n");
    }
    return 0;
}
