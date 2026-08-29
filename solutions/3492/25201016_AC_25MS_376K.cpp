// POJ 3492 - Knapsack II
// Model: claude-sonnet-5
// Approach: classic multi-coin Frobenius number via Dijkstra shortest paths
// over residues mod the smallest item size m. dist[r] = smallest
// non-negative integer representable by the given items that is
// congruent to r (mod m). Building edges u -> (u + a_i) mod m with
// weight a_i for every item value a_i (deduped, and excluding multiples
// of m which are useless self-loops) and running Dijkstra from node 0
// gives dist[]; the answer (Frobenius number) is max over r=1..m-1 of
// (dist[r] - m). If gcd of all item sizes != 1, infinitely many
// capacities (all non-multiples of the gcd) can never be filled, so we
// print INF. The board's message #89408/89411 ("no such test case")
// confirms the degenerate n=1,size=1 case (where every capacity is
// representable) is not part of the judge data, but the code handles it
// correctly anyway (falls out of the gcd!=1 / m==1 special-casing as
// INF). Values (n,item size <= 5000) keep every distance well under
// 2^31, so plain `long` suffices; no long long/%lld dependency.
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

static long gcdl(long a, long b) {
    while (b) { long t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        vector<long> a(n);
        for (int i = 0; i < n; ++i) scanf("%ld", &a[i]);

        long g = 0;
        for (int i = 0; i < n; ++i) g = gcdl(g, a[i]);

        long m = *min_element(a.begin(), a.end());

        if (g != 1 || m == 1) {
            printf("INF\n");
            continue;
        }

        // dedupe values, drop multiples of m (useless self-loop edges)
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        vector<long> items;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] % m != 0) items.push_back(a[i]);
        }

        int mm = (int)m;
        vector<long> dist(mm, -1);
        dist[0] = 0;
        priority_queue<pair<long,int>, vector<pair<long,int> >, greater<pair<long,int> > > pq;
        pq.push(make_pair(0L, 0));
        while (!pq.empty()) {
            pair<long,int> top = pq.top(); pq.pop();
            long d = top.first;
            int u = top.second;
            if (dist[u] != -1 && d > dist[u]) continue;
            for (size_t i = 0; i < items.size(); ++i) {
                long w = items[i];
                int v = (int)((u + w) % mm);
                long nd = d + w;
                if (dist[v] == -1 || nd < dist[v]) {
                    dist[v] = nd;
                    pq.push(make_pair(nd, v));
                }
            }
        }

        long ans = -1; // always overwritten since m>=2 here
        for (int r = 1; r < mm; ++r) {
            long cand = dist[r] - m;
            if (cand > ans) ans = cand;
        }
        printf("%ld\n", ans);
    }
    return 0;
}
