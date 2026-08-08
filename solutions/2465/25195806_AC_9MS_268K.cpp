// POJ 2465 - Adventures in Moving - Part IV
// Model: claude-opus-5
//
// Truck burns 1 litre/km, tank holds 200 L, picked up with 100 L (half full)
// and must be returned with at least 100 L. Exact DP over (checkpoint, litres
// in tank), litres being an integer in [0,200]:
//   f[v] = min cost to stand at the current checkpoint holding v litres.
// Start f[100] = 0 at km 0. For each station in order, first shift by the gap
// g (f'[v] = f[v+g], which drops every state that cannot cover the leg and
// every state that would need more than the 200 L tank), then relax purchases
// with an ascending sweep f[v] = min(f[v], f[v-1] + price), which buys any
// integer number of litres at that station. After the last station, shift by
// the final leg and take the min over v >= 100. No feasible state means
// "Impossible" -- reachable via a gap over 200 km, and also with no stations
// at all, which the input permits.
//
// Ambiguity: a gas station may sit exactly at the destination (the sample has
// one, at km 500), and it is not stated whether it may be used to top the tank
// back up to half full on arrival. The sample cannot settle it -- that station
// is the most expensive of the seven and no optimal plan buys there, so both
// readings print 450550. The discuss board settles it: the reformulation posted
// there by an accepted solver is to extend the destination by 100 km and merely
// require the tank not run dry, which places a station at km D strictly before
// the virtual endpoint and so permits buying there. This code takes that
// (permissive) reading; on the separating case "50 / 50 7" it prints 350 where
// the restrictive reading prints "Impossible".
//
// Costs are tenths of a cent: at most (10000 + 100) litres at 2000 each, about
// 2.0e7, so int suffices.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static const int CAP = 200;
static const int HALF = 100;
static const int INF = 1000000000;

int main() {
    int D;
    if (scanf("%d", &D) != 1) return 0;
    vector<int> pos, pri;
    int p, c;
    while (scanf("%d %d", &p, &c) == 2) { pos.push_back(p); pri.push_back(c); }

    vector<int> f(CAP + 1, INF), nf(CAP + 1, INF);
    f[HALF] = 0;
    int cur = 0;
    for (size_t i = 0; i < pos.size(); i++) {
        if (pos[i] > D) continue;
        int g = pos[i] - cur;
        if (g < 0) g = 0;
        fill(nf.begin(), nf.end(), INF);
        for (int v = 0; v + g <= CAP; v++) nf[v] = f[v + g];
        f = nf;
        for (int v = 1; v <= CAP; v++)
            if (f[v - 1] < INF && f[v - 1] + pri[i] < f[v]) f[v] = f[v - 1] + pri[i];
        cur = pos[i];
    }
    int g = D - cur;
    int best = INF;
    for (int v = HALF; v <= CAP; v++)
        if (v + g <= CAP && f[v + g] < best) best = f[v + g];

    if (best >= INF) printf("Impossible\n");
    else printf("%d\n", best);
    return 0;
}
