// POJ 3762 - The Bonus Salary!
// Model: claude-sonnet-5
// Each task occupies a fixed clock-time interval [L,R) (seconds since
// midnight, R strictly after L). Assigning tasks to K days so that a day's
// tasks never overlap is equivalent (interval graphs are perfect graphs) to
// choosing a max-weight subset of the N intervals such that no instant of
// the 24h clock is covered by more than K chosen intervals. This is the
// same construction as POJ 3680 "Intervals": compress the O(N) endpoints,
// chain them with capacity-K zero-cost edges, add a capacity-1 cost(-w)
// shortcut edge for every task, and push K units of min-cost flow from the
// first to the last coordinate. Back-to-back tasks (one ends exactly when
// another starts) can share a day, matching the sample (task1 ends 09:30:00,
// task4 starts 09:30:00, both scheduled on day 1) -- this falls out for free
// since coordinates are compressed points and the flow edges are half-open.
// K may be 0, in which case no flow is pushed and the answer is 0.
// Discuss board confirms: this is a well-known reskin of 3680, solved with
// min-cost flow (thread 143160, 156284); no framing surprises found for the
// single-test-case input here.
#include <algorithm>
#include <cstdio>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int to, rev, cap, cost;
    Edge(int to_, int rev_, int cap_, int cost_) : to(to_), rev(rev_), cap(cap_), cost(cost_) {}
};

struct MinCostFlow {
    vector< vector<Edge> > graph;

    explicit MinCostFlow(int n) : graph(n) {}

    void addEdge(int from, int to, int cap, int cost) {
        graph[from].push_back(Edge(to, (int)graph[to].size(), cap, cost));
        graph[to].push_back(Edge(from, (int)graph[from].size() - 1, 0, -cost));
    }

    int minCost(int source, int sink, int wantedFlow) {
        const int n = (int)graph.size();
        const int inf = 0x3f3f3f3f;
        vector<int> potential(n, inf);
        potential[source] = 0;

        // All original edges go from a lower to a higher compressed
        // coordinate, so this ordered relaxation gives valid initial
        // shortest-path potentials without needing SPFA/Bellman-Ford.
        for (int v = source; v <= sink; ++v) {
            if (potential[v] == inf) continue;
            for (size_t e = 0; e < graph[v].size(); ++e) {
                const Edge &edge = graph[v][e];
                if (edge.cap > 0 && edge.to > v && potential[edge.to] > potential[v] + edge.cost) {
                    potential[edge.to] = potential[v] + edge.cost;
                }
            }
        }
        for (int v = 0; v < n; ++v) if (potential[v] == inf) potential[v] = 0;

        int totalCost = 0;
        int flow = 0;
        while (flow < wantedFlow) {
            vector<int> distance(n, inf), previousVertex(n), previousEdge(n);
            priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;
            distance[source] = 0;
            pq.push(make_pair(0, source));
            while (!pq.empty()) {
                int d = pq.top().first;
                int v = pq.top().second;
                pq.pop();
                if (d != distance[v]) continue;
                for (size_t i = 0; i < graph[v].size(); ++i) {
                    const Edge &edge = graph[v][i];
                    if (edge.cap == 0) continue;
                    int nd = d + edge.cost + potential[v] - potential[edge.to];
                    if (nd < distance[edge.to]) {
                        distance[edge.to] = nd;
                        previousVertex[edge.to] = v;
                        previousEdge[edge.to] = (int)i;
                        pq.push(make_pair(nd, edge.to));
                    }
                }
            }
            if (distance[sink] >= inf) break;
            for (int v = 0; v < n; ++v) if (distance[v] < inf) potential[v] += distance[v];
            // Only keep augmenting while it actually helps (true cost of the
            // path, using original costs, is negative); potential[sink] is
            // exactly that true shortest-path cost after the update above.
            if (potential[sink] >= 0) break;
            int add = wantedFlow - flow;
            for (int v = sink; v != source; v = previousVertex[v]) {
                add = min(add, graph[previousVertex[v]][previousEdge[v]].cap);
            }
            flow += add;
            totalCost += add * potential[sink];
            for (int v = sink; v != source; v = previousVertex[v]) {
                Edge &edge = graph[previousVertex[v]][previousEdge[v]];
                edge.cap -= add;
                graph[v][edge.rev].cap += add;
            }
        }
        return totalCost;
    }
};

static int readTime() {
    int h, m, s;
    scanf("%d:%d:%d", &h, &m, &s);
    return h * 3600 + m * 60 + s;
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    vector<int> left(n), right(n), weight(n);
    vector<int> coordinates;
    coordinates.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        left[i] = readTime();
        right[i] = readTime();
        scanf("%d", &weight[i]);
        coordinates.push_back(left[i]);
        coordinates.push_back(right[i]);
    }

    if (k == 0 || n == 0) {
        printf("0\n");
        return 0;
    }

    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    int m = (int)coordinates.size();

    MinCostFlow flow(m);
    for (int i = 0; i + 1 < m; ++i) flow.addEdge(i, i + 1, k, 0);
    for (int i = 0; i < n; ++i) {
        int l = (int)(lower_bound(coordinates.begin(), coordinates.end(), left[i]) - coordinates.begin());
        int r = (int)(lower_bound(coordinates.begin(), coordinates.end(), right[i]) - coordinates.begin());
        flow.addEdge(l, r, 1, -weight[i]);
    }

    printf("%d\n", -flow.minCost(0, m - 1, k));
    return 0;
}
