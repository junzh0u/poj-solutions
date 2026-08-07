// POJ 1603 - Risk
// Model: claude-opus-5
//
// Approach: the board is an undirected graph on exactly 20 vertices, given as
// 19 adjacency lines (line I lists only the neighbours J > I, so each edge is
// stated once).  "Minimum number of countries to conquer, including the
// destination" is just the number of edges on a shortest path, so the answer is
// an unweighted BFS distance.  With 20 vertices a plain BFS per query is ample
// (N <= 100 queries per set); Floyd-Warshall would do as well.
//
// Input framing (the part the statement states but is easy to miss, and the
// discuss board repeats): the file holds MANY test sets back to back and must
// be read to EOF.  Everything is whitespace-separated, so `cin >>` handles the
// line structure; EOF is detected on the very first token of a set.
//
// Output framing (the board's other recurring complaint - it is a PE magnet):
// "Test Set #T", then one "A to B: d" line per query, then a single blank line
// after EVERY set, including the last.  No padding, no leading spaces.

#include <cstdio>
#include <iostream>

using namespace std;

int main() {
    bool adj[21][21];
    int dist[21];
    int que[21];
    int setNo = 0;
    int x;

    while (cin >> x) {
        ++setNo;
        for (int i = 1; i <= 20; ++i)
            for (int j = 1; j <= 20; ++j)
                adj[i][j] = false;

        // x is already the neighbour count of country 1.
        for (int i = 1; i <= 19; ++i) {
            int cnt;
            if (i == 1) cnt = x;
            else cin >> cnt;
            for (int k = 0; k < cnt; ++k) {
                int j;
                cin >> j;
                adj[i][j] = true;
                adj[j][i] = true;
            }
        }

        int n;
        cin >> n;
        printf("Test Set #%d\n", setNo);
        for (int q = 0; q < n; ++q) {
            int a, b;
            cin >> a >> b;

            for (int i = 1; i <= 20; ++i) dist[i] = -1;
            int head = 0, tail = 0;
            dist[a] = 0;
            que[tail++] = a;
            while (head < tail && dist[b] < 0) {
                int u = que[head++];
                for (int v = 1; v <= 20; ++v) {
                    if (adj[u][v] && dist[v] < 0) {
                        dist[v] = dist[u] + 1;
                        que[tail++] = v;
                    }
                }
            }
            printf("%d to %d: %d\n", a, b, dist[b]);
        }
        printf("\n");
    }
    return 0;
}
