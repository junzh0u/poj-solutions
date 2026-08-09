// POJ 3653 - Here We Go(relians) Again
// Model: gpt-5.6-terra
// Dijkstra on the intersections: every open street adds one or two directed
// edges with weight 2520 divided by its integral speed limit.
// The input alternates complete horizontal rows and, except after the last,
// complete vertical rows.

#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

struct Edge {
    int to;
    int cost;
    Edge(int t, int c) : to(t), cost(c) {}
};

static void addStreet(vector<Edge> graph[], int a, int b, int speed, char dir, char forward, char backward) {
    if (speed == 0) return;
    int cost = 2520 / speed;
    if (dir == '*' || dir == forward) graph[a].push_back(Edge(b, cost));
    if (dir == '*' || dir == backward) graph[b].push_back(Edge(a, cost));
}

int main() {
    int rows, cols;
    while (scanf("%d%d", &rows, &cols) == 2 && (rows != 0 || cols != 0)) {
        const int width = cols + 1;
        const int nodes = (rows + 1) * width;
        vector<Edge> graph[441];
        int r, c, speed;
        char dir[8];
        for (r = 0; r <= rows; ++r) {
            for (c = 0; c < cols; ++c) {
                scanf("%d%s", &speed, dir);
                int west = r * width + c;
                addStreet(graph, west, west + 1, speed, dir[0], '>', '<');
            }
            if (r == rows) continue;
            for (c = 0; c <= cols; ++c) {
                scanf("%d%s", &speed, dir);
                int north = r * width + c;
                addStreet(graph, north, north + width, speed, dir[0], 'v', '^');
            }
        }

        const int inf = 0x3f3f3f3f;
        int dist[441];
        memset(dist, 0x3f, sizeof(dist));
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        dist[0] = 0;
        pq.push(make_pair(0, 0));
        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            if (d != dist[u]) continue;
            int i;
            for (i = 0; i < (int)graph[u].size(); ++i) {
                const Edge &edge = graph[u][i];
                if (dist[edge.to] > d + edge.cost) {
                    dist[edge.to] = d + edge.cost;
                    pq.push(make_pair(dist[edge.to], edge.to));
                }
            }
        }
        if (dist[nodes - 1] == inf) puts("Holiday");
        else printf("%d blips\n", dist[nodes - 1]);
    }
    return 0;
}
