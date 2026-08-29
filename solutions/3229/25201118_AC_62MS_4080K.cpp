// POJ 3229 - The Best Travel Design
// Model: claude-sonnet-5
//
// Approach: Bus speed 120km/h, train speed 80km/h; scale all times by 240
// (lcm(80,120)) so every travel time is an exact integer (240/80=3 per km
// for train, 240/120=2 per km for bus) -- this avoids the floating-point
// precision issues the discuss board reports WA from. Total time budget is
// 12*K hours (board: "total time is 12*K hours, days don't matter"), scaled
// the same way.
//
// Floyd-Warshall gives shortest travel time between every pair of sights
// (passing through unvisited intermediate sights costs no extra stay time --
// only explicitly chosen stops accrue stay time, per the board: a sight
// visited more than once has its stay time counted only once, and the
// starting/ending sight 1 has its stay time counted once, at departure, not
// again on return). Because Floyd-Warshall distances satisfy the triangle
// inequality, growing the visited set can never shrink the minimum tour cost,
// so the answer is: the largest subset S (containing sight 1 and every
// required sight) for which the optimal Hamiltonian cycle on S (touring S,
// starting and ending at 1) plus the stay times of S fits the budget.
// Standard bitmask DP (N<=15): dp[mask][i] = min travel time starting at 1,
// visiting exactly the sights in mask, currently at i.
//
// Ambiguity settled from the discuss board (message ids 71652/71662 for the
// 12*K-hours budget, 72195/72218 for sight 1's stay time being counted only
// once, on departure).

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long ll;
const ll INF = (ll)1e15;

int N, M, K;
ll D[16][16];
int stayTime[16];
ll dp[1 << 15][15];

int main() {
    while (scanf("%d %d %d", &N, &M, &K) == 3) {
        if (N == 0 && M == 0 && K == 0) break;

        int requiredMask = 1; // sight 1 always required
        for (int i = 0; i < M; i++) {
            int r;
            scanf("%d", &r);
            requiredMask |= (1 << (r - 1));
        }
        for (int i = 1; i <= N; i++) scanf("%d", &stayTime[i]);

        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= N; j++)
                D[i][j] = (i == j) ? 0 : INF;

        while (true) {
            int x, y, len, kind;
            scanf("%d %d %d %d", &x, &y, &len, &kind);
            if (x == 0 && y == 0 && len == 0 && kind == 0) break;
            int mult = (kind == 0) ? 3 : 2; // train: 240/80=3, bus: 240/120=2
            ll w = (ll)len * mult;
            if (w < D[x][y]) { D[x][y] = w; D[y][x] = w; }
        }

        for (int k = 1; k <= N; k++)
            for (int i = 1; i <= N; i++) {
                if (D[i][k] >= INF) continue;
                for (int j = 1; j <= N; j++) {
                    if (D[k][j] >= INF) continue;
                    ll cand = D[i][k] + D[k][j];
                    if (cand < D[i][j]) D[i][j] = cand;
                }
            }

        ll budget = 12LL * K * 240;

        int full = 1 << N;
        for (int mask = 0; mask < full; mask++)
            for (int i = 0; i < N; i++)
                dp[mask][i] = INF;
        dp[1][0] = 0;

        for (int mask = 1; mask < full; mask++) {
            if (!(mask & 1)) continue;
            for (int i = 0; i < N; i++) {
                if (!(mask & (1 << i))) continue;
                if (dp[mask][i] >= INF) continue;
                ll base = dp[mask][i];
                for (int j = 0; j < N; j++) {
                    if (mask & (1 << j)) continue;
                    ll w = D[i + 1][j + 1];
                    if (w >= INF) continue;
                    ll nd = base + w;
                    int nmask = mask | (1 << j);
                    if (nd < dp[nmask][j]) dp[nmask][j] = nd;
                }
            }
        }

        int best = -1;
        for (int mask = 1; mask < full; mask++) {
            if (!(mask & 1)) continue;
            if ((mask & requiredMask) != requiredMask) continue;
            ll staySum = 0;
            for (int i = 0; i < N; i++)
                if (mask & (1 << i)) staySum += (ll)stayTime[i + 1] * 240;
            ll minCycle = INF;
            for (int i = 0; i < N; i++) {
                if (!(mask & (1 << i))) continue;
                if (dp[mask][i] >= INF) continue;
                ll back = D[i + 1][1];
                if (back >= INF) continue;
                ll cand = dp[mask][i] + back;
                if (cand < minCycle) minCycle = cand;
            }
            if (minCycle >= INF) continue;
            ll total = staySum + minCycle;
            if (total <= budget) {
                int cnt = __builtin_popcount((unsigned)mask);
                if (cnt > best) best = cnt;
            }
        }

        if (best < 0) printf("No Solution\n");
        else printf("%d\n", best);
    }
    return 0;
}
