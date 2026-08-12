// POJ 3241 - Object Clustering
// Model: gpt-5.6-terra
// The discussion-board clarification resolves the literal existential wording
// as standard bottleneck clustering. Build a sparse Manhattan MST in four
// orientations, then stop Kruskal when exactly K components remain.
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
using namespace std;

struct Point { int x, y; };
struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const { return w < other.w; }
};

vector<Point>* points_for_sort;
bool by_sum(int a, int b) {
    const vector<Point>& p = *points_for_sort;
    int sa = p[a].x + p[a].y, sb = p[b].x + p[b].y;
    if (sa != sb) return sa < sb;
    return p[a].x < p[b].x;
}

struct DSU {
    vector<int> p, r;
    DSU(int n) : p(n), r(n, 0) { for (int i = 0; i < n; ++i) p[i] = i; }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool join(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<Point> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;

    vector<Edge> edges;
    map< pair<int, int>, int > representative;
    for (int i = 0; i < n; ++i) {
        pair<int, int> key = make_pair(p[i].x, p[i].y);
        map< pair<int, int>, int >::iterator it = representative.find(key);
        if (it == representative.end()) representative[key] = i;
        else {
            Edge e;
            e.u = i; e.v = it->second; e.w = 0;
            edges.push_back(e);
        }
    }
    vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    points_for_sort = &p;
    for (int swapped = 0; swapped < 2; ++swapped) {
        for (int reflected = 0; reflected < 2; ++reflected) {
            sort(order.begin(), order.end(), by_sum);
            map<int, int> active;
            for (int oi = 0; oi < n; ++oi) {
                int i = order[oi];
                map<int, int>::iterator it = active.lower_bound(-p[i].y);
                while (it != active.end()) {
                    int j = it->second;
                    if (p[i].x - p[i].y < p[j].x - p[j].y) break;
                    Edge e;
                    e.u = i; e.v = j;
                    e.w = abs(p[i].x - p[j].x) + abs(p[i].y - p[j].y);
                    edges.push_back(e);
                    active.erase(it++);
                }
                active[-p[i].y] = i;
            }
            for (int i = 0; i < n; ++i) p[i].x = -p[i].x;
        }
        for (int i = 0; i < n; ++i) swap(p[i].x, p[i].y);
    }
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    int components = n;
    for (size_t i = 0; i < edges.size(); ++i) {
        if (dsu.join(edges[i].u, edges[i].v)) {
            --components;
            if (components == k) {
                cout << edges[i].w << '\n';
                return 0;
            }
        }
    }
    return 0;
}
