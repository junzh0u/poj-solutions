// POJ 2395 - Out of Hay
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 2005;
const int MAXM = 10005;

int par[MAXN];

int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

struct Edge {
    int a, b, l; // l <= 1,000,000,000 fits comfortably in a 32-bit int
};

bool cmp(const Edge &x, const Edge &y) {
    return x.l < y.l;
}

Edge edges[MAXM];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &edges[i].a, &edges[i].b, &edges[i].l);
    }
    sort(edges, edges + m, cmp);
    for (int i = 1; i <= n; ++i) par[i] = i;

    int ans = 0;
    int comps = n;
    for (int i = 0; i < m && comps > 1; ++i) {
        int ra = find(edges[i].a);
        int rb = find(edges[i].b);
        if (ra != rb) {
            par[ra] = rb;
            comps--;
            if (edges[i].l > ans) ans = edges[i].l;
        }
    }

    printf("%d\n", ans);
    return 0;
}
