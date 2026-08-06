// POJ 2394 - Checking an Alibi
// Model: gpt-5.6-terra
// Approach: Run Dijkstra once from the barn (field 1) on the undirected path
// graph, then report every cow whose shortest travel time is at most M.
// Multiple paths and multiple cows in one field are represented directly; an
// unreachable field is not reported because its distance remains infinite.
#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
#include <functional>

using namespace std;

struct Edge {
    int to;
    int cost;
    Edge(int to_, int cost_) : to(to_), cost(cost_) {}
};

int main() {
    int field_count, path_count, cow_count;
    int deadline;
    if (scanf("%d%d%d%d", &field_count, &path_count, &cow_count, &deadline) != 4) {
        return 0;
    }

    vector< vector<Edge> > graph(field_count + 1);
    int a, b, travel;
    for (int i = 0; i < path_count; ++i) {
        scanf("%d%d%d", &a, &b, &travel);
        graph[a].push_back(Edge(b, travel));
        graph[b].push_back(Edge(a, travel));
    }

    const int inf = 1000000000;
    vector<int> dist(field_count + 1, inf);
    typedef pair<int, int> State;
    priority_queue<State, vector<State>, greater<State> > queue;
    dist[1] = 0;
    queue.push(State(0, 1));
    while (!queue.empty()) {
        State current = queue.top();
        queue.pop();
        int current_dist = current.first;
        int node = current.second;
        if (current_dist != dist[node]) {
            continue;
        }
        for (size_t i = 0; i < graph[node].size(); ++i) {
            const Edge &edge = graph[node][i];
            int candidate = current_dist + edge.cost;
            if (candidate < dist[edge.to]) {
                dist[edge.to] = candidate;
                queue.push(State(candidate, edge.to));
            }
        }
    }

    vector<int> guilty;
    for (int i = 1; i <= cow_count; ++i) {
        scanf("%d", &a);
        if (dist[a] <= deadline) {
            guilty.push_back(i);
        }
    }
    printf("%d\n", static_cast<int>(guilty.size()));
    for (size_t i = 0; i < guilty.size(); ++i) {
        printf("%d\n", guilty[i]);
    }
    return 0;
}
