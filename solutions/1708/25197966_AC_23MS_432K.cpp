// POJ 1708 - Game
// Model: claude-sonnet-5
//
// Two pawns start at circles L and K. A pawn at circle u may move along
// an oriented, colored edge u->v if the edge's color equals the color of
// the circle currently occupied by the OTHER pawn, and only if v is not
// where the other pawn sits (two pawns may never share a circle). The
// game ends the instant either pawn reaches circle Q. Find the minimum
// number of moves, or report impossible.
//
// This is BFS over the state space of ordered pairs (posA, posB), of
// which there are at most N*N <= 10000. From each state at most deg(a)+
// deg(b) transitions are generated, so total work is O(N*M) which is
// small for N<=100, M<=10000.
//
// Board note (message 357023): the graph has multi-edges (parallel edges
// with possibly different colors between the same ordered pair), so an
// adjacency matrix loses information -- adjacency lists are required.
// Board note (message 121157 et al.): the "two pawns can never occupy
// the same circle" rule is a common WA if omitted from the transition
// check.
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n, l, k, q;
    if (scanf("%d %d %d %d", &n, &l, &k, &q) != 4) return 0;
    vector<int> color(n + 1);
    for (int i = 1; i <= n; ++i) scanf("%d", &color[i]);
    int m;
    scanf("%d", &m);
    vector<vector<pair<int,int> > > adj(n + 1); // adj[u] = list of (v, edgeColor)
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        adj[a].push_back(make_pair(b, c));
    }

    // dist[a][b]: min moves to reach ordered state (posA=a, posB=b)
    vector<vector<int> > dist(n + 1, vector<int>(n + 1, -1));
    queue<pair<int,int> > qu;
    dist[l][k] = 0;
    qu.push(make_pair(l, k));

    int answer = -1;
    if (l == q || k == q) answer = 0; // not possible per constraints (L,K,Q distinct) but safe

    while (!qu.empty() && answer < 0) {
        pair<int,int> cur = qu.front(); qu.pop();
        int a = cur.first, b = cur.second;
        int d = dist[a][b];

        // Move pawn at a: requires edge color == color[b], destination != b
        int colB = color[b];
        for (size_t i = 0; i < adj[a].size(); ++i) {
            int v = adj[a][i].first;
            int c = adj[a][i].second;
            if (c != colB) continue;
            if (v == b) continue; // can't collide
            if (v == q) { answer = d + 1; break; }
            if (dist[v][b] == -1) {
                dist[v][b] = d + 1;
                qu.push(make_pair(v, b));
            }
        }
        if (answer >= 0) break;

        // Move pawn at b: requires edge color == color[a], destination != a
        int colA = color[a];
        for (size_t i = 0; i < adj[b].size(); ++i) {
            int v = adj[b][i].first;
            int c = adj[b][i].second;
            if (c != colA) continue;
            if (v == a) continue;
            if (v == q) { answer = d + 1; break; }
            if (dist[a][v] == -1) {
                dist[a][v] = d + 1;
                qu.push(make_pair(a, v));
            }
        }
    }

    if (answer >= 0) {
        printf("YES\n%d\n", answer);
    } else {
        printf("NO\n");
    }
    return 0;
}
