// POJ 2838 - Graph Connectivity
// Model: gpt-5.6-terra
// Offline dynamic connectivity: assign every edge its active interval, place
// intervals in a segment tree over command time, and answer queries with a
// rollback union-find during a depth-first traversal.  The graph is undirected.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const int MAXQ = 20005;

struct Change {
    int a, b, sizeA;
    Change(int aa = -1, int bb = -1, int ss = 0) : a(aa), b(bb), sizeA(ss) {}
};

struct RollbackDSU {
    int parent[MAXN], sz[MAXN];
    vector<Change> history;

    void init(int n) {
        history.clear();
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            sz[i] = 1;
        }
    }
    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }
    int snapshot() const { return (int)history.size(); }
    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            history.push_back(Change());
            return;
        }
        if (sz[x] < sz[y]) swap(x, y);
        history.push_back(Change(x, y, sz[x]));
        parent[y] = x;
        sz[x] += sz[y];
    }
    void rollback(int at) {
        while ((int)history.size() > at) {
            Change c = history.back();
            history.pop_back();
            if (c.a == -1) continue;
            parent[c.b] = c.b;
            sz[c.a] = c.sizeA;
        }
    }
};

struct Edge {
    int u, v;
    Edge(int uu = 0, int vv = 0) : u(uu), v(vv) {}
};

char op[MAXQ];
int x[MAXQ], y[MAXQ];
vector<Edge> tree[MAXQ * 4];
RollbackDSU dsu;

void add(int node, int left, int right, int ql, int qr, const Edge &e) {
    if (ql <= left && right <= qr) {
        tree[node].push_back(e);
        return;
    }
    int mid = (left + right) / 2;
    if (ql <= mid) add(node * 2, left, mid, ql, qr, e);
    if (qr > mid) add(node * 2 + 1, mid + 1, right, ql, qr, e);
}

void solve(int node, int left, int right) {
    int saved = dsu.snapshot();
    for (size_t i = 0; i < tree[node].size(); ++i)
        dsu.unite(tree[node][i].u, tree[node][i].v);
    if (left == right) {
        if (op[left] == 'Q')
            puts(dsu.find(x[left]) == dsu.find(y[left]) ? "Y" : "N");
    } else {
        int mid = (left + right) / 2;
        solve(node * 2, left, mid);
        solve(node * 2 + 1, mid + 1, right);
    }
    dsu.rollback(saved);
}

int main() {
    int n, q;
    if (scanf("%d%d", &n, &q) != 2) return 0;
    map< pair<int, int>, int > insertedAt;
    for (int i = 1; i <= q; ++i) {
        scanf(" %c%d%d", &op[i], &x[i], &y[i]);
        if (x[i] > y[i]) swap(x[i], y[i]);
        pair<int, int> e = make_pair(x[i], y[i]);
        if (op[i] == 'I') {
            insertedAt[e] = i;
        } else if (op[i] == 'D') {
            int start = insertedAt[e];
            if (start <= i - 1) add(1, 1, q, start, i - 1, Edge(x[i], y[i]));
            insertedAt.erase(e);
        }
    }
    for (map< pair<int, int>, int >::iterator it = insertedAt.begin(); it != insertedAt.end(); ++it)
        add(1, 1, q, it->second, q, Edge(it->first.first, it->first.second));
    dsu.init(n);
    solve(1, 1, q);
    return 0;
}
