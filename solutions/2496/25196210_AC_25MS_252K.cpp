// POJ 2496 - Military Recruit
// Model: claude-sonnet-5
// Approach: All-pairs shortest paths via Floyd-Warshall on the directed
// weighted graph (treating "-1" entries as no edge, i.e. infinity, never
// as an actual edge weight of -1 -- the board flags this exact mistake as
// a common WA). Collect the n*(n-1) ordered-pair shortest distances
// (i != j; every site is reachable from every other, so all are finite),
// sort them, and take the k-th smallest where k = ceil(p * n*(n-1) / 100)
// computed with integer arithmetic (p*total + 99) / 100 to avoid float
// rounding. This matches both samples and the board's own stated formula
// ("output the n*(n-1)*p/100 + 1'th entry when it doesn't divide evenly").
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 105;
const int INF = 1000000000;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; tc++) {
        int p, n;
        scanf("%d %d", &p, &n);
        static int d[MAXN][MAXN];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int v;
                scanf("%d", &v);
                if (i == j) d[i][j] = 0;
                else if (v < 0) d[i][j] = INF;
                else d[i][j] = v;
            }
        }
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (d[i][k] == INF) continue;
                for (int j = 0; j < n; j++) {
                    if (d[k][j] == INF) continue;
                    if (d[i][k] + d[k][j] < d[i][j]) {
                        d[i][j] = d[i][k] + d[k][j];
                    }
                }
            }
        }
        static int vals[MAXN * MAXN];
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                vals[cnt++] = d[i][j];
            }
        }
        sort(vals, vals + cnt);
        int k = (p * cnt + 99) / 100;
        if (k < 1) k = 1;
        if (k > cnt) k = cnt;
        int ans = vals[k - 1];
        printf("Scenario #%d:\n%d\n\n", tc, ans);
    }
    return 0;
}
