// POJ 2349 - Arctic Network
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int par[505];
int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }

struct Edge {
    double w;
    int u, v;
    bool operator<(const Edge &o) const { return w < o.w; }
};

int main() {
    int N;
    scanf("%d", &N);
    while (N--) {
        int S, P;
        scanf("%d %d", &S, &P);
        static int x[505], y[505];
        for (int i = 0; i < P; i++) scanf("%d %d", &x[i], &y[i]);

        Edge edges[500 * 499 / 2];
        int ne = 0;
        for (int i = 0; i < P; i++) {
            for (int j = i + 1; j < P; j++) {
                double dx = (double)(x[i] - x[j]);
                double dy = (double)(y[i] - y[j]);
                edges[ne].w = sqrt(dx * dx + dy * dy);
                edges[ne].u = i;
                edges[ne].v = j;
                ne++;
            }
        }
        sort(edges, edges + ne);

        for (int i = 0; i < P; i++) par[i] = i;
        double mstW[505];
        int nm = 0;
        for (int i = 0; i < ne && nm < P - 1; i++) {
            int ru = find(edges[i].u), rv = find(edges[i].v);
            if (ru != rv) {
                par[ru] = rv;
                mstW[nm++] = edges[i].w;
            }
        }

        sort(mstW, mstW + nm, greater<double>());

        int removeCount = S - 1;
        double ans = 0.0;
        if (removeCount < nm) ans = mstW[removeCount];
        printf("%.2f\n", ans);
    }
    return 0;
}
