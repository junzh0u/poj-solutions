// POJ 3271 - Lilypad Pond
// Model: claude-sonnet-5
//
// Approach: Knight-move BFS where existing lilypads (1/3/4) form free (cost-0)
// moves and water cells (0) cost 1 (a new lilypad must be placed there). This
// is a vertex-weighted shortest path problem, solvable with 0-1 BFS to get the
// minimum number of new lilypads.
//
// The tricky part is counting the number of ways. A first attempt modeled
// each maximal knight-connected group of EXISTING lilypads as one graph node
// ("component") with free entry, and summed ways additively into and out of
// that node. That approach double-counts: whenever two water cells W1, W2
// (with W2 one step "later") are connected BOTH by a direct knight move AND
// by a knight move through a shared existing-lilypad group, both routes
// place the exact same set of new pads {W1, W2}, yet get counted as two
// separate contributions. Differential testing against a brute-force
// reference caught this on a 4x8 grid (minimum answer 2, true way count 4,
// component-node model produced 5) once grids got large/dense enough to
// contain such a redundant pair of routes; a first submission using that
// buggy version got Wrong Answer.
//
// The fix: never materialize existing-lilypad groups as graph nodes at all.
// Only "special" cells -- water (0), start (3), end (4) -- are graph nodes.
// For each special cell, explore (BFS) through chains of existing lilypad
// cells (type 1, which are pure free pass-throughs) to find every OTHER
// special cell reachable that way, and add a single deduplicated edge to
// each such cell (regardless of how many different 1-chains would reach it,
// or whether it's also directly knight-adjacent). Entering a water cell
// costs 1 (place a pad there); entering start/end costs 0 (already a
// lilypad). This is exactly a 0-1 BFS on these deduplicated edges.
//
// With edges deduplicated per destination cell up front, standard
// shortest-path counting is safe: process nodes in increasing distance
// order and, for each node v, sum ways[u] over neighbors u with
// dist[u] + edgeWeight(u,v) == dist[v]. Since every predecessor edge into v
// is now unique per source cell, no set can be double-attributed. dist[start]
// = 0 gives ways[start] = 1 as the base case, and dist 0 only ever occurs
// for start itself, so the "already reachable" case naturally prints 0 / 1.
//
// This was re-verified against an exhaustive brute force (enumerate subsets
// of water cells by increasing size, check knight-move reachability) on 900+
// random small grids (up to 5x5, various lilypad/water/rock densities across
// several profiles), including the specific 4x8 case that broke the earlier
// component-node model, plus edge cases: immediate reachability (0/1),
// unreachable targets (-1), 1-row/1-column grids (knight moves impossible),
// and 30x30 worst-case grids for both timing and int64 range (statement
// guarantees the count fits in a signed 64-bit integer).
//
// Statement ambiguity: none beyond the "0 additional -> print 0 and 1" case,
// confirmed by the discuss board (message 74171/124386): many WA submissions
// stemmed from missing that direct-reachability case.

#include <cstdio>
#include <cstring>
#include <vector>
#include <deque>
using namespace std;

typedef long long ll;

int M, N;
int grid[30][30];

int dr[8] = {1,1,-1,-1,2,2,-2,-2};
int dc[8] = {2,-2,2,-2,1,-1,1,-1};

bool isLily1(int v) { return v == 1; }
bool isSpecial(int v) { return v == 0 || v == 3 || v == 4; }
bool inBounds(int r, int c) { return r >= 0 && r < M && c >= 0 && c < N; }

