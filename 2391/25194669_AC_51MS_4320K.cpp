// POJ 2391 - Ombrophobic Bovines
// Model: claude-sonnet-5
// Approach: Binary search on time T over the O(F^2) candidate values that are
// all-pairs-shortest-path distances between fields (computed with
// Floyd-Warshall on the F<=200 field graph). For a fixed T, feasibility is a
// max-flow question: build a flow network with a super source connected to
// each field i with capacity = cows[i], each field i connected to the super
// sink with capacity = shelter_capacity[i], and an (infinite-capacity) edge
// field i -> field j whenever dist[i][j] <= T (paths are wide, so any number
// of cows can use them simultaneously; only the source/sink capacities are
// ever a bottleneck). T is feasible iff the max flow saturates every cow
// (equals total cow count). Feasibility is monotonic in T, so binary search
// over the sorted distinct candidate distances (plus 0) finds the minimum
// feasible T; if even the largest candidate is infeasible, answer is -1.
// Ambiguity: none noted in the statement itself; standard USACO 2005 March
// Gold "Silent Alarm"/"Rain" style problem, matches sample exactly.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

typedef long long ll;
const ll INF = (ll)4e18;

int F, P;
ll dist_[205][205];
int cows[205], shelter[205];

// Dinic's max flow
struct Edge { int to; ll cap; int next; };
std::vector<Edge> edges;
int head_[410];
int levelArr[410];
int iterArr[410];
int N; // number of nodes: 0 = source, 1..F = OUT fields, F+1..2F = IN fields, 2F+1 = sink

void initGraph() {
    edges.clear();
    memset(head_, -1, sizeof(head_));
}

void add_edge(int u, int v, ll cap) {
    edges.push_back(Edge()); edges.back().to = v; edges.back().cap = cap; edges.back().next = head_[u]; head_[u] = (int)edges.size() - 1;
    edges.push_back(Edge()); edges.back().to = u; edges.back().cap = 0;   edges.back().next = head_[v]; head_[v] = (int)edges.size() - 1;
}

bool bfs(int s, int t) {
    memset(levelArr, -1, sizeof(int) * N);
    std::vector<int> q;
    q.reserve(N);
    levelArr[s] = 0;
    q.push_back(s);
    for (size_t qi = 0; qi < q.size(); ++qi) {
        int u = q[qi];
        for (int e = head_[u]; e != -1; e = edges[e].next) {
            if (edges[e].cap > 0 && levelArr[edges[e].to] < 0) {
                levelArr[edges[e].to] = levelArr[u] + 1;
                q.push_back(edges[e].to);
            }
        }
    }
    return levelArr[t] >= 0;
}

ll dfs(int u, int t, ll f) {
    if (u == t) return f;
    for (int &e = iterArr[u]; e != -1; e = edges[e].next) {
        int v = edges[e].to;
        if (edges[e].cap > 0 && levelArr[v] == levelArr[u] + 1) {
            ll d = dfs(v, t, std::min(f, edges[e].cap));
            if (d > 0) {
                edges[e].cap -= d;
                edges[e ^ 1].cap += d;
                return d;
            }
        }
    }
    return 0;
}

ll maxflow(int s, int t) {
    ll flow = 0;
    while (bfs(s, t)) {
        memcpy(iterArr, head_, sizeof(int) * N);
        ll f;
        while ((f = dfs(s, t, INF)) > 0) flow += f;
    }
    return flow;
}

// Bipartite construction: OUT_i (node i, 1..F) represents field i as a
// source of cows; IN_j (node F+j, 1..F) represents field j as a shelter
// destination. Edges only go S -> OUT_i -> IN_j -> Tn, with no edge leaving
// an IN node other than to the sink, so flow can never chain through
// multiple field-field hops (which would incorrectly let two edges each
// individually <= T sum to a transit time greater than T).
bool feasible(ll T, ll totalCows) {
    initGraph();
    int S = 0, Tn = 2 * F + 1;
    N = 2 * F + 2;
    for (int i = 1; i <= F; ++i) {
        if (cows[i] > 0) add_edge(S, i, cows[i]);
        if (shelter[i] > 0) add_edge(F + i, Tn, shelter[i]);
    }
    for (int i = 1; i <= F; ++i) {
        for (int j = 1; j <= F; ++j) {
            if (dist_[i][j] <= T) {
                add_edge(i, F + j, INF);
            }
        }
    }
    ll f = maxflow(S, Tn);
    return f >= totalCows;
}

int main() {
    if (scanf("%d %d", &F, &P) != 2) return 0;
    for (int i = 1; i <= F; ++i) {
        for (int j = 1; j <= F; ++j) dist_[i][j] = (i == j) ? 0 : INF;
    }
    for (int i = 1; i <= F; ++i) {
        scanf("%d %d", &cows[i], &shelter[i]);
    }
    for (int k = 0; k < P; ++k) {
        int u, v; ll w;
        scanf("%d %d %lld", &u, &v, &w);
        if (w < dist_[u][v]) { dist_[u][v] = w; dist_[v][u] = w; }
    }
    // Floyd-Warshall
    for (int k = 1; k <= F; ++k) {
        for (int i = 1; i <= F; ++i) {
            if (dist_[i][k] == INF) continue;
            for (int j = 1; j <= F; ++j) {
                if (dist_[k][j] == INF) continue;
                ll nd = dist_[i][k] + dist_[k][j];
                if (nd < dist_[i][j]) dist_[i][j] = nd;
            }
        }
    }

    ll totalCows = 0;
    for (int i = 1; i <= F; ++i) totalCows += cows[i];

    std::vector<ll> cand;
    cand.push_back(0);
    for (int i = 1; i <= F; ++i) {
        for (int j = 1; j <= F; ++j) {
            if (i != j && dist_[i][j] < INF) cand.push_back(dist_[i][j]);
        }
    }
    std::sort(cand.begin(), cand.end());
    cand.erase(std::unique(cand.begin(), cand.end()), cand.end());

    if (totalCows == 0) {
        printf("0\n");
        return 0;
    }

    int lo = 0, hi = (int)cand.size() - 1, ans = -1;
    if (!cand.empty() && feasible(cand[hi], totalCows)) {
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (feasible(cand[mid], totalCows)) hi = mid; else lo = mid + 1;
        }
        ans = lo;
    }

    if (ans == -1) {
        printf("-1\n");
    } else {
        printf("%lld\n", cand[ans]);
    }
    return 0;
}
