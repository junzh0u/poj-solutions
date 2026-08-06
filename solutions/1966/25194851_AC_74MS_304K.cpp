// POJ 1966 - Cable TV Network
// Model: gpt-5.6-terra
// Compute vertex connectivity by trying every pair of retained relays as flow endpoints.
// Split each other relay into an in-to-out edge of capacity one; cable directions have infinite capacity.
// Self-loop cables are ignored because they do not change connectivity.

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;

const int INF = 1000000;

struct Edge {
    int to;
    int capacity;
    int reverse;
};

class Dinic {
public:
    explicit Dinic(int count) : graph(count), level(count), position(count) {
    }

    void addEdge(int from, int to, int capacity) {
        Edge forward;
        Edge backward;
        forward.to = to;
        forward.capacity = capacity;
        forward.reverse = (int)graph[to].size();
        backward.to = from;
        backward.capacity = 0;
        backward.reverse = (int)graph[from].size();
        graph[from].push_back(forward);
        graph[to].push_back(backward);
    }

    int maxFlow(int source, int sink, int limit) {
        int flow = 0;
        while (flow < limit && buildLevels(source, sink)) {
            fill(position.begin(), position.end(), 0);
            while (flow < limit) {
                int added = sendFlow(source, sink, limit - flow);
                if (added == 0) {
                    break;
                }
                flow += added;
            }
        }
        return flow;
    }

private:
    vector< vector<Edge> > graph;
    vector<int> level;
    vector<int> position;

    bool buildLevels(int source, int sink) {
        queue<int> pending;
        fill(level.begin(), level.end(), -1);
        level[source] = 0;
        pending.push(source);
        while (!pending.empty()) {
            int vertex = pending.front();
            size_t index;
            pending.pop();
            for (index = 0; index < graph[vertex].size(); ++index) {
                Edge const &edge = graph[vertex][index];
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[vertex] + 1;
                    pending.push(edge.to);
                }
            }
        }
        return level[sink] != -1;
    }

    int sendFlow(int vertex, int sink, int available) {
        while (position[vertex] < (int)graph[vertex].size()) {
            Edge &edge = graph[vertex][position[vertex]];
            if (edge.capacity > 0 && level[edge.to] == level[vertex] + 1) {
                int sent;
                if (edge.to == sink) {
                    sent = min(available, edge.capacity);
                } else {
                    sent = sendFlow(edge.to, sink, min(available, edge.capacity));
                }
                if (sent > 0) {
                    edge.capacity -= sent;
                    graph[edge.to][edge.reverse].capacity += sent;
                    return sent;
                }
            }
            ++position[vertex];
        }
        return 0;
    }
};

int main() {
    int n;
    int cables;
    while (scanf("%d%d", &n, &cables) == 2) {
        bool connected[50][50];
        int cable;
        int source;
        int sink;
        int answer = n;
        memset(connected, 0, sizeof(connected));
        for (cable = 0; cable < cables; ++cable) {
            int from;
            int to;
            scanf(" (%d,%d)", &from, &to);
            if (from != to) {
                connected[from][to] = true;
                connected[to][from] = true;
            }
        }
        for (source = 0; source < n; ++source) {
            for (sink = source + 1; sink < n; ++sink) {
                Dinic flow(2 * n);
                int vertex;
                int result;
                for (vertex = 0; vertex < n; ++vertex) {
                    flow.addEdge(vertex, vertex + n,
                                 vertex == source || vertex == sink ? INF : 1);
                }
                for (vertex = 0; vertex < n; ++vertex) {
                    int other;
                    for (other = vertex + 1; other < n; ++other) {
                        if (connected[vertex][other]) {
                            flow.addEdge(vertex + n, other, INF);
                            flow.addEdge(other + n, vertex, INF);
                        }
                    }
                }
                result = flow.maxFlow(source + n, sink, answer);
                answer = min(answer, result);
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
