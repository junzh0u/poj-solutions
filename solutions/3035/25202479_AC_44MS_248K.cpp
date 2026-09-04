// POJ 3035 - Random Walking
// Model: claude-sonnet-5
//
// Approach: the k output numbers are the exact (not stationary!) distribution
// of a random walk on the graph after 0,1,...,k-1 steps, starting from the
// uniform distribution over all 2^n nodes (n independent fair coin tosses).
// From node u the walk follows a uniformly random incident edge (weighted by
// multiplicity), so for edge (u,v) with multiplicity m: next[v] += p[u]*m/deg(u)
// and next[u] += p[v]*m/deg(v). We simulate this exactly for k steps (V<=1024
// nodes, E<=2000 edges, k<=100), and for each step and each of the n bit
// positions sum p[v] over nodes v with that bit set, requiring it to lie
// strictly between 25% and 75%.
//
// Ambiguity/pitfall (confirmed against the discuss board, message 56674/56679
// and message 150406): the walk need not converge to the stationary
// distribution deg(v)/(2e) within k steps -- sample 1's graph (star with
// center 3) is periodic with period 2, oscillating between the uniform
// distribution and (1/12,1/12,1/12,3/4) forever, never settling; using the
// stationary distribution instead of exact step-by-step simulation gives the
// wrong answer. Also, per message 150406, the k numbers' bit events must be
// checked independently per output index, never merged/summed across the k
// generated numbers.
#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int k, n, e;
    while (scanf("%d %d %d", &k, &n, &e) == 3 && (k || n || e)) {
        int V = 1 << n;
        vector<int> deg(V, 0);
        vector<int> eu(e), ev(e);
        for (int i = 0; i < e; i++) {
            int v1, v2;
            scanf("%d %d", &v1, &v2);
            eu[i] = v1;
            ev[i] = v2;
            deg[v1]++;
            deg[v2]++;
        }
        vector<double> p(V, 1.0 / V);
        bool good = true;
        const double EPS = 1e-9;
        for (int step = 0; step < k; step++) {
            for (int bit = 0; bit < n; bit++) {
                double prob = 0.0;
                for (int v = 0; v < V; v++) {
                    if (v & (1 << bit)) prob += p[v];
                }
                if (!(prob > 0.25 + EPS && prob < 0.75 - EPS)) {
                    good = false;
                    break;
                }
            }
            if (!good) break;
            if (step + 1 < k) {
                vector<double> nxt(V, 0.0);
                for (int i = 0; i < e; i++) {
                    int u = eu[i], v = ev[i];
                    nxt[v] += p[u] / deg[u];
                    nxt[u] += p[v] / deg[v];
                }
                p = nxt;
            }
        }
        printf(good ? "Yes\n" : "No\n");
    }
    return 0;
}
