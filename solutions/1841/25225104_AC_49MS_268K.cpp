// POJ 1841 - Meadow
// Model: gpt-6.1-sol
// Repeated flower visits make each bee's set a connected component at the
// chosen distance threshold. Remove the B-1 largest edges of an MST.
// Dense Prim uses squared integer distances in O(F^2) time and O(F) space.
// The input's C means F; read datasets to EOF (no leading case count).
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
int main() {
    int n, b;
    while (scanf("%d%d", &n, &b) == 2) {
        vector<int> x(n), y(n), best(n, 2000000000), edges;
        vector<bool> used(n, false);
        for (int i=0; i<n; ++i) scanf("%d%d", &x[i], &y[i]);
        best[0]=0;
        for (int k=0; k<n; ++k) {
            int v=-1;
            for (int i=0; i<n; ++i)
                if (!used[i] && (v==-1 || best[i]<best[v])) v=i;
            used[v]=true;
            if (k) edges.push_back(best[v]);
            for (int i=0; i<n; ++i) if (!used[i]) {
                int dx=x[v]-x[i], dy=y[v]-y[i];
                int d=dx*dx+dy*dy;
                if (d<best[i]) best[i]=d;
            }
        }
        sort(edges.begin(), edges.end());
        int answer = b==n ? 0 : edges[n-b-1];
        printf("%.2f\n", sqrt((double)answer));
    }
    return 0;
}
