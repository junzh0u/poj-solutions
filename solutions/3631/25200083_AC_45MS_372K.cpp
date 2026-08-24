// POJ 3631 - Cuckoo Hashing
// Model: gpt-5.6-terra
// Treat hash locations as graph vertices and dictionary words as edges.  A
// component can hold all of its words exactly when its edge count does not
// exceed its vertex count; union-find maintains that invariant as edges arrive.
// A pair of identical hashes is a self-loop and is handled by the same count.

#include <iostream>
#include <vector>

using namespace std;

class DisjointSet {
    vector<int> parent;
    vector<int> size;
    vector<int> edges;

public:
    DisjointSet(int n) : parent(n), size(n, 1), edges(n, 0) {
        int i;
        for (i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool addEdge(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) {
            ++edges[ra];
            return edges[ra] <= size[ra];
        }
        if (size[ra] < size[rb]) {
            int tmp = ra;
            ra = rb;
            rb = tmp;
        }
        parent[rb] = ra;
        size[ra] += size[rb];
        edges[ra] += edges[rb] + 1;
        return edges[ra] <= size[ra];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int tests;
    cin >> tests;
    while (tests--) {
        int m, n, i;
        cin >> m >> n;
        DisjointSet dsu(n);
        bool possible = true;
        for (i = 0; i < m; ++i) {
            int h1, h2;
            cin >> h1 >> h2;
            if (!dsu.addEdge(h1, h2)) possible = false;
        }
        cout << (possible ? "successful hashing" : "rehash necessary") << '\n';
    }
    return 0;
}
