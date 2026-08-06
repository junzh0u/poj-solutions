// POJ 3680 - Intervals
// Model: gpt-5.6-terra
// Compress endpoints and send K units of min-cost flow from the first to the
// last coordinate. Coordinate edges have capacity K and cost zero; choosing an
// interval is a capacity-one forward shortcut with cost -weight. Intervals
// sharing an endpoint may use the same flow unit because the input intervals
// are open, so they do not cover that endpoint.
#include <algorithm>
#include <cstdio>
#include <cstring>
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

        // Before residual edges exist, all usable edges go from a lower to a
        // higher compressed coordinate, so this ordered relaxation gives the
        // initial shortest-path potentials without SPFA.
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
                    int nextDistance = d + edge.cost + potential[v] - potential[edge.to];
                    if (nextDistance < distance[edge.to]) {
                        distance[edge.to] = nextDistance;
                        previousVertex[edge.to] = v;
                        previousEdge[edge.to] = (int)i;
                        pq.push(make_pair(nextDistance, edge.to));
                    }
                }
            }
            for (int v = 0; v < n; ++v) if (distance[v] < inf) potential[v] += distance[v];
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

struct Interval {
    int left, right, weight;
};

int main() {
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int n, k;
        scanf("%d%d", &n, &k);
        vector<Interval> intervals(n);
        vector<int> coordinates;
        for (int i = 0; i < n; ++i) {
            scanf("%d%d%d", &intervals[i].left, &intervals[i].right, &intervals[i].weight);
            coordinates.push_back(intervals[i].left);
            coordinates.push_back(intervals[i].right);
        }
        sort(coordinates.begin(), coordinates.end());
        coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
        int m = (int)coordinates.size();
        MinCostFlow flow(m);
        for (int i = 0; i + 1 < m; ++i) flow.addEdge(i, i + 1, k, 0);
        for (int i = 0; i < n; ++i) {
            int left = (int)(lower_bound(coordinates.begin(), coordinates.end(), intervals[i].left) - coordinates.begin());
            int right = (int)(lower_bound(coordinates.begin(), coordinates.end(), intervals[i].right) - coordinates.begin());
            flow.addEdge(left, right, 1, -intervals[i].weight);
        }
        printf("%d\n", -flow.minCost(0, m - 1, k));
    }
    return 0;
}
