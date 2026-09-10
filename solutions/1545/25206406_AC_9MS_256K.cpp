// POJ 1545 - Galactic Import
// Model: claude-sonnet-5
// Each intermediate planet on a shipping route keeps 5% of the shipment,
// so a planet's export arrives at Earth worth value * 0.95^k, where k is
// the number of intermediate planets on the route used. Since the 5% fee
// is a fixed per-hop multiplier independent of which planets are on the
// route, the value-maximizing route to any planet is simply the one with
// the fewest hops from a ThrustoZoom ('*') planet. So this reduces to a
// multi-source BFS (unweighted shortest path) from all '*' planets over
// the shipping-line graph: for planet P let d(P) be its BFS distance to
// the nearest '*' planet (0 if P itself has '*'); the delivered value is
// value(P) * 0.95^d(P). Output the planet maximizing this, ties broken
// alphabetically (checking letters A..Z in order with a strict '>'
// comparison naturally keeps the first-seen, i.e. alphabetically first,
// maximum).
// The adjacency matrix is filled symmetrically from whichever end(s) list
// the connection, so it tolerates input listing an edge from either or
// both endpoints (the samples always list both, but nothing in the
// statement promises that in general).
// Ambiguity: none found in the statement; POJ discuss board confirms this
// is a shortest-path problem ("最短路").
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        double value[26];
        bool present[26];
        bool isStar[26];
        static bool conn[26][26];
        memset(present, 0, sizeof(present));
        memset(isStar, 0, sizeof(isStar));
        memset(conn, 0, sizeof(conn));
        for (int i = 0; i < n; ++i) {
            char letterBuf[8], connBuf[64];
            double v;
            scanf("%s %lf %s", letterBuf, &v, connBuf);
            int p = letterBuf[0] - 'A';
            present[p] = true;
            value[p] = v;
            int len = (int)strlen(connBuf);
            for (int j = 0; j < len; ++j) {
                char c = connBuf[j];
                if (c == '*') {
                    isStar[p] = true;
                } else {
                    int q = c - 'A';
                    conn[p][q] = true;
                    conn[q][p] = true;
                }
            }
        }
        int dist[26];
        for (int i = 0; i < 26; ++i) dist[i] = -1;
        queue<int> bfsq;
        for (int i = 0; i < 26; ++i) {
            if (present[i] && isStar[i]) {
                dist[i] = 0;
                bfsq.push(i);
            }
        }
        while (!bfsq.empty()) {
            int u = bfsq.front(); bfsq.pop();
            for (int v = 0; v < 26; ++v) {
                if (present[v] && conn[u][v] && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    bfsq.push(v);
                }
            }
        }
        int best = -1;
        double bestVal = -1.0;
        for (int i = 0; i < 26; ++i) {
            if (!present[i] || dist[i] == -1) continue;
            double received = value[i];
            for (int k = 0; k < dist[i]; ++k) received *= 0.95;
            if (received > bestVal + 1e-9) {
                bestVal = received;
                best = i;
            }
        }
        printf("Import from %c\n", 'A' + best);
    }
    return 0;
}
