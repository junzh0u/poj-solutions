// POJ 3594 - Escort of Dr. Who How
// Model: gpt-5.6-terra
// For every possible departure time, Dijkstra finds the earliest arrival in
// the FIFO time-window graph.  The minimum arrival-minus-departure is kept.
// A road may be entered at max(arrival, opening), and must be left by closing.

#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    int begin;
    int end;
    int cost;
};

struct State {
    int time;
    int vertex;
    State(int time_, int vertex_) : time(time_), vertex(vertex_) {}
    bool operator<(const State& other) const {
        return time > other.time;
    }
};

int main() {
    int n, m, source, target;
    if (scanf("%d%d%d%d", &n, &m, &source, &target) != 4) return 0;

    vector< vector<Edge> > graph(n + 1);
    int latest = 0;
    int i;
    for (i = 0; i < m; ++i) {
        int from, to, begin, end, cost;
        scanf("%d%d%d%d%d", &from, &to, &begin, &end, &cost);
        if (end - begin >= cost) {
            Edge edge;
            edge.to = to;
            edge.begin = begin;
            edge.end = end;
            edge.cost = cost;
            graph[from].push_back(edge);
            if (end > latest) latest = end;
        }
    }

    const int INF = 0x3f3f3f3f;
    int answer = INF;
    int start;
    for (start = 0; start <= latest; ++start) {
        vector<int> dist(n + 1, INF);
        priority_queue<State> pq;
        dist[source] = start;
        pq.push(State(start, source));

        while (!pq.empty()) {
            State current = pq.top();
            pq.pop();
            if (current.time != dist[current.vertex]) continue;
            int j;
            for (j = 0; j < (int)graph[current.vertex].size(); ++j) {
                const Edge& edge = graph[current.vertex][j];
                int leave = max(current.time, edge.begin);
                int arrive = leave + edge.cost;
                if (arrive <= edge.end && arrive < dist[edge.to]) {
                    dist[edge.to] = arrive;
                    pq.push(State(arrive, edge.to));
                }
            }
        }

        if (dist[target] == INF) break;
        if (dist[target] - start < answer) answer = dist[target] - start;
    }

    if (answer == INF) puts("Impossible");
    else printf("%d\n", answer);
    return 0;
}
