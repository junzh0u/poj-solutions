// POJ 1688 - Dolphin Pool
// Model: claude-sonnet-5
//
// Answer = (# boundary loops of the union of disks) - (# connected
// components of the union). Each component with h holes has h+1 boundary
// loops (1 outer + h inner), so summing gives total holes = L - C.
//
// Per circle, split it by its crossing points with other circles (proper
// 2-point crossings) into elementary arcs. Containment status vs any other
// circle is constant along an elementary arc (it can only change at a
// crossing, which is a division point), so testing an arc's midpoint tells
// us if the whole arc is on the union's boundary. Boundary arcs form a
// graph where every vertex has degree exactly 2, so it decomposes into
// simple cycles = loops (counted via BFS components). A circle with no
// crossings is a single whole-circle arc: contributes its own loop unless
// swallowed by containment in another circle.
//
// Board (msg 5390/5392/5393): despite the statement's promise that no
// ring's center lies inside another, real judge data has genuine
// containment (no crossing at all) between circles. Handled directly by
// the disk-overlap test (connectivity, d^2 < (ri+rj)^2) and the
// point-in-circle test (which arcs/whole circles survive on the boundary)
// without special-casing. Tangency is trusted absent per the statement, so
// all d^2 vs (ri+/-rj)^2 comparisons use exact integer arithmetic.

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

static int par[25];
static int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
static void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a != b) par[a] = b;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        vector<long> X(n), Y(n), R(n);
        for (int i = 0; i < n; i++) {
            long xi, yi, ri;
            scanf("%ld %ld %ld", &xi, &yi, &ri);
            X[i] = xi; Y[i] = yi; R[i] = ri;
        }
        for (int i = 0; i < n; i++) par[i] = i;

        // DSU by disk overlap (crossing OR containment): d^2 < (ri+rj)^2
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) {
                long dx = X[i] - X[j], dy = Y[i] - Y[j];
                long d2 = dx * dx + dy * dy;
                long sumR = R[i] + R[j];
                if (d2 < sumR * sumR) unite(i, j);
            }

        // Per-circle crossing points (only proper 2-point crossings).
        vector<vector<pair<double, int> > > pts(n);
        vector<double> vx, vy; // vertex id -> coordinates

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) {
                long dx = X[i] - X[j], dy = Y[i] - Y[j];
                long d2 = dx * dx + dy * dy;
                long sumR = R[i] + R[j];
                long diffR = R[i] - R[j];
                if (d2 > diffR * diffR && d2 < sumR * sumR) {
                    double dxi = (double)(X[j] - X[i]);
                    double dyi = (double)(Y[j] - Y[i]);
                    double d = sqrt((double)d2);
                    double a = ((double)d2 + (double)(R[i] * R[i]) - (double)(R[j] * R[j])) / (2.0 * d);
                    double h2 = (double)(R[i] * R[i]) - a * a;
                    if (h2 < 0) h2 = 0;
                    double h = sqrt(h2);
                    double mx = X[i] + a * dxi / d;
                    double my = Y[i] + a * dyi / d;
                    double ux = -dyi / d, uy = dxi / d;
                    double p1x = mx + h * ux, p1y = my + h * uy;
                    double p2x = mx - h * ux, p2y = my - h * uy;
                    int id1 = (int)vx.size(); vx.push_back(p1x); vy.push_back(p1y);
                    int id2 = (int)vx.size(); vx.push_back(p2x); vy.push_back(p2y);
                    pts[i].push_back(make_pair(atan2(p1y - Y[i], p1x - X[i]), id1));
                    pts[i].push_back(make_pair(atan2(p2y - Y[i], p2x - X[i]), id2));
                    pts[j].push_back(make_pair(atan2(p1y - Y[j], p1x - X[j]), id1));
                    pts[j].push_back(make_pair(atan2(p2y - Y[j], p2x - X[j]), id2));
                }
            }

        int V = (int)vx.size();
        vector<vector<int> > adj(V);
        int isolatedLoops = 0;
        const double PI = acos(-1.0);
        const double EPS = 1e-6;

        for (int i = 0; i < n; i++) {
            if (pts[i].empty()) {
                // whole circle is a single arc; test one representative point
                double px = X[i] + (double)R[i], py = (double)Y[i];
                bool inside = false;
                for (int l = 0; l < n; l++) {
                    if (l == i) continue;
                    double ddx = px - X[l], ddy = py - Y[l];
                    double dist2 = ddx * ddx + ddy * ddy;
                    if (dist2 < (double)(R[l] * R[l]) - EPS) { inside = true; break; }
                }
                if (!inside) isolatedLoops++;
                continue;
            }
            vector<pair<double, int> > &lst = pts[i];
            sort(lst.begin(), lst.end());
            int m = (int)lst.size();
            for (int k = 0; k < m; k++) {
                int kk = (k + 1) % m;
                double angA = lst[k].first, angB = lst[kk].first;
                double mid;
                if (kk != 0) mid = (angA + angB) / 2.0;
                else mid = (angA + (angB + 2 * PI)) / 2.0;
                double px = X[i] + R[i] * cos(mid);
                double py = Y[i] + R[i] * sin(mid);
                bool inside = false;
                for (int l = 0; l < n; l++) {
                    if (l == i) continue;
                    double ddx = px - X[l], ddy = py - Y[l];
                    double dist2 = ddx * ddx + ddy * ddy;
                    if (dist2 < (double)(R[l] * R[l]) - EPS) { inside = true; break; }
                }
                if (!inside) {
                    int idA = lst[k].second, idB = lst[kk].second;
                    adj[idA].push_back(idB);
                    adj[idB].push_back(idA);
                }
            }
        }

        // count connected components among vertices with edges (each is a simple cycle)
        vector<int> visited(V, 0);
        int boundaryLoops = 0;
        for (int s = 0; s < V; s++) {
            if (visited[s] || adj[s].empty()) continue;
            boundaryLoops++;
            queue<int> q;
            q.push(s); visited[s] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (size_t t = 0; t < adj[u].size(); t++) {
                    int v = adj[u][t];
                    if (!visited[v]) { visited[v] = 1; q.push(v); }
                }
            }
        }

        int totalLoops = boundaryLoops + isolatedLoops;
        int totalComponents = 0;
        for (int i = 0; i < n; i++) if (find(i) == i) totalComponents++;

        printf("%d\n", totalLoops - totalComponents);
    }
    return 0;
}
