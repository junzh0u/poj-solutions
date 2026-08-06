// POJ 1502 - MPI Maelstrom
//
// n processors (n <= 100) broadcast a message starting from node 1. A node
// that already has the message can forward it to any number of neighbors at
// once, each arriving after that edge's cost. The minimum time for every
// node to receive the message is exactly the single-source shortest-path
// distance from node 1 (a node's earliest arrival = min over neighbors of
// neighbor's arrival + edge cost), so the answer is the largest shortest-path
// distance from node 1 to any other node. Solved with plain O(n^2) Dijkstra,
// which is comfortably fast for n <= 100.
//
// The input format is the trap: only the strictly lower triangular half of
// the (symmetric) adjacency matrix is given, row i having i-1 entries, and a
// missing edge is written as the literal character 'x' rather than a number.
// That means the matrix cannot be scanned with "%d" alone; each entry is read
// as a token and only converted to an integer when it isn't "x".
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int MAXN = 105;
const int INF = 1000000000;
int dist_[MAXN][MAXN];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            dist_[i][j] = (i == j) ? 0 : INF;

    for (int i = 2; i <= n; ++i) {
        for (int j = 1; j < i; ++j) {
            char tok[32];
            scanf("%s", tok);
            if (tok[0] == 'x' || tok[0] == 'X') {
                // no direct link
            } else {
                int v = atoi(tok);
                dist_[i][j] = v;
                dist_[j][i] = v;
            }
        }
    }

    // Dijkstra from node 1
    bool visited[MAXN];
    memset(visited, 0, sizeof(visited));
    int d[MAXN];
    for (int i = 1; i <= n; ++i) d[i] = dist_[1][i];
    visited[1] = true;
    d[1] = 0;
    for (int iter = 0; iter < n; ++iter) {
        int u = -1, best = INF;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i] && d[i] < best) { best = d[i]; u = i; }
        }
        if (u == -1) break;
        visited[u] = true;
        for (int v = 1; v <= n; ++v) {
            if (!visited[v] && dist_[u][v] < INF && d[u] + dist_[u][v] < d[v]) {
                d[v] = d[u] + dist_[u][v];
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (d[i] > ans) ans = d[i];
    }
    printf("%d\n", ans);
    return 0;
}
