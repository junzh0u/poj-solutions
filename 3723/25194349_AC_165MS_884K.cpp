// POJ 3723 - Conscription
//
// Every one of the N girls and M boys costs 10000 RMB to conscript on its
// own. Each girl-boy relationship (x, y, d) lets one of the pair be
// conscripted for 10000-d once the other is already in, but "only one
// relationship can be used when collecting one soldier" -- each person's
// price is discounted by at most one edge. Model girls and boys as the
// N+M vertices of one graph (boy y becomes vertex N+y) and each
// relationship as an edge of weight d. Any set of edges usable as
// discounts must be acyclic (a cycle would need two edges discounting the
// same person), so the maximum total discount is the weight of a maximum
// spanning forest: run Kruskal with edges sorted by descending d and a
// union-find over all N+M vertices, adding an edge's weight to the savings
// whenever it joins two different components. The graph is generally
// disconnected (isolated people, or whole components with no edges at
// all), which the union-find/forest formulation handles automatically --
// those people just stay at full price. Answer = (N+M)*10000 minus the
// total savings. Duplicate edges between the same pair are harmless: only
// the first one processed (by weight, ties arbitrary) can ever union its
// endpoints, so redundant duplicates are simply skipped by the rx != ry
// check.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 20005;
int parent_[MAXN];

int find_(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

struct Edge {
    int x, y, d;
};

bool cmp(const Edge &a, const Edge &b) {
    return a.d > b.d;
}

Edge edges[50005];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N, M, R;
        scanf("%d %d %d", &N, &M, &R);
        for (int i = 0; i < N + M; ++i) parent_[i] = i;
        for (int i = 0; i < R; ++i) {
            int x, y, d;
            scanf("%d %d %d", &x, &y, &d);
            edges[i].x = x;
            edges[i].y = y + N;
            edges[i].d = d;
        }
        sort(edges, edges + R, cmp);
        int total = (N + M) * 10000;
        for (int i = 0; i < R; ++i) {
            int rx = find_(edges[i].x);
            int ry = find_(edges[i].y);
            if (rx != ry) {
                parent_[rx] = ry;
                total -= edges[i].d;
            }
        }
        printf("%d\n", total);
    }
    return 0;
}
