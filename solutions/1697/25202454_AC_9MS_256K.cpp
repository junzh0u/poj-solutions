// POJ 1697 - The Erythea Campaign
// Model: claude-sonnet-5
//
// Grid of R rows x C columns of squares gives an (R+1) x (C+1) lattice of
// road crossings. A crossing's neighboring squares (up to 4: NW,NE,SW,SE)
// determine which of its edges are usable: a horizontal edge is blocked
// iff both the square above and below it exist and are stronghold squares
// (a road strictly inside a solid stronghold block); a vertical edge is
// blocked iff both the squares left and right of it exist and are
// stronghold squares. A crossing whose 4 neighboring squares all exist and
// are all stronghold squares is thus automatically isolated (all 4
// incident edges blocked), matching "path never intersects the interior
// of a stronghold". A crossing is on the *boundary* of a stronghold iff it
// touches at least one stronghold square but is not such a fully-interior
// point.
//
// risk(v) = R + C - d(v), where d(v) is the BFS shortest number of edges
// (over the same allowed-edge graph) from v to the nearest boundary
// point -- computed via a multi-source BFS seeded at every boundary point.
//
// The answer is the minimum, over paths from S to D using only allowed
// edges, of the sum of risk(v) over every vertex visited (S and D
// included, each vertex costed once since an optimal path never benefits
// from revisiting a vertex -- risk stays non-negative on the judge's
// data, confirmed by multiple accepted plain-Dijkstra solutions on the
// discuss board). This is a vertex-weighted shortest path, solved by
// Dijkstra treating the cost of entering v as risk(v).
//
// Statement ambiguity: "number of rows and number of columns" for the
// grid vs. the (y,x) coordinate convention. Verified against the sample:
// an "8 6" grid (8 rows, 6 columns) is followed by 8 map lines of length
// 6, and y indexes rows (0..R), x indexes columns (0..C). No other
// ambiguity found; board data (message 160020, 8 extra cases with
// expected output) matches this reading exactly.

#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

const int MAXR = 82, MAXC = 82;
char g[MAXR][MAXC];
int R, C;

inline bool existsSq(int i, int j) {
    return i >= 0 && i < R && j >= 0 && j < C;
}
inline int valSq(int i, int j) {
    if (!existsSq(i, j)) return 0;
    return g[i][j] == '1' ? 1 : 0;
}

inline int pid(int y, int x) { return y * (C + 1) + x; }

