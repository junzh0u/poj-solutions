// POJ 2784 - Buy or Build
// Model: gpt-5.6-terra
// Build the complete geometric graph's MST with Prim. Buying subnetworks adds
// zero-cost connectivity; after contracting it, some MST of the augmented graph
// uses only original MST edges. Enumerate all purchases and reconnect by DSU.

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
};

struct Network {
    int cost;
    vector<int> city;
};

struct Edge {
    int first;
    int second;
    int cost;
};

bool edge_cost_less(const Edge &first, const Edge &second) {
    return first.cost < second.cost;
}

class DisjointSet {
public:
    explicit DisjointSet(int count) : parent(count), rank(count, 0) {
        for (int i = 0; i < count; ++i) {
            parent[i] = i;
        }
    }

    int find(int value) {
        if (parent[value] != value) {
            parent[value] = find(parent[value]);
        }
        return parent[value];
    }

    bool unite(int first, int second) {
        first = find(first);
        second = find(second);
        if (first == second) {
            return false;
        }
        if (rank[first] < rank[second]) {
            swap(first, second);
        }
        parent[second] = first;
        if (rank[first] == rank[second]) {
            ++rank[first];
        }
        return true;
    }

private:
    vector<int> parent;
    vector<int> rank;
};

int squared_distance(const Point &first, const Point &second) {
    const int dx = first.x - second.x;
    const int dy = first.y - second.y;
    return dx * dx + dy * dy;
}

long long build_mst(const vector<Point> &point, vector<Edge> &tree) {
    const int city_count = static_cast<int>(point.size());
    const int INFINITY_COST = 2000000001;
    vector<int> distance(city_count, INFINITY_COST);
    vector<int> predecessor(city_count, -1);
    vector<bool> used(city_count, false);
    distance[0] = 0;

    long long total = 0;
    for (int step = 0; step < city_count; ++step) {
        int city = -1;
        for (int candidate = 0; candidate < city_count; ++candidate) {
            if (!used[candidate] &&
                (city == -1 || distance[candidate] < distance[city])) {
                city = candidate;
            }
        }

        used[city] = true;
        total += distance[city];
        if (predecessor[city] != -1) {
            Edge edge;
            edge.first = city;
            edge.second = predecessor[city];
            edge.cost = distance[city];
            tree.push_back(edge);
        }

        for (int candidate = 0; candidate < city_count; ++candidate) {
            if (!used[candidate]) {
                const int cost = squared_distance(point[city], point[candidate]);
                if (cost < distance[candidate]) {
                    distance[candidate] = cost;
                    predecessor[candidate] = city;
                }
            }
        }
    }
    return total;
}

int main() {
    int city_count;
    int network_count;
    if (!(cin >> city_count >> network_count)) {
        return 0;
    }

    vector<Network> network(network_count);
    for (int i = 0; i < network_count; ++i) {
        int member_count;
        cin >> member_count >> network[i].cost;
        network[i].city.resize(member_count);
        for (int j = 0; j < member_count; ++j) {
            cin >> network[i].city[j];
            --network[i].city[j];
        }
    }

    vector<Point> point(city_count);
    for (int i = 0; i < city_count; ++i) {
        cin >> point[i].x >> point[i].y;
    }

    vector<Edge> tree;
    long long answer = build_mst(point, tree);
    sort(tree.begin(), tree.end(), edge_cost_less);

    const int purchase_sets = 1 << network_count;
    for (int mask = 1; mask < purchase_sets; ++mask) {
        DisjointSet components(city_count);
        long long total = 0;

        for (int i = 0; i < network_count; ++i) {
            if ((mask & (1 << i)) != 0) {
                total += network[i].cost;
                const vector<int> &member = network[i].city;
                for (size_t j = 1; j < member.size(); ++j) {
                    components.unite(member[0], member[j]);
                }
            }
        }
        if (total >= answer) {
            continue;
        }

        for (size_t i = 0; i < tree.size(); ++i) {
            if (components.unite(tree[i].first, tree[i].second)) {
                total += tree[i].cost;
            }
        }
        answer = min(answer, total);
    }

    cout << answer << '\n';
    return 0;
}
