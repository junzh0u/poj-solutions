// POJ 1984 - Navigation Nightmare
// Model: gpt-5.6-terra
// Process roads in the order they become known. A weighted disjoint-set stores
// each farm's coordinate offset to its component root, so a connected query is
// answered by the Manhattan norm of the difference of those offsets.
// Queries are sorted by their requested data index but printed in input order;
// this avoids relying on the input queries being ordered by that index.
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct Road {
    int a;
    int b;
    int x;
    int y;
};

struct Query {
    int a;
    int b;
    int index;
    int order;
};

bool by_index(const Query& left, const Query& right) {
    return left.index < right.index;
}

class WeightedDisjointSet {
public:
    WeightedDisjointSet(int count)
        : parent(count + 1), rank(count + 1, 0), x_offset(count + 1, 0),
          y_offset(count + 1, 0) {
        int i;
        for (i = 1; i <= count; ++i) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] == node) {
            return node;
        }
        int previous_parent = parent[node];
        int root = find(previous_parent);
        x_offset[node] += x_offset[previous_parent];
        y_offset[node] += y_offset[previous_parent];
        parent[node] = root;
        return root;
    }

    void connect(int a, int b, int delta_x, int delta_y) {
        int root_a = find(a);
        int root_b = find(b);
        if (root_a == root_b) {
            return;
        }

        // position(root_a) - position(root_b), inferred from b - a = delta.
        int root_x = x_offset[b] - x_offset[a] - delta_x;
        int root_y = y_offset[b] - y_offset[a] - delta_y;

        if (rank[root_a] < rank[root_b]) {
            parent[root_a] = root_b;
            x_offset[root_a] = root_x;
            y_offset[root_a] = root_y;
        } else {
            parent[root_b] = root_a;
            x_offset[root_b] = -root_x;
            y_offset[root_b] = -root_y;
            if (rank[root_a] == rank[root_b]) {
                ++rank[root_a];
            }
        }
    }

    int distance(int a, int b) {
        if (find(a) != find(b)) {
            return -1;
        }
        return abs(x_offset[a] - x_offset[b]) + abs(y_offset[a] - y_offset[b]);
    }

private:
    vector<int> parent;
    vector<int> rank;
    vector<int> x_offset;
    vector<int> y_offset;
};

int main() {
    int farm_count;
    int road_count;
    if (!(cin >> farm_count >> road_count)) {
        return 0;
    }

    vector<Road> roads(road_count);
    int i;
    for (i = 0; i < road_count; ++i) {
        char direction;
        cin >> roads[i].a >> roads[i].b >> roads[i].x >> direction;
        roads[i].y = 0;
        if (direction == 'N') {
            roads[i].y = roads[i].x;
            roads[i].x = 0;
        } else if (direction == 'S') {
            roads[i].y = -roads[i].x;
            roads[i].x = 0;
        } else if (direction == 'W') {
            roads[i].x = -roads[i].x;
        }
    }

    int query_count;
    cin >> query_count;
    vector<Query> queries(query_count);
    for (i = 0; i < query_count; ++i) {
        cin >> queries[i].a >> queries[i].b >> queries[i].index;
        queries[i].order = i;
    }
    sort(queries.begin(), queries.end(), by_index);

    WeightedDisjointSet sets(farm_count);
    vector<int> answers(query_count);
    int next_road = 0;
    for (i = 0; i < query_count; ++i) {
        while (next_road < queries[i].index) {
            const Road& road = roads[next_road];
            sets.connect(road.a, road.b, road.x, road.y);
            ++next_road;
        }
        answers[queries[i].order] = sets.distance(queries[i].a, queries[i].b);
    }

    for (i = 0; i < query_count; ++i) {
        cout << answers[i] << '\n';
    }
    return 0;
}
