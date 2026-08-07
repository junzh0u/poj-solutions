// POJ 2175 - Evacuation Plan
// Model: claude-sonnet-5
// Approach: recompute the minimum cost flow (source -> buildings -> shelters ->
// sink; buildings have exact supply Bi, shelters have capacity Cj, edge cost is
// the Manhattan distance plus 1) with successive shortest augmenting paths
// (SPFA/Bellman-Ford, since residual edges can be negative-cost). Compare its
// total cost with the given plan's cost (sum of E_ij * D_ij): equal costs mean
// OPTIMAL; otherwise print SUBOPTIMAL followed by the recomputed assignment,
// which is provably optimal and therefore strictly better whenever the given
// plan was not optimal already, satisfying the special judge's "valid and
// better" requirement without needing to relate it to the input plan at all.
// No statement ambiguity found. Board warnings checked: this is a minimum-cost
// (not maximum-cost) flow, the "+1" in D_i,j must not be dropped, and totals
// must not overflow (handled here with long long throughout).

#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
using namespace std;

typedef long long ll;
const ll INF = (ll)1e18;

struct Edge { int to; ll cap; ll cost; int rev; };
vector<Edge> graph_[210];
int V;

void add_edge(int from, int to, ll cap, ll cost) {
    Edge a; a.to = to; a.cap = cap; a.cost = cost; a.rev = (int)graph_[to].size();
    Edge b; b.to = from; b.cap = 0; b.cost = -cost; b.rev = (int)graph_[from].size();
    graph_[from].push_back(a);
    graph_[to].push_back(b);
}

ll dist_[210];
int prevv[210], preve[210];
bool inqueue[210];

bool spfa(int s, int t) {
    for (int i = 0; i < V; i++) dist_[i] = INF;
    dist_[s] = 0;
    memset(inqueue, 0, sizeof(inqueue));
    queue<int> q;
    q.push(s); inqueue[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop(); inqueue[u] = false;
        for (int i = 0; i < (int)graph_[u].size(); i++) {
            Edge &e = graph_[u][i];
            if (e.cap > 0 && dist_[u] + e.cost < dist_[e.to]) {
                dist_[e.to] = dist_[u] + e.cost;
                prevv[e.to] = u; preve[e.to] = i;
                if (!inqueue[e.to]) {
                    inqueue[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }
    return dist_[t] < INF;
}

ll min_cost_flow(int s, int t) {
    ll totalcost = 0;
    while (spfa(s, t)) {
        ll d = INF;
        for (int v = t; v != s; v = prevv[v])
            d = min(d, graph_[prevv[v]][preve[v]].cap);
        for (int v = t; v != s; v = prevv[v]) {
            graph_[prevv[v]][preve[v]].cap -= d;
            Edge &back = graph_[prevv[v]][preve[v]];
            graph_[back.to][back.rev].cap += d;
        }
        totalcost += d * dist_[t];
    }
    return totalcost;
}

int main() {
    int N, M;
    while (scanf("%d %d", &N, &M) == 2) {
        vector<int> X(N + 1), Y(N + 1), B(N + 1);
        vector<int> P(M + 1), Q(M + 1), C(M + 1);
        for (int i = 1; i <= N; i++) scanf("%d %d %d", &X[i], &Y[i], &B[i]);
        for (int j = 1; j <= M; j++) scanf("%d %d %d", &P[j], &Q[j], &C[j]);
        vector<vector<int> > E(N + 1, vector<int>(M + 1));
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= M; j++)
                scanf("%d", &E[i][j]);

        vector<vector<ll> > D(N + 1, vector<ll>(M + 1));
        ll planCost = 0;
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= M; j++) {
                D[i][j] = (ll)abs(X[i] - P[j]) + (ll)abs(Y[i] - Q[j]) + 1;
                planCost += (ll)E[i][j] * D[i][j];
            }

        V = N + M + 2;
        for (int i = 0; i < V; i++) graph_[i].clear();
        int S = 0, T = N + M + 1;
        vector<vector<int> > eidx(N + 1, vector<int>(M + 1));
        vector<vector<ll> > ecap(N + 1, vector<ll>(M + 1));
        for (int i = 1; i <= N; i++) add_edge(S, i, B[i], 0);
        for (int j = 1; j <= M; j++) add_edge(N + j, T, C[j], 0);
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= M; j++) {
                ll cap = min(B[i], C[j]);
                ecap[i][j] = cap;
                eidx[i][j] = (int)graph_[i].size();
                add_edge(i, N + j, cap, D[i][j]);
            }

        ll optCost = min_cost_flow(S, T);

        if (optCost == planCost) {
            printf("OPTIMAL\n");
        } else {
            printf("SUBOPTIMAL\n");
            for (int i = 1; i <= N; i++) {
                for (int j = 1; j <= M; j++) {
                    ll flow = ecap[i][j] - graph_[i][eidx[i][j]].cap;
                    printf("%lld%c", flow, j == M ? '\n' : ' ');
                }
            }
        }
    }
    return 0;
}
