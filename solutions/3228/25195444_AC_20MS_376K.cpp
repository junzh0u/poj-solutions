// POJ 3228 - Gold Transportation
// Model: claude-sonnet-5
//
// Roads carry unlimited flow; only a node's own mine (source) capacity and
// storehouse (sink) capacity are bounded. We want the minimum threshold D
// such that using only roads with distance <= D, every gold mine's stock
// can be fully delivered.
//
// Key reduction: fix a threshold D and look at the connected components of
// the graph restricted to edges with weight <= D. Within one connected
// component, every internal edge has unlimited capacity, so the max flow
// from the component's mines to its storehouses equals
// min(total mine stock in component, total storehouse capacity in
// component) -- any interior cut would have to sever an infinite-capacity
// edge, so the only finite cuts are "right after the sources" or "right
// before the sinks". Hence full feasibility at threshold D is exactly:
// every component has (mine sum) <= (storehouse sum).
//
// This condition is monotone in D (merging two feasible components keeps
// their sums feasible), so we can run Kruskal in increasing edge-weight
// order with a DSU that carries each component's mine/storehouse sums and a
// counter of "infeasible" components (mine sum > storehouse sum). The
// answer is the weight of the edge whose union first drops that counter to
// zero. If it never reaches zero, output "No Solution". (If it is already
// zero with no edges at all, the answer is 0 -- no transportation needed.)
//
// Ambiguity check: the statement says nothing about total supply equalling
// total demand; the algorithm only requires supply <= demand per component,
// leftover storehouse capacity is fine. Confirmed against the sample
// (answer 6, reached exactly when the {1,2} mine-heavy component merges
// with the {3,4} storehouse-heavy component) and cross-checked with a
// binary-search + max-flow reference on random small graphs.

#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

struct Edge {
    int x, y, d;
};

bool cmpEdge(const Edge &a, const Edge &b) { return a.d < b.d; }

int parent[205];
long long supply[205], demand[205];

int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        long long mine[205], store[205];
        for (int i = 1; i <= n; ++i) {
            scanf("%lld", &mine[i]);
        }
        for (int i = 1; i <= n; ++i) {
            scanf("%lld", &store[i]);
        }
        int m;
        scanf("%d", &m);
        vector<Edge> edges(m);
        for (int i = 0; i < m; ++i) {
            scanf("%d %d %d", &edges[i].x, &edges[i].y, &edges[i].d);
        }
        sort(edges.begin(), edges.end(), cmpEdge);

        int badCount = 0;
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            supply[i] = mine[i];
            demand[i] = store[i];
            if (supply[i] > demand[i]) ++badCount;
        }

        int answer = -1;
        if (badCount == 0) {
            answer = 0;
        } else {
            for (int i = 0; i < m; ++i) {
                int rx = findRoot(edges[i].x);
                int ry = findRoot(edges[i].y);
                if (rx == ry) continue;
                int oldBadX = (supply[rx] > demand[rx]) ? 1 : 0;
                int oldBadY = (supply[ry] > demand[ry]) ? 1 : 0;
                long long ns = supply[rx] + supply[ry];
                long long nd = demand[rx] + demand[ry];
                parent[rx] = ry;
                supply[ry] = ns;
                demand[ry] = nd;
                int newBad = (ns > nd) ? 1 : 0;
                badCount += newBad - oldBadX - oldBadY;
                if (badCount == 0) {
                    answer = edges[i].d;
                    break;
                }
            }
        }

        if (answer >= 0) {
            printf("%d\n", answer);
        } else {
            printf("No Solution\n");
        }
    }
    return 0;
}
