// POJ 2987 - Firing
// Model: gpt-5.6-terra
// Maximum-weight closure: selecting employee i requires every edge i -> j target.
// A minimum s-t cut gives maximum profit; its residual source-reachable side is
// the inclusion-minimal minimum cut, which also minimizes the fired headcount.
#include <cstdio>
#include <cstring>

const int MAXN = 5005;
const int MAXE = 140010;
const long long INF = (1LL << 60);

struct Edge {
    int to;
    int next;
    long long cap;
};

Edge edges[MAXE];
int head[MAXN];
int level[MAXN];
int current[MAXN];
int queue_[MAXN];
int edge_count;
int node_count;
int source;
int sink;

void add_edge(int from, int to, long long cap) {
    edges[edge_count].to = to;
    edges[edge_count].cap = cap;
    edges[edge_count].next = head[from];
    head[from] = edge_count++;
    edges[edge_count].to = from;
    edges[edge_count].cap = 0;
    edges[edge_count].next = head[to];
    head[to] = edge_count++;
}

bool build_level_graph() {
    std::memset(level, -1, sizeof(level));
    int front = 0;
    int back = 0;
    queue_[back++] = source;
    level[source] = 0;
    while (front < back) {
        int u = queue_[front++];
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (edges[e].cap > 0 && level[v] == -1) {
                level[v] = level[u] + 1;
                queue_[back++] = v;
            }
        }
    }
    return level[sink] != -1;
}

long long send_flow(int u, long long limit) {
    if (u == sink) {
        return limit;
    }
    for (int &e = current[u]; e != -1; e = edges[e].next) {
        int v = edges[e].to;
        if (edges[e].cap <= 0 || level[v] != level[u] + 1) {
            continue;
        }
        long long pushed = send_flow(v, limit < edges[e].cap ? limit : edges[e].cap);
        if (pushed != 0) {
            edges[e].cap -= pushed;
            edges[e ^ 1].cap += pushed;
            return pushed;
        }
    }
    return 0;
}

long long max_flow() {
    long long flow = 0;
    while (build_level_graph()) {
        std::memcpy(current, head, sizeof(head));
        while (true) {
            long long pushed = send_flow(source, INF);
            if (pushed == 0) {
                break;
            }
            flow += pushed;
        }
    }
    return flow;
}

int main() {
    int n;
    int m;
    if (std::scanf("%d%d", &n, &m) != 2) {
        return 0;
    }

    source = 0;
    sink = n + 1;
    node_count = n + 2;
    edge_count = 0;
    std::memset(head, -1, sizeof(head));

    long long positive_sum = 0;
    for (int i = 1; i <= n; ++i) {
        long long value;
        std::scanf("%lld", &value);
        if (value > 0) {
            add_edge(source, i, value);
            positive_sum += value;
        } else if (value < 0) {
            add_edge(i, sink, -value);
        }
    }
    for (int k = 0; k < m; ++k) {
        int manager;
        int underling;
        std::scanf("%d%d", &manager, &underling);
        add_edge(manager, underling, INF);
    }

    long long best_profit = positive_sum - max_flow();
    bool reachable[MAXN];
    std::memset(reachable, 0, sizeof(reachable));
    int front = 0;
    int back = 0;
    queue_[back++] = source;
    reachable[source] = true;
    while (front < back) {
        int u = queue_[front++];
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (edges[e].cap > 0 && !reachable[v]) {
                reachable[v] = true;
                queue_[back++] = v;
            }
        }
    }

    int fired = 0;
    for (int i = 1; i < node_count - 1; ++i) {
        if (reachable[i]) {
            ++fired;
        }
    }
    std::printf("%d %lld\n", fired, best_profit);
    return 0;
}
