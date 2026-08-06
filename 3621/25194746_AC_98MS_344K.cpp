// POJ 3621 - Sightseeing Cows
// Model: gpt-5.6-terra
// Approach: Binary-search the maximum cycle fun/time ratio.  For a candidate
// ratio x, edge u->v has weight fun[u] - x*time; a positive-weight cycle
// exists exactly when some valid sightseeing cycle has ratio above x.  SPFA
// starts every vertex at zero so it detects such a cycle in every component.
// Self-loops are ignored because a trip must visit at least two landmarks.

#include <cstdio>
#include <queue>
#include <vector>

struct Edge {
    int to;
    int time;
    Edge(int to_, int time_) : to(to_), time(time_) {}
};

int landmark_count;
std::vector<int> fun;
std::vector< std::vector<Edge> > graph;

bool has_positive_cycle(double ratio) {
    std::vector<double> distance(landmark_count, 0.0);
    std::vector<int> relax_count(landmark_count, 0);
    std::vector<char> in_queue(landmark_count, 1);
    std::queue<int> pending;
    int i;

    for (i = 0; i < landmark_count; ++i) {
        pending.push(i);
    }

    while (!pending.empty()) {
        int from = pending.front();
        std::size_t j;
        pending.pop();
        in_queue[from] = 0;

        for (j = 0; j < graph[from].size(); ++j) {
            const Edge &edge = graph[from][j];
            double candidate = distance[from] + fun[from] - ratio * edge.time;
            if (candidate > distance[edge.to]) {
                distance[edge.to] = candidate;
                ++relax_count[edge.to];
                if (relax_count[edge.to] >= landmark_count) {
                    return true;
                }
                if (!in_queue[edge.to]) {
                    in_queue[edge.to] = 1;
                    pending.push(edge.to);
                }
            }
        }
    }
    return false;
}

int main() {
    int path_count;
    int i;
    double low = 0.0;
    double high = 1000.0;

    if (std::scanf("%d%d", &landmark_count, &path_count) != 2) {
        return 0;
    }
    fun.resize(landmark_count);
    graph.resize(landmark_count);
    for (i = 0; i < landmark_count; ++i) {
        std::scanf("%d", &fun[i]);
    }
    for (i = 0; i < path_count; ++i) {
        int from;
        int to;
        int time;
        std::scanf("%d%d%d", &from, &to, &time);
        --from;
        --to;
        if (from != to) {
            graph[from].push_back(Edge(to, time));
        }
    }

    if (!has_positive_cycle(0.0)) {
        std::printf("0.00\n");
        return 0;
    }
    for (i = 0; i < 50; ++i) {
        double middle = (low + high) / 2.0;
        if (has_positive_cycle(middle)) {
            low = middle;
        } else {
            high = middle;
        }
    }
    std::printf("%.2f\n", low);
    return 0;
}