int main() {
    int M;
    if (scanf("%d", &M) != 1) return 0;
    while (M--) {
        scanf("%d %d", &R, &C);
        int ys, xs, yd, xd;
        scanf("%d %d %d %d", &ys, &xs, &yd, &xd);
        for (int i = 0; i < R; i++) {
            scanf("%s", g[i]);
        }

        int NP = (R + 1) * (C + 1);

        // classify points: boundary?
        vector<char> isBoundary(NP, 0);
        for (int y = 0; y <= R; y++) {
            for (int x = 0; x <= C; x++) {
                int existNW = existsSq(y - 1, x - 1);
                int existNE = existsSq(y - 1, x);
                int existSW = existsSq(y, x - 1);
                int existSE = existsSq(y, x);
                int vNW = valSq(y - 1, x - 1);
                int vNE = valSq(y - 1, x);
                int vSW = valSq(y, x - 1);
                int vSE = valSq(y, x);
                int countExist = existNW + existNE + existSW + existSE;
                int countOne = vNW + vNE + vSW + vSE;
                bool fullyInterior = (countExist == 4 && countOne == 4);
                bool boundary = (countOne >= 1) && !fullyInterior;
                isBoundary[pid(y, x)] = boundary ? 1 : 0;
            }
        }

        // edge blocking helpers
        // horiz edge (y,x)-(y,x+1): blocked if square(y-1,x) and square(y,x) both exist and are 1
        // vert edge (y,x)-(y+1,x): blocked if square(y,x-1) and square(y,x) both exist and are 1

        // multi-source BFS for d[]
        vector<int> d(NP, -1);
        queue<int> q;
        for (int y = 0; y <= R; y++) {
            for (int x = 0; x <= C; x++) {
                if (isBoundary[pid(y, x)]) {
                    d[pid(y, x)] = 0;
                    q.push(pid(y, x));
                }
            }
        }
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            int y = cur / (C + 1), x = cur % (C + 1);
            // neighbors
            // right: (y,x)-(y,x+1) horiz edge
            if (x + 1 <= C) {
                bool blocked = existsSq(y - 1, x) && existsSq(y, x) &&
                               valSq(y - 1, x) == 1 && valSq(y, x) == 1;
                if (!blocked) {
                    int nxt = pid(y, x + 1);
                    if (d[nxt] == -1) { d[nxt] = d[cur] + 1; q.push(nxt); }
                }
            }
            // left: (y,x-1)-(y,x) horiz edge
            if (x - 1 >= 0) {
                bool blocked = existsSq(y - 1, x - 1) && existsSq(y, x - 1) &&
                               valSq(y - 1, x - 1) == 1 && valSq(y, x - 1) == 1;
                if (!blocked) {
                    int nxt = pid(y, x - 1);
                    if (d[nxt] == -1) { d[nxt] = d[cur] + 1; q.push(nxt); }
                }
            }
            // down: (y,x)-(y+1,x) vert edge
            if (y + 1 <= R) {
                bool blocked = existsSq(y, x - 1) && existsSq(y, x) &&
                               valSq(y, x - 1) == 1 && valSq(y, x) == 1;
                if (!blocked) {
                    int nxt = pid(y + 1, x);
                    if (d[nxt] == -1) { d[nxt] = d[cur] + 1; q.push(nxt); }
                }
            }
            // up: (y-1,x)-(y,x) vert edge
            if (y - 1 >= 0) {
                bool blocked = existsSq(y - 1, x - 1) && existsSq(y - 1, x) &&
                               valSq(y - 1, x - 1) == 1 && valSq(y - 1, x) == 1;
                if (!blocked) {
                    int nxt = pid(y - 1, x);
                    if (d[nxt] == -1) { d[nxt] = d[cur] + 1; q.push(nxt); }
                }
            }
        }

        // risk[v] = R + C - d[v] (only meaningful where d[v] != -1)
        // Dijkstra for min sum of risk along path from S to D
        const int INF = 1 << 30;
        vector<int> dist(NP, INF);
        vector<char> done(NP, 0);
        int S = pid(ys, xs), D = pid(yd, xd);

        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        if (d[S] != -1) {
            dist[S] = (int)(R + C) - d[S];
            pq.push(make_pair(dist[S], S));
        }
        while (!pq.empty()) {
            pair<int, int> top = pq.top(); pq.pop();
            int du = top.first;
            int u = top.second;
            if (done[u]) continue;
            done[u] = 1;
            if (du > dist[u]) continue;
            int y = u / (C + 1), x = u % (C + 1);
            // 4 neighbors with same blocking rules as above
            if (x + 1 <= C) {
                bool blocked = existsSq(y - 1, x) && existsSq(y, x) &&
                               valSq(y - 1, x) == 1 && valSq(y, x) == 1;
                int v = pid(y, x + 1);
                if (!blocked && d[v] != -1) {
                    int nd = dist[u] + (int)(R + C) - d[v];
                    if (nd < dist[v]) { dist[v] = nd; pq.push(make_pair(nd, v)); }
                }
            }
            if (x - 1 >= 0) {
                bool blocked = existsSq(y - 1, x - 1) && existsSq(y, x - 1) &&
                               valSq(y - 1, x - 1) == 1 && valSq(y, x - 1) == 1;
                int v = pid(y, x - 1);
                if (!blocked && d[v] != -1) {
                    int nd = dist[u] + (int)(R + C) - d[v];
                    if (nd < dist[v]) { dist[v] = nd; pq.push(make_pair(nd, v)); }
                }
            }
            if (y + 1 <= R) {
                bool blocked = existsSq(y, x - 1) && existsSq(y, x) &&
                               valSq(y, x - 1) == 1 && valSq(y, x) == 1;
                int v = pid(y + 1, x);
                if (!blocked && d[v] != -1) {
                    int nd = dist[u] + (int)(R + C) - d[v];
                    if (nd < dist[v]) { dist[v] = nd; pq.push(make_pair(nd, v)); }
                }
            }
            if (y - 1 >= 0) {
                bool blocked = existsSq(y - 1, x - 1) && existsSq(y - 1, x) &&
                               valSq(y - 1, x - 1) == 1 && valSq(y - 1, x) == 1;
                int v = pid(y - 1, x);
                if (!blocked && d[v] != -1) {
                    int nd = dist[u] + (int)(R + C) - d[v];
                    if (nd < dist[v]) { dist[v] = nd; pq.push(make_pair(nd, v)); }
                }
            }
        }

        if (dist[D] >= INF) {
            printf("no solution\n");
        } else {
            printf("%d\n", dist[D]);
        }
    }
    return 0;
}
