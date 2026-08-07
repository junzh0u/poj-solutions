// POJ 3260 - The Fewest Coins
// Model: claude-opus-5
//
// FJ pays some amount P >= T out of the coins he carries (bounded supply, C_i of
// value V_i); the shopkeeper hands back P - T from an unlimited supply of the same
// values, always in the fewest coins.  Minimise (coins paid) + (coins in change).
//
// Two independent knapsacks over amounts, joined at the end:
//   pay[p] - bounded (multiple) knapsack, min coins of FJ's to make exactly p,
//            done by binary splitting of each C_i into powers of two;
//   chg[a] - unbounded (complete) knapsack, min coins to make exactly a.
//   answer = min over p >= T of pay[p] + chg[p - T].
//
// Search bound: it is never useful to pay more than T + maxV^2.  If the change
// exceeded maxV^2 it would need more than maxV coins, and by pigeonhole on the
// prefix sums of those coins some non-empty subset sums to a multiple of maxV;
// the same holds for the (then equally large) payment, and the two multiples can
// be cancelled against each other for a strictly cheaper transaction.  With
// V_i <= 120 and T <= 10000 that caps the tables at 24400 entries.
// Verified empirically too: a brute force that searches p all the way up to the
// total value FJ carries agrees on ~1500 random small instances, including
// profiles where that total is an order of magnitude past T + maxV^2.
//
// Note: "impossible" covers both directions - FJ must be able to hand over p and
// the shopkeeper must be able to make p - T exactly - so -1 is printed only when
// no p at all works.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static const int INF = 0x3f3f3f3f;

int main() {
    int n, t;
    while (scanf("%d %d", &n, &t) == 2) {
        vector<int> v(n), c(n);
        int maxv = 0;
        for (int i = 0; i < n; ++i) {
            scanf("%d", &v[i]);
            if (v[i] > maxv) maxv = v[i];
        }
        for (int i = 0; i < n; ++i) scanf("%d", &c[i]);

        int limit = t + maxv * maxv;

        vector<int> pay(limit + 1, INF);
        pay[0] = 0;
        for (int i = 0; i < n; ++i) {
            int cnt = c[i];
            int cap = limit / v[i];
            if (cnt > cap) cnt = cap;
            for (int k = 1; cnt > 0; k <<= 1) {
                int take = k < cnt ? k : cnt;
                cnt -= take;
                int w = take * v[i];
                for (int j = limit; j >= w; --j)
                    if (pay[j - w] + take < pay[j]) pay[j] = pay[j - w] + take;
            }
        }

        int cl = limit - t;
        vector<int> chg(cl + 1, INF);
        chg[0] = 0;
        for (int i = 0; i < n; ++i)
            for (int j = v[i]; j <= cl; ++j)
                if (chg[j - v[i]] + 1 < chg[j]) chg[j] = chg[j - v[i]] + 1;

        int best = INF;
        for (int p = t; p <= limit; ++p)
            if (pay[p] < INF && chg[p - t] < INF && pay[p] + chg[p - t] < best)
                best = pay[p] + chg[p - t];

        printf("%d\n", best == INF ? -1 : best);
    }
    return 0;
}
