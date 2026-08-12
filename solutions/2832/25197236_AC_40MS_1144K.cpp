// POJ 2832 - How Many Pairs?
// Model: gpt-5.6-terra
// Sort edges and queries by threshold.  After adding every edge of a given
// length, the disjoint-set components are exactly the vertices connected by
// paths whose every edge is at most that length.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int u, v, w;
};

struct Query {
    int limit, index;
};

bool edgeLess(const Edge &a, const Edge &b) { return a.w < b.w; }
bool queryLess(const Query &a, const Query &b) { return a.limit < b.limit; }

class DisjointSet {
    vector<int> parent, size;
public:
    DisjointSet(int n) : parent(n + 1), size(n + 1, 1) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    long join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return 0;
        if (size[a] < size[b]) swap(a, b);
        long added = (long)size[a] * size[b];
        parent[b] = a;
        size[a] += size[b];
        return added;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, q;
    if (!(cin >> n >> m >> q)) return 0;
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i)
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    vector<Query> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].limit;
        queries[i].index = i;
    }
    sort(edges.begin(), edges.end(), edgeLess);
    sort(queries.begin(), queries.end(), queryLess);

    DisjointSet dsu(n);
    vector<long> answer(q);
    long pairs = 0;
    int edge = 0;
    for (int i = 0; i < q; ++i) {
        while (edge < m && edges[edge].w <= queries[i].limit) {
            pairs += dsu.join(edges[edge].u, edges[edge].v);
            ++edge;
        }
        answer[queries[i].index] = pairs;
    }
    for (int i = 0; i < q; ++i) cout << answer[i] << '\n';
    return 0;
}
