// POJ 1340 - The Longest Detour Problem
// Model: claude-sonnet-5
//
// The given path r=path[0..k-1]=s is a shortest r-s path in the (undirected)
// weighted graph G. Because a subpath of a shortest path is itself a shortest
// path (nonnegative weights), d_G(u,s) for any u on the path is just the sum
// of edge weights of the path from u to s (a suffix sum). For every edge
// e=(path[i],path[i+1]) on the path we temporarily delete e (both directions,
// since the adjacency matrix is symmetric) and run an O(n^2) Dijkstra from
// path[i] to s in G-e; the detour increment for that edge is that new
// distance minus the suffix-sum distance. The answer is the maximum
// increment over all edges of the path.
//
// Input parsing: the discuss board (multiple threads, e.g. message ids
// 351920, 179860, 164665, 41805) documents that the judge data around the
// trailing "path" line is deliberately hostile: arbitrary blank lines (even
// many) before it, and stray trailing spaces on it, while the number of
// vertices on that line is not given anywhere and must be inferred from the
// line itself. cin>> alone can't find "the line with the path" (it would
// happily read numbers belonging to a later blank/garbage line), so after
// reading the n x n matrix with plain whitespace-skipping cin>>, the rest of
// the current physical line is discarded and then getline() is used
// repeatedly, skipping over lines that parse to zero integers, until a
// non-empty line of integers is found; that line's ints are the path.
//
// Weights are small nonnegative integers (sample data <= a few hundred) so
// plain int (32-bit, compatible with POJ's old MSVC "language 4" which has
// no long long) is more than enough for path-length sums up to ~100 edges.
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static const int MAXN = 105;
static const int INF = 1000000000;

int n;
int w[MAXN][MAXN];

int dijkstra(int src, int dst) {
    vector<int> dist(n + 1, INF);
    vector<bool> visited(n + 1, false);
    dist[src] = 0;
    for (int iter = 0; iter < n; iter++) {
        int u = -1, best = INF + 1;
        for (int i = 1; i <= n; i++) {
            if (!visited[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = true;
        if (u == dst) break;
        for (int v = 1; v <= n; v++) {
            if (w[u][v] > 0 && dist[u] + w[u][v] < dist[v]) {
                dist[v] = dist[u] + w[u][v];
            }
        }
    }
    return dist[dst];
}

int main() {
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                cin >> w[i][j];

        string line;
        getline(cin, line); // discard remainder of the matrix's last line

        vector<int> path;
        while (path.empty()) {
            if (!getline(cin, line)) break;
            istringstream iss(line);
            int x;
            while (iss >> x) path.push_back(x);
        }

        int k = (int)path.size();
        vector<int> suf(k, 0);
        for (int i = k - 2; i >= 0; i--)
            suf[i] = suf[i + 1] + w[path[i]][path[i + 1]];

        int best = 0;
        for (int i = 0; i < k - 1; i++) {
            int u = path[i], v = path[i + 1];
            int ow1 = w[u][v], ow2 = w[v][u];
            w[u][v] = 0;
            w[v][u] = 0;
            int newDist = dijkstra(u, path[k - 1]);
            w[u][v] = ow1;
            w[v][u] = ow2;
            int inc = newDist - suf[i];
            if (inc > best) best = inc;
        }
        cout << best << "\n";
    }
    return 0;
}
