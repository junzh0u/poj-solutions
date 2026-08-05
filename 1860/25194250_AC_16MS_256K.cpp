// POJ 1860 - Currency Exchange
//
// Arbitrage detection.  Each exchange point gives two directed edges, A->B and
// B->A; taking edge (u,v) with rate r and commission c turns x units of u into
// (x - c) * r units of v.  Nick wins iff, starting from V units of currency S,
// some sequence of exchanges brings him back to S with more than V.
//
// Run Bellman-Ford *maximising* the amount of money instead of minimising a
// distance: dist[S] = V, dist[i] = 0 for every other currency (holding zero of
// a currency is always achievable, and the relaxation only ever raises a value,
// so the "money must stay non-negative" rule is respected automatically).
// Relax dist[v] = max(dist[v], (dist[u] - c) * r) over all 2M edges.
//
// n-1 rounds are enough to settle every simple path, so if an n-th round can
// still raise something there is a cycle whose traversal keeps growing the
// capital.  Because every exchange point is usable in both directions, anything
// reachable from S can also reach S, so a growing cycle is always cashable back
// into S -- report YES.  A 1e-9 slack in the comparison stops floating-point
// noise (and cycles that merely converge to a fixed point) from being mistaken
// for real growth; dist[S] > V is also accepted directly.
//
// Statement subtlety: "increase his capital" means strictly more than V, and
// V may be 0, in which case no rate can ever produce a profit.

#include <cstdio>

static const int MAXN = 105;
static const int MAXE = 205;

int ea[MAXE], eb[MAXE];
double er[MAXE], ec[MAXE];
double dist[MAXN];

int main() {
    int n, m, s;
    double v;
    while (scanf("%d %d %d %lf", &n, &m, &s, &v) == 4) {
        int cnt = 0;
        for (int i = 0; i < m; ++i) {
            int a, b;
            double rab, cab, rba, cba;
            if (scanf("%d %d %lf %lf %lf %lf", &a, &b, &rab, &cab, &rba, &cba) != 6)
                return 0;
            ea[cnt] = a; eb[cnt] = b; er[cnt] = rab; ec[cnt] = cab; ++cnt;
            ea[cnt] = b; eb[cnt] = a; er[cnt] = rba; ec[cnt] = cba; ++cnt;
        }

        for (int i = 1; i <= n; ++i) dist[i] = 0.0;
        dist[s] = v;

        /* Bellman-Ford maximising the amount of money.  n-1 rounds settle every
           simple path; if a round n relaxation is still possible there is a
           cycle that keeps growing the capital, i.e. an arbitrage loop. */
        bool changed = false;
        int round;
        for (round = 0; round < n; ++round) {
            changed = false;
            for (int e = 0; e < cnt; ++e) {
                double cand = (dist[ea[e]] - ec[e]) * er[e];
                if (cand > dist[eb[e]] + 1e-9) {
                    dist[eb[e]] = cand;
                    changed = true;
                }
            }
            if (!changed) break;
        }

        bool yes = changed || dist[s] > v + 1e-9;
        printf("%s\n", yes ? "YES" : "NO");
    }
    return 0;
}
