// POJ 3686 - The Windy's
// Model: claude-sonnet-5
// Approach: Minimize average completion time when N orders are distributed among M
// workshops, each workshop processing its assigned orders sequentially in an order we
// choose. Classic reduction: within a single workshop, if a job is placed at position t
// (1-indexed) in that workshop's own processing sequence, its processing time contributes
// with weight t to the total sum of completion times (it is included in its own completion
// time and in every later job's completion time in that same sequence). So we build a
// bipartite assignment problem between the N orders and N*M "slots", where slot (j,t) for
// workshop j and position t (t = 1..N) has cost t*Z[i][j] for order i. An order assigned to
// slot (j,t) is placed at position t within workshop j. Because cost is monotone increasing
// in t for a fixed workshop, the optimal assignment for any workshop always fills a prefix
// of positions 1..k (using position t without t-1 filled would never be optimal), so no
// extra prefix constraint is needed. Solve via the O(n^2*m) Hungarian algorithm (successive
// shortest augmenting paths with potentials) on the n=N x m=N*M cost matrix, sum the costs
// of the matched columns to get total completion time, divide by N for the average.
// No statement ambiguity found; verified against all three sample cases.
#include <cstdio>
#include <vector>
using namespace std;

typedef long long ll;
static const ll INF = (ll)1e18;

int main(){
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N, M;
        scanf("%d %d", &N, &M);
        vector<vector<int> > Z(N + 1, vector<int>(M + 1));
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= M; j++)
                scanf("%d", &Z[i][j]);

        int n = N;
        int m = N * M; // column (j-1)*N + t, t = 1..N, for workshop j position t

        vector<vector<ll> > a(n + 1, vector<ll>(m + 1));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= M; j++) {
                int base = (j - 1) * N;
                for (int t = 1; t <= N; t++) {
                    a[i][base + t] = (ll)t * Z[i][j];
                }
            }
        }

        vector<ll> u(n + 1, 0), v(m + 1, 0);
        vector<int> p(m + 1, 0), way(m + 1, 0);

        for (int i = 1; i <= n; i++) {
            p[0] = i;
            int j0 = 0;
            vector<ll> minv(m + 1, INF);
            vector<bool> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1 = -1;
                ll delta = INF;
                for (int j = 1; j <= m; j++) {
                    if (!used[j]) {
                        ll cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                        if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                    }
                }
                for (int j = 0; j <= m; j++) {
                    if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                    else minv[j] -= delta;
                }
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        ll total = 0;
        for (int j = 1; j <= m; j++) {
            if (p[j] != 0) total += a[p[j]][j];
        }
        double avg = (double)total / N;
        printf("%.6f\n", avg);
    }
    return 0;
}
