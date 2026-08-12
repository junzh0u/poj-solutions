// POJ 3357 - Oreon
// Model: gpt-5.6-terra
// Approach: Build the undirected edge list from the lower triangle of the
// adjacency matrix, then run Kruskal's algorithm.  Sorting first by personnel
// count and then by endpoint names implements the requested lexicographically
// least minimum spanning tree and also leaves the selected edges in output
// order.  Matrix entries may be comma-separated or whitespace-separated, so
// each row is parsed character by character.

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int u;
    int v;
    int cost;
};

bool edgeLess(const Edge &a, const Edge &b) {
    if (a.cost != b.cost) return a.cost < b.cost;
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

struct DisjointSet {
    vector<int> parent;

    DisjointSet(int n) : parent(n) {
        int i;
        for (i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        parent[a] = b;
        return true;
    }
};

int main() {
    int cases;
    int tc;
    if (!(cin >> cases)) return 0;
    for (tc = 1; tc <= cases; ++tc) {
        int n;
        int i;
        int j;
        vector<Edge> edges;
        cin >> n;
        for (i = 0; i < n; ++i) {
            string row;
            cin >> ws;
            getline(cin, row);
            int value = 0;
            bool inNumber = false;
            j = 0;
            int col = 0;
            while (j <= static_cast<int>(row.size())) {
                if (j < static_cast<int>(row.size()) &&
                    isdigit(static_cast<unsigned char>(row[j]))) {
                    value = value * 10 + row[j] - '0';
                    inNumber = true;
                } else if (inNumber) {
                    if (col < i && value != 0) {
                        Edge e;
                        e.u = col;
                        e.v = i;
                        e.cost = value;
                        edges.push_back(e);
                    }
                    ++col;
                    value = 0;
                    inNumber = false;
                }
                ++j;
            }
        }

        sort(edges.begin(), edges.end(), edgeLess);
        DisjointSet dsu(n);
        cout << "Case " << tc << ":\n";
        for (i = 0; i < static_cast<int>(edges.size()); ++i) {
            if (dsu.unite(edges[i].u, edges[i].v)) {
                cout << static_cast<char>('A' + edges[i].u) << '-'
                     << static_cast<char>('A' + edges[i].v) << ' '
                     << edges[i].cost << '\n';
            }
        }
    }
    return 0;
}