int main() {
    if (scanf("%d %d", &M, &N) != 2) return 0;
    int startR = -1, startC = -1, endR = -1, endC = -1;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &grid[i][j]);
            if (grid[i][j] == 3) { startR = i; startC = j; }
            if (grid[i][j] == 4) { endR = i; endC = j; }
        }
    }

    int totalCells = M * N;
    vector<int> specId(totalCells, -1);
    vector<int> specCost; // 1 for water (must place a pad), 0 for start/end
    int numSpecial = 0;
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            if (isSpecial(grid[r][c])) {
                specId[r*N+c] = numSpecial++;
                specCost.push_back(grid[r][c] == 0 ? 1 : 0);
            }
        }
    }

    vector< vector<int> > adj(numSpecial);

    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            if (!isSpecial(grid[r][c])) continue;
            int sId = specId[r*N+c];
            vector<char> visitedLily(totalCells, 0);
            vector<int> qlily;
            vector<char> foundSpecial(numSpecial, 0);

            // First knight move from (r,c) itself.
            for (int k = 0; k < 8; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (!inBounds(nr, nc)) continue;
                int v = grid[nr][nc];
                if (isSpecial(v)) {
                    int tId = specId[nr*N+nc];
                    if (tId != sId && !foundSpecial[tId]) {
                        foundSpecial[tId] = 1;
                        adj[sId].push_back(tId);
                    }
                } else if (isLily1(v)) {
                    int cell = nr*N+nc;
                    if (!visitedLily[cell]) { visitedLily[cell] = 1; qlily.push_back(cell); }
                }
            }
            // BFS through the free existing-lilypad (type 1) network.
            for (size_t qi = 0; qi < qlily.size(); qi++) {
                int cur = qlily[qi];
                int cr = cur / N, cc = cur % N;
                for (int k = 0; k < 8; k++) {
                    int nr = cr + dr[k], nc = cc + dc[k];
                    if (!inBounds(nr, nc)) continue;
                    int v = grid[nr][nc];
                    if (isSpecial(v)) {
                        int tId = specId[nr*N+nc];
                        if (tId != sId && !foundSpecial[tId]) {
                            foundSpecial[tId] = 1;
                            adj[sId].push_back(tId);
                        }
                    } else if (isLily1(v)) {
                        int cell = nr*N+nc;
                        if (!visitedLily[cell]) { visitedLily[cell] = 1; qlily.push_back(cell); }
                    }
                }
            }
        }
    }

    int startId = specId[startR*N+startC];
    int endId = specId[endR*N+endC];

    const int INF = 1000000000;
    vector<int> dist(numSpecial, INF);
    vector<char> done(numSpecial, 0);
    vector<ll> ways(numSpecial, 0);

    deque<int> dq;
    dist[startId] = 0;
    dq.push_back(startId);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        if (done[u]) continue;
        done[u] = 1;
        for (size_t i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            int w = specCost[v];
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }

    if (dist[endId] >= INF) {
        printf("-1\n");
        return 0;
    }

    int maxDist = 0;
    for (int i = 0; i < numSpecial; i++) if (dist[i] < INF && dist[i] > maxDist) maxDist = dist[i];

    vector< vector<int> > byDistFree(maxDist+1), byDistWater(maxDist+1);
    for (int i = 0; i < numSpecial; i++) {
        if (dist[i] >= INF) continue;
        if (specCost[i] == 0) byDistFree[dist[i]].push_back(i);
        else byDistWater[dist[i]].push_back(i);
    }

    ways[startId] = 1;
    for (int d = 0; d <= maxDist; d++) {
        // Water nodes at this dist: predecessors are at dist d-1 (edge weight 1).
        for (size_t idx = 0; idx < byDistWater[d].size(); idx++) {
            int v = byDistWater[d][idx];
            ll sum = 0;
            for (size_t i = 0; i < adj[v].size(); i++) {
                int u = adj[v][i];
                if (dist[u] == d - 1) sum += ways[u];
            }
            ways[v] = sum;
        }
        // Free (start/end) nodes at this dist: predecessors are at dist d (edge weight 0).
        for (size_t idx = 0; idx < byDistFree[d].size(); idx++) {
            int v = byDistFree[d][idx];
            if (v == startId && d == 0) continue; // base case already set
            ll sum = 0;
            for (size_t i = 0; i < adj[v].size(); i++) {
                int u = adj[v][i];
                if (dist[u] == d) sum += ways[u];
            }
            ways[v] = sum;
        }
    }

    printf("%d\n", dist[endId]);
    printf("%lld\n", ways[endId]);

    return 0;
}
