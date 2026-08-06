// POJ 2263 - Heavy Cargo
// Model: gpt-5.6-terra
// Sort roads from highest limit to lowest and join their endpoints with DSU.
// The first threshold that connects the requested cities is their maximum bottleneck load.
// Parallel roads are retained as independent edges; the output deliberately has no colon after Scenario #x.
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int a;
    int b;
    int limit;
};

struct DescendingLimit {
    bool operator()(const Edge& x, const Edge& y) const {
        return x.limit > y.limit;
    }
};

struct DisjointSet {
    vector<int> parent;
    vector<int> size;

    DisjointSet(int n) : parent(n), size(n, 1) {
        int i;
        for (i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, r, scenario = 1;
    while (cin >> n >> r && (n != 0 || r != 0)) {
        map<string, int> id;
        vector<Edge> edges;
        int nextId = 0;
        int i;
        for (i = 0; i < r; ++i) {
            string u, v;
            int limit;
            cin >> u >> v >> limit;
            if (id.find(u) == id.end()) id[u] = nextId++;
            if (id.find(v) == id.end()) id[v] = nextId++;
            Edge e;
            e.a = id[u];
            e.b = id[v];
            e.limit = limit;
            edges.push_back(e);
        }
        string start, destination;
        cin >> start >> destination;
        if (id.find(start) == id.end()) id[start] = nextId++;
        if (id.find(destination) == id.end()) id[destination] = nextId++;

        sort(edges.begin(), edges.end(), DescendingLimit());
        DisjointSet sets(n + 2);
        int answer = 0;
        for (i = 0; i < (int)edges.size(); ++i) {
            sets.join(edges[i].a, edges[i].b);
            if (sets.find(id[start]) == sets.find(id[destination])) {
                answer = edges[i].limit;
                break;
            }
        }
        cout << "Scenario #" << scenario++ << "\n";
        cout << answer << " tons\n\n";
    }
    return 0;
}
