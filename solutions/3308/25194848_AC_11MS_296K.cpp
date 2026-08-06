// POJ 3308 - Paratroopers
// Model: gpt-5.6-terra
// Taking logs changes the minimum product vertex cover into a minimum-weight
// bipartite vertex cover. A source-row-column-sink min cut finds that cover.
// Costs below one, though outside the stated range, are selected immediately
// because their negative logarithm can only improve every feasible cover.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;

const double EPS = 1e-10;

struct Edge {
    int to;
    int rev;
    double cap;
    Edge(int to_, int rev_, double cap_) : to(to_), rev(rev_), cap(cap_) {}
};

class Dinic {
public:
    explicit Dinic(int vertices) : graph(vertices), level(vertices), work(vertices) {}

    void add_edge(int from, int to, double cap) {
        graph[from].push_back(Edge(to, (int)graph[to].size(), cap));
        graph[to].push_back(Edge(from, (int)graph[from].size() - 1, 0.0));
    }

    double max_flow(int source, int sink) {
        double result = 0.0;
        while (build_level_graph(source, sink)) {
            fill(work.begin(), work.end(), 0);
            double pushed;
            while ((pushed = send_flow(source, sink, 1e100)) > EPS) {
                result += pushed;
            }
        }
        return result;
    }

private:
    vector< vector<Edge> > graph;
    vector<int> level;
    vector<int> work;

    bool build_level_graph(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            int i;
            for (i = 0; i < (int)graph[v].size(); ++i) {
                const Edge &e = graph[v][i];
                if (e.cap > EPS && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[sink] >= 0;
    }

    double send_flow(int v, int sink, double limit) {
        if (v == sink) return limit;
        int &i = work[v];
        for (; i < (int)graph[v].size(); ++i) {
            Edge &e = graph[v][i];
            if (e.cap <= EPS || level[e.to] != level[v] + 1) continue;
            double pushed = send_flow(e.to, sink, limit < e.cap ? limit : e.cap);
            if (pushed > EPS) {
                e.cap -= pushed;
                graph[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
        return 0.0;
    }
};

int main() {
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int rows, columns, landings;
        scanf("%d%d%d", &rows, &columns, &landings);
        vector<double> row_cost(rows);
        vector<double> column_cost(columns);
        vector<bool> forced_row(rows, false);
        vector<bool> forced_column(columns, false);
        double forced_log_cost = 0.0;
        int i;
        for (i = 0; i < rows; ++i) {
            scanf("%lf", &row_cost[i]);
            if (row_cost[i] < 1.0) {
                forced_row[i] = true;
                forced_log_cost += log(row_cost[i]);
            }
        }
        for (i = 0; i < columns; ++i) {
            scanf("%lf", &column_cost[i]);
            if (column_cost[i] < 1.0) {
                forced_column[i] = true;
                forced_log_cost += log(column_cost[i]);
            }
        }

        vector< pair<int, int> > uncovered_landings;
        for (i = 0; i < landings; ++i) {
            int row, column;
            scanf("%d%d", &row, &column);
            --row;
            --column;
            if (!forced_row[row] && !forced_column[column]) {
                uncovered_landings.push_back(make_pair(row, column));
            }
        }

        double finite_sum = 0.0;
        for (i = 0; i < rows; ++i) {
            if (!forced_row[i]) finite_sum += log(row_cost[i]);
        }
        for (i = 0; i < columns; ++i) {
            if (!forced_column[i]) finite_sum += log(column_cost[i]);
        }
        double infinity = finite_sum + 1.0;
        int source = rows + columns;
        int sink = source + 1;
        Dinic flow(sink + 1);
        for (i = 0; i < rows; ++i) {
            if (!forced_row[i]) flow.add_edge(source, i, log(row_cost[i]));
        }
        for (i = 0; i < columns; ++i) {
            if (!forced_column[i]) flow.add_edge(rows + i, sink, log(column_cost[i]));
        }
        for (i = 0; i < (int)uncovered_landings.size(); ++i) {
            flow.add_edge(uncovered_landings[i].first,
                          rows + uncovered_landings[i].second, infinity);
        }
        double answer = exp(forced_log_cost + flow.max_flow(source, sink));
        printf("%.4f\n", answer);
    }
    return 0;
}
