// POJ 3099 - Go Go Gorelians
// Model: gpt-5.6-terra
// Approach: Build the conquest tree by linking each new planet to the earliest
// previously conquered planet at minimum squared Euclidean distance.  The
// optimal RGGG planets are the one or two middle vertices of a tree diameter.
// Squared distance preserves all comparisons and avoids floating-point error.

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Planet {
    int id;
    int x;
    int y;
    int z;
};

int squared_distance(const Planet &a, const Planet &b) {
    const int dx = a.x - b.x;
    const int dy = a.y - b.y;
    const int dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

int farthest_vertex(const std::vector< std::vector<int> > &graph, int start,
                    std::vector<int> *parent) {
    const int n = static_cast<int>(graph.size());
    std::vector<int> distance(n, -1);
    std::queue<int> pending;
    pending.push(start);
    distance[start] = 0;
    if (parent != 0) {
        parent->assign(n, -1);
    }

    int farthest = start;
    while (!pending.empty()) {
        const int current = pending.front();
        pending.pop();
        if (distance[current] > distance[farthest]) {
            farthest = current;
        }
        for (std::vector<int>::const_iterator it = graph[current].begin();
             it != graph[current].end(); ++it) {
            const int next = *it;
            if (distance[next] != -1) {
                continue;
            }
            distance[next] = distance[current] + 1;
            if (parent != 0) {
                (*parent)[next] = current;
            }
            pending.push(next);
        }
    }
    return farthest;
}

int main() {
    std::ios::sync_with_stdio(false);

    int n;
    while (std::cin >> n && n != 0) {
        std::vector<Planet> planets(n);
        std::vector< std::vector<int> > graph(n);

        for (int i = 0; i < n; ++i) {
            std::cin >> planets[i].id >> planets[i].x >> planets[i].y
                     >> planets[i].z;
            if (i == 0) {
                continue;
            }

            int nearest = 0;
            int best_distance = squared_distance(planets[i], planets[0]);
            for (int j = 1; j < i; ++j) {
                const int distance = squared_distance(planets[i], planets[j]);
                if (distance < best_distance) {
                    best_distance = distance;
                    nearest = j;
                }
            }
            graph[i].push_back(nearest);
            graph[nearest].push_back(i);
        }

        const int endpoint = farthest_vertex(graph, 0, 0);
        std::vector<int> parent;
        const int other_endpoint = farthest_vertex(graph, endpoint, &parent);

        std::vector<int> diameter;
        for (int vertex = other_endpoint; vertex != -1;
             vertex = parent[vertex]) {
            diameter.push_back(vertex);
        }

        const int first_middle = diameter[(diameter.size() - 1) / 2];
        const int second_middle = diameter[diameter.size() / 2];
        if (first_middle == second_middle) {
            std::cout << planets[first_middle].id << '\n';
        } else {
            int first_id = planets[first_middle].id;
            int second_id = planets[second_middle].id;
            if (first_id > second_id) {
                std::swap(first_id, second_id);
            }
            std::cout << first_id << ' ' << second_id << '\n';
        }
    }
    return 0;
}
