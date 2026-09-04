// POJ 1529 - Shipping Routes
// Model: claude-sonnet-5
// Approach: For each data set, build an unweighted graph over M (<=30)
// warehouses and run Floyd-Warshall to get the fewest-legs distance between
// every pair. For each shipping request, cost = size * legs * 100, or
// "NO SHIPMENT POSSIBLE" if the destination is unreachable.
//
// Statement ambiguity: the exact blank-line placement in the output is not
// fully pinned down by prose; the board (message 12552) records that the
// judge's own expected output was changed after report so that a data set
// with zero shipping requests still contributes two blank lines (one after
// the "DATA SET n" header, one that would otherwise separate the request
// lines from the next section). Printing a blank line unconditionally right
// after each "DATA SET n" header, and another unconditionally after the
// block of P request lines (even when P=0), reproduces the sample exactly
// line-for-line and matches that report.

#include <cstdio>
#include <cstring>
#include <string>
#include <map>
using namespace std;

static const int INF = 1000000;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    printf("SHIPPING ROUTES OUTPUT\n\n");
    for (int tc = 1; tc <= T; tc++) {
        int M, N, P;
        scanf("%d %d %d", &M, &N, &P);
        map<string, int> id;
        char buf[8];
        for (int i = 0; i < M; i++) {
            scanf("%s", buf);
            id[string(buf)] = i;
        }
        static int dist[30][30];
        for (int i = 0; i < M; i++)
            for (int j = 0; j < M; j++)
                dist[i][j] = (i == j) ? 0 : INF;
        for (int i = 0; i < N; i++) {
            char a[8], b[8];
            scanf("%s %s", a, b);
            int u = id[string(a)], v = id[string(b)];
            dist[u][v] = 1;
            dist[v][u] = 1;
        }
        for (int k = 0; k < M; k++)
            for (int i = 0; i < M; i++)
                for (int j = 0; j < M; j++)
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

        printf("DATA SET %d\n\n", tc);
        for (int q = 0; q < P; q++) {
            int size;
            char a[8], b[8];
            scanf("%d %s %s", &size, a, b);
            int u = id[string(a)], v = id[string(b)];
            if (dist[u][v] >= INF) {
                printf("NO SHIPMENT POSSIBLE\n");
            } else {
                long cost = (long)size * dist[u][v] * 100;
                printf("$%ld\n", cost);
            }
        }
        printf("\n");
    }
    printf("END OF OUTPUT\n");
    return 0;
}
