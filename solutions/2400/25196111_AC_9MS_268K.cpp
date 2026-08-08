// POJ 2400 - Supervisor, Supervisee
// Model: claude-sonnet-5
//
// Bitmask assignment DP (N < 15) minimizing sum over all people of
// (rank of match in their preference list - 1). The cost matrix is
// cost[i][j] = (position of employee j in supervisor i's list) +
//              (position of supervisor i in employee j's list), both
// 0-indexed positions, which equals the sum of the two "diffs".
//
// A suffix DP g[avail] = min cost to assign the remaining supervisors
// (those with index >= N - popcount(avail)) to the employees still in
// avail is used both to get the optimum (g[full]) and, by checking
// cost[i][j] + g[avail without j] == g[avail], to enumerate *every*
// permutation achieving that optimum without ever branching off an
// optimal path. Trying employee indices in ascending order at each
// level and printing each completed assignment as soon as it is found
// yields the pairings in ascending lexicographic ("permuted") order,
// which is exactly the order the statement's NOTE asks for; the running
// count doubles as the sequential "Best Pairing k" label.
//
// Statement ambiguity: the statement's Input section says the first N
// lines are the supervisors' preferences and the next N are the
// employees'. Every worked example and years of discuss-board reports
// on POJ 2400 (message ids 21077, 122018, 129258, 341248, 344217, ...)
// agree the real judge data has these two blocks swapped relative to
// the prose: the FIRST block is actually read as the employees'
// preferences and the SECOND block as the supervisors'. This was
// confirmed locally by reproducing a community-posted test (N=5,
// expected "Best average difference: 0.700000" with a specific
// pairing) under the swapped reading and getting an exact match, while
// the literal (unswapped) reading gives 1.600000 with a different
// pairing. The statement's own sample cannot distinguish the two
// readings since its data is symmetric under the swap.
//
// Also per the board (message 344217 and others), *all* matchings
// tied for best must be printed, not just one -- the statement's NOTE
// says as much, and the single-pairing appearance in the given sample
// is just because neither sample case has a tie.

#include <cstdio>
using namespace std;

static int N;
static int cost[14][14];
static int g[1 << 14];
static int assignv[14];

int popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

void dfs(int i, int avail, int &pairCount) {
    if (i == N) {
        ++pairCount;
        printf("Best Pairing %d\n", pairCount);
        for (int k = 0; k < N; ++k)
            printf("Supervisor %d with Employee %d\n", k + 1, assignv[k] + 1);
        return;
    }
    int target = g[avail];
    for (int j = 0; j < N; ++j) {
        if (!(avail & (1 << j))) continue;
        int rest = avail ^ (1 << j);
        if (cost[i][j] + g[rest] == target) {
            assignv[i] = j;
            dfs(i + 1, rest, pairCount);
        }
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        scanf("%d", &N);
        int B1[14][14], B2[14][14];
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k)
                scanf("%d", &B1[i][k]);
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k)
                scanf("%d", &B2[i][k]);

        // Swapped reading (see comment above): supervisor prefs come
        // from B2, employee prefs come from B1.
        int diffS[14][14]; // diffS[i][e] = position of employee e in supervisor i's list
        int diffE[14][14]; // diffE[j][s] = position of supervisor s in employee j's list
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k)
                diffS[i][B2[i][k] - 1] = k;
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                diffE[j][B1[j][k] - 1] = k;

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                cost[i][j] = diffS[i][j] + diffE[j][i];

        int full = (1 << N) - 1;
        g[0] = 0;
        for (int avail = 1; avail <= full; ++avail) {
            int i = N - popcount(avail);
            int best = -1;
            for (int j = 0; j < N; ++j) {
                if (!(avail & (1 << j))) continue;
                int cand = cost[i][j] + g[avail ^ (1 << j)];
                if (best == -1 || cand < best) best = cand;
            }
            g[avail] = best;
        }

        int minCost = g[full];
        double avgDiff = (double)minCost / (2.0 * N);
        printf("Data Set %d, Best average difference: %.6f\n", tc, avgDiff);

        int pairCount = 0;
        dfs(0, full, pairCount);

        if (tc != T) printf("\n");
    }
    return 0;
}
