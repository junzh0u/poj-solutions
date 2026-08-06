// POJ 3498 - March of the Penguins
// Model: claude-sonnet-5
//
// For each floe, split it into an "in" node and an "out" node joined by an
// edge of capacity m_i (max jumps off that floe). Source connects to each
// in-node with capacity n_i (penguins starting there). For every pair (i,j)
// whose distance <= D, add an infinite edge out_i -> in_j (a penguin may
// jump off i onto j). To test whether floe k can be the meeting point,
// connect in_k -> sink with infinite capacity and run max flow from source;
// floe k works iff the flow equals the total number of penguins W. Repeat
// for every k (topology built once per test case; only capacities reset
// between candidates).
//
// Ambiguity: none found in the statement itself; the discuss board
// (message 107119) independently describes the same split-vertex
// enumerate-destination construction, which matches the sample ("1 2 4"
// verified by hand against floe-0 failing because floes 2 and 3 must both
// funnel through floe 1, whose m=1 allows only one jump off it).
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
using namespace std;

typedef long long ll;
const ll INF = (ll)1e15;

struct Dinic {
    int V;
    vector<int> eto, enext, ehead;
    vector<ll> ecap;

    void init(int v) {
        V = v;
        ehead.assign(v, -1);
        eto.clear(); enext.clear(); ecap.clear();
    }
    int addEdge(int u, int v, ll cap) {
        eto.push_back(v); ecap.push_back(cap); enext.push_back(ehead[u]); ehead[u] = (int)eto.size() - 1;
        eto.push_back(u); ecap.push_back(0);   enext.push_back(ehead[v]); ehead[v] = (int)eto.size() - 1;
        return (int)eto.size() - 2; // index of forward edge
    }

    vector<int> level, it;
    int S, T;

    bool bfs() {
        level.assign(V, -1);
        vector<int> q; q.reserve(V);
        level[S] = 0; q.push_back(S);
        for (size_t qi = 0; qi < q.size(); ++qi) {
            int u = q[qi];
            for (int e = ehead[u]; e != -1; e = enext[e]) {
                if (ecap[e] > 0 && level[eto[e]] < 0) {
                    level[eto[e]] = level[u] + 1;
                    q.push_back(eto[e]);
                }
            }
        }
        return level[T] >= 0;
    }

    ll dfs(int u, ll f) {
        if (u == T) return f;
        for (int &e = it[u]; e != -1; e = enext[e]) {
            int v = eto[e];
            if (ecap[e] > 0 && level[v] == level[u] + 1) {
                ll d = dfs(v, min(f, ecap[e]));
                if (d > 0) {
                    ecap[e] -= d;
                    ecap[e ^ 1] += d;
                    return d;
                }
            }
        }
        return 0;
    }

    ll maxflow(int s, int t, ll limit) {
        S = s; T = t;
        ll flow = 0;
        while (bfs()) {
            it = ehead;
            ll f;
            while ((f = dfs(S, INF)) > 0) {
                flow += f;
                if (flow >= limit) return flow;
            }
        }
        return flow;
    }
};

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N;
        double D;
        scanf("%d %lf", &N, &D);
        vector<double> x(N), y(N);
        vector<int> n(N), m(N);
        for (int i = 0; i < N; ++i) {
            scanf("%lf %lf %d %d", &x[i], &y[i], &n[i], &m[i]);
        }
        ll W = 0;
        for (int i = 0; i < N; ++i) W += n[i];

        // adjacency: reachable[i][j] true if dist(i,j) <= D
        double D2 = D * D;
        vector<vector<bool> > reach(N, vector<bool>(N, false));
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                double dx = x[i] - x[j], dy = y[i] - y[j];
                double d2 = dx * dx + dy * dy;
                if (d2 <= D2 + 1e-6) {
                    reach[i][j] = reach[j][i] = true;
                }
            }
        }

        // node ids: S=0, in_i = 1+2*i, out_i = 2+2*i, T = 1+2*N
        int S = 0, Tn = 1 + 2 * N;
        int V = 2 + 2 * N;
        Dinic dinic;
        dinic.init(V);
        vector<int> sinkEdge(N);
        for (int i = 0; i < N; ++i) {
            int inI = 1 + 2 * i, outI = 2 + 2 * i;
            dinic.addEdge(S, inI, n[i]);
            dinic.addEdge(inI, outI, m[i]);
            sinkEdge[i] = dinic.addEdge(inI, Tn, 0); // dynamic sink edge, cap 0 for now
        }
        for (int i = 0; i < N; ++i) {
            int outI = 2 + 2 * i;
            for (int j = 0; j < N; ++j) {
                if (i != j && reach[i][j]) {
                    int inJ = 1 + 2 * j;
                    dinic.addEdge(outI, inJ, INF);
                }
            }
        }

        vector<ll> origCap = dinic.ecap;
        vector<int> result;

        for (int k = 0; k < N; ++k) {
            if (W == 0) { result.push_back(k); continue; } // trivially feasible, no penguins to move
            dinic.ecap = origCap;
            dinic.ecap[sinkEdge[k]] = INF;
            ll flow = dinic.maxflow(S, Tn, W);
            if (flow >= W) result.push_back(k);
        }

        if (result.empty()) {
            printf("-1\n");
        } else {
            for (size_t i = 0; i < result.size(); ++i) {
                if (i) printf(" ");
                printf("%d", result[i]);
            }
            printf("\n");
        }
    }
    return 0;
}
