// POJ 1698 - Alice's Chance
// Model: gpt-5.6-terra
// Build a flow network: each film supplies its required work days, each eligible
// day before its deadline is an edge, and each calendar day accepts one film.
#include <cstdio>
#include <cstring>
#include <queue>

struct Edge {
    int to;
    int cap;
    int next;
};

const int MAXV = 400;
const int MAXE = 16000;

int head[MAXV], level[MAXV], work[MAXV];
Edge edges[MAXE];
int edgeCount;

void addEdge(int from, int to, int cap) {
    edges[edgeCount].to = to;
    edges[edgeCount].cap = cap;
    edges[edgeCount].next = head[from];
    head[from] = edgeCount++;
    edges[edgeCount].to = from;
    edges[edgeCount].cap = 0;
    edges[edgeCount].next = head[to];
    head[to] = edgeCount++;
}

bool buildLevelGraph(int source, int sink) {
    std::queue<int> q;
    std::memset(level, -1, sizeof(level));
    level[source] = 0;
    q.push(source);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int e = head[v]; e != -1; e = edges[e].next) {
            if (edges[e].cap > 0 && level[edges[e].to] == -1) {
                level[edges[e].to] = level[v] + 1;
                q.push(edges[e].to);
            }
        }
    }
    return level[sink] != -1;
}

int sendFlow(int v, int sink, int flow) {
    if (v == sink) return flow;
    for (int &e = work[v]; e != -1; e = edges[e].next) {
        int to = edges[e].to;
        if (edges[e].cap > 0 && level[to] == level[v] + 1) {
            int pushed = sendFlow(to, sink, flow < edges[e].cap ? flow : edges[e].cap);
            if (pushed > 0) {
                edges[e].cap -= pushed;
                edges[e ^ 1].cap += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int main() {
    int testCases;
    if (std::scanf("%d", &testCases) != 1) return 0;
    while (testCases--) {
        int n;
        std::scanf("%d", &n);
        int available[20][7], needed[20], weeks[20];
        int totalNeeded = 0;
        for (int i = 0; i < n; ++i) {
            for (int d = 0; d < 7; ++d) std::scanf("%d", &available[i][d]);
            std::scanf("%d%d", &needed[i], &weeks[i]);
            totalNeeded += needed[i];
        }

        const int source = 0;
        const int filmBase = 1;
        const int dayBase = filmBase + n;
        const int sink = dayBase + 350;
        std::memset(head, -1, sizeof(head));
        edgeCount = 0;

        for (int i = 0; i < n; ++i) {
            addEdge(source, filmBase + i, needed[i]);
            for (int day = 0; day < weeks[i] * 7; ++day) {
                if (available[i][day % 7]) addEdge(filmBase + i, dayBase + day, 1);
            }
        }
        for (int day = 0; day < 350; ++day) addEdge(dayBase + day, sink, 1);

        int flow = 0;
        while (buildLevelGraph(source, sink)) {
            std::memcpy(work, head, sizeof(head));
            int pushed;
            while ((pushed = sendFlow(source, sink, 1000000000)) > 0) flow += pushed;
        }
        std::puts(flow == totalNeeded ? "Yes" : "No");
    }
    return 0;
}
