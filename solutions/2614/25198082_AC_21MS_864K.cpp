// POJ 2614 - Old Wine Into New Bottles
// Model: claude-sonnet-5
//
// Each bottle type i has a min/max fill (min_i <= max_i <= 4500, min_i in
// [0.95,0.99]*max_i).  We choose nonnegative counts k_i of each type; each
// individual bottle can be filled anywhere in [min_i,max_i].  For a fixed
// selection, the achievable total poured volume ranges continuously over
// [sum k_i*min_i, sum k_i*max_i]; feasibility requires sum k_i*min_i <= W
// (wine available), and the best we can pour is min(W, sum k_i*max_i).  So
// the task reduces to an unbounded knapsack: maximize M = sum k_i*max_i
// subject to sum k_i*min_i <= W, then waste = max(0, W - M).
//
// W can be up to 1e9 ml, far too large for a direct O(W*n) DP, but every
// item's weight (min_i) is bounded by 4500.  Let (G,VG) be the item with the
// best value/weight ratio max_i/min_i (found by cross-multiplication to
// avoid floating point).  For every other item i, min_i*VG - max_i*G >= 0
// (its ratio is no better than the best one), so this quantity is a valid
// nonnegative edge cost in a graph over residues mod G: node r holds the
// (m,v) pair reachable with minimum "cost" = m*VG - v*G among all subsets
// with sum(min_i) == m (mod G).  A short algebraic identity shows that for
// fixed target capacity W and fixed residue r, the expression
//   v + floor((W-m)/G)*VG
// (pad the representative up to <=W using more copies of the best-ratio
// item) equals a constant minus cost/G -- i.e. is maximized exactly when
// cost is minimized, independent of which representative of the residue
// class is used.  So: Dijkstra once over the <=4500 residues (nonnegative
// edge costs) to get the minimum-cost representative per residue, then scan
// all residues and take the best padded value.  This is the standard
// "shortest path over residues" trick for huge-capacity unbounded knapsack
// with bounded item weights.
//
// No statement ambiguity found; single dataset per input (matches the two
// separate Sample Input / Sample Input (2) blocks in the statement, which
// are independent examples, not one concatenated stream).
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

int main() {
    // Use iostream throughout (not scanf/printf %lld) so 64-bit I/O does not
    // depend on which format specifier the judge's compiler expects.
    long long Wlitres;
    int n;
    if (!(cin >> Wlitres >> n)) return 0;
    vector<long long> mn(n), mx(n);
    for (int i = 0; i < n; i++) {
        cin >> mn[i] >> mx[i];
    }

    // find best ratio item: maximize mx[i]/mn[i], compare via cross mult
    int best = 0;
    for (int i = 1; i < n; i++) {
        // mx[i]*mn[best] vs mx[best]*mn[i]
        if (mx[i] * mn[best] > mx[best] * mn[i]) best = i;
    }
    long long G = mn[best], VG = mx[best];

    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(G, INF), M(G, 0), V(G, 0);
    dist[0] = 0; M[0] = 0; V[0] = 0;

    priority_queue<pair<long long,int>, vector<pair<long long,int> >, greater<pair<long long,int> > > pq;
    pq.push(make_pair(0LL, 0));

    while (!pq.empty()) {
        pair<long long,int> top = pq.top(); pq.pop();
        long long d = top.first;
        int r = top.second;
        if (d != dist[r]) continue;
        for (int i = 0; i < n; i++) {
            long long costI = mn[i] * VG - mx[i] * G; // >= 0
            int nr = (int)((r + mn[i]) % G);
            long long nd = d + costI;
            long long nm = M[r] + mn[i];
            long long nv = V[r] + mx[i];
            if (nd < dist[nr] || (nd == dist[nr] && nm < M[nr])) {
                dist[nr] = nd;
                M[nr] = nm;
                V[nr] = nv;
                pq.push(make_pair(nd, nr));
            }
        }
    }

    long long Wml = Wlitres * 1000LL;
    long long best_M = 0; // baseline: use nothing
    for (int r = 0; r < (int)G; r++) {
        if (dist[r] >= INF) continue;
        if (M[r] > Wml) continue;
        long long t = (Wml - M[r]) / G;
        long long cand = V[r] + t * VG;
        if (cand > best_M) best_M = cand;
    }

    long long waste = Wml - best_M;
    if (waste < 0) waste = 0;
    cout << waste << "\n";
    return 0;
}
