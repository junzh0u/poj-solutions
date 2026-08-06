// POJ 3436 - ACM Computer Factory
// Model: gpt-5.6-terra
// Split each machine into an input and an output node with its performance as
// the connecting capacity.  Source edges represent machines that can start
// from an empty computer; sink edges represent machines that make all parts.
// An inter-machine edge exists exactly when the producer's output satisfies
// every required-present/required-absent input part of the consumer.
// Input contains multiple test cases, terminated by EOF.

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int MAX_N = 50;
const int MAX_P = 10;
const int INF = 1000000000;

struct Edge {
    int to;
    int rev;
    int cap;
    int original;
};

class Dinic {
public:
    explicit Dinic(int n) : graph(n), level(n), iter(n) {}

    int addEdge(int from, int to, int cap) {
        Edge forward;
        forward.to = to;
        forward.rev = static_cast<int>(graph[to].size());
        forward.cap = cap;
        forward.original = cap;
        Edge backward;
        backward.to = from;
        backward.rev = static_cast<int>(graph[from].size());
        backward.cap = 0;
        backward.original = 0;
        graph[from].push_back(forward);
        graph[to].push_back(backward);
        return static_cast<int>(graph[from].size()) - 1;
    }

    int maxFlow(int source, int sink) {
        int result = 0;
        while (buildLevelGraph(source, sink)) {
            fill(iter.begin(), iter.end(), 0);
            int pushed;
            while ((pushed = sendFlow(source, sink, INF)) > 0) {
                result += pushed;
            }
        }
        return result;
    }

    int flowOn(int from, int edgeIndex) const {
        const Edge &edge = graph[from][edgeIndex];
        return edge.original - edge.cap;
    }

private:
    vector< vector<Edge> > graph;
    vector<int> level;
    vector<int> iter;

    bool buildLevelGraph(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            int i;
            for (i = 0; i < static_cast<int>(graph[v].size()); ++i) {
                const Edge &edge = graph[v][i];
                if (edge.cap > 0 && level[edge.to] < 0) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[sink] >= 0;
    }

    int sendFlow(int v, int sink, int flow) {
        if (v == sink) {
            return flow;
        }
        int &i = iter[v];
        while (i < static_cast<int>(graph[v].size())) {
            Edge &edge = graph[v][i];
            if (edge.cap > 0 && level[v] < level[edge.to]) {
                int pushed = sendFlow(edge.to, sink,
                                      flow < edge.cap ? flow : edge.cap);
                if (pushed > 0) {
                    edge.cap -= pushed;
                    graph[edge.to][edge.rev].cap += pushed;
                    return pushed;
                }
            }
            ++i;
        }
        return 0;
    }
};

struct Machine {
    int performance;
    int input[MAX_P];
    int output[MAX_P];
};

struct Connection {
    int from;
    int to;
    int edgeIndex;
};

bool canStart(const Machine &machine, int parts) {
    int k;
    for (k = 0; k < parts; ++k) {
        if (machine.input[k] == 1) {
            return false;
        }
    }
    return true;
}

bool canFinish(const Machine &machine, int parts) {
    int k;
    for (k = 0; k < parts; ++k) {
        if (machine.output[k] == 0) {
            return false;
        }
    }
    return true;
}

bool canConnect(const Machine &from, const Machine &to, int parts) {
    int k;
    for (k = 0; k < parts; ++k) {
        if (to.input[k] != 2 && to.input[k] != from.output[k]) {
            return false;
        }
    }
    return true;
}

int main() {
    int parts;
    int count;
    while (scanf("%d%d", &parts, &count) == 2) {
        Machine machines[MAX_N];
        int i;
        int j;
        for (i = 0; i < count; ++i) {
            scanf("%d", &machines[i].performance);
            for (j = 0; j < parts; ++j) {
                scanf("%d", &machines[i].input[j]);
            }
            for (j = 0; j < parts; ++j) {
                scanf("%d", &machines[i].output[j]);
            }
        }

        int source = 2 * count;
        int sink = source + 1;
        Dinic flow(2 * count + 2);
        vector<Connection> connections;
        for (i = 0; i < count; ++i) {
            flow.addEdge(2 * i, 2 * i + 1, machines[i].performance);
            if (canStart(machines[i], parts)) {
                flow.addEdge(source, 2 * i, INF);
            }
            if (canFinish(machines[i], parts)) {
                flow.addEdge(2 * i + 1, sink, INF);
            }
        }
        for (i = 0; i < count; ++i) {
            for (j = 0; j < count; ++j) {
                if (i != j && canConnect(machines[i], machines[j], parts)) {
                    Connection connection;
                    connection.from = i;
                    connection.to = j;
                    connection.edgeIndex = flow.addEdge(2 * i + 1, 2 * j, INF);
                    connections.push_back(connection);
                }
            }
        }

        int answer = flow.maxFlow(source, sink);
        vector<Connection> used;
        for (i = 0; i < static_cast<int>(connections.size()); ++i) {
            if (flow.flowOn(2 * connections[i].from + 1,
                            connections[i].edgeIndex) > 0) {
                used.push_back(connections[i]);
            }
        }
        printf("%d %d\n", answer, static_cast<int>(used.size()));
        for (i = 0; i < static_cast<int>(used.size()); ++i) {
            int amount = flow.flowOn(2 * used[i].from + 1, used[i].edgeIndex);
            printf("%d %d %d\n", used[i].from + 1, used[i].to + 1, amount);
        }
    }
    return 0;
}
