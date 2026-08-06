// POJ 3020 - Antenna Placement
//
// Each antenna covers either one '*' cell alone, or a domino of two
// adjacent '*' cells (one antenna serves both). Minimizing antenna
// count is therefore: (number of '*' cells) - (max number of cells
// that can be paired off with an adjacent '*' cell), i.e. the answer
// is cnt - maximum_matching, where the matching is over the graph
// whose vertices are '*' cells and whose edges connect grid-adjacent
// '*' cells.
//
// That graph is bipartite by construction: 2-color the grid like a
// chessboard (r+c even/odd) and every edge crosses colors, since grid
// neighbors always differ in (r+c) parity. So a plain augmenting-path
// (Kuhn's) bipartite matching finds the true maximum matching of the
// graph (no need for general-graph matching / blossoms). Grid is at
// most 40x10 = 400 cells, so O(V*E) Kuhn's is comfortably fast.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

int h, w;
char grid[45][15];
int idOf[45][15];
vector<int> adj[405];
int matchR[405];
bool used[405];

bool tryK(int u) {
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (used[v]) continue;
        used[v] = true;
        if (matchR[v] == -1 || tryK(matchR[v])) {
            matchR[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    while (n--) {
        cin >> h >> w;
        for (int r = 0; r < h; ++r) {
            string row;
            cin >> row;
            for (int c = 0; c < w; ++c) grid[r][c] = row[c];
        }
        int cnt = 0;
        int leftCount = 0;
        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                idOf[r][c] = -1;
                if (grid[r][c] == '*') {
                    ++cnt;
                    if ((r + c) % 2 == 0) {
                        idOf[r][c] = leftCount++;
                    }
                }
            }
        }
        for (int i = 0; i < leftCount; ++i) adj[i].clear();
        // assign right ids
        int rightCount = 0;
        int rightId[45][15];
        for (int r = 0; r < h; ++r)
            for (int c = 0; c < w; ++c)
                rightId[r][c] = -1;
        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                if (grid[r][c] == '*' && (r + c) % 2 == 1) {
                    rightId[r][c] = rightCount++;
                }
            }
        }
        static const int dr[4] = {-1, 1, 0, 0};
        static const int dc[4] = {0, 0, -1, 1};
        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                if (grid[r][c] == '*' && (r + c) % 2 == 0) {
                    int u = idOf[r][c];
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + dr[k];
                        int nc = c + dc[k];
                        if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
                        if (grid[nr][nc] == '*' && rightId[nr][nc] != -1) {
                            adj[u].push_back(rightId[nr][nc]);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < rightCount; ++i) matchR[i] = -1;
        int matching = 0;
        for (int u = 0; u < leftCount; ++u) {
            for (int i = 0; i < rightCount; ++i) used[i] = false;
            if (tryK(u)) ++matching;
        }
        printf("%d\n", cnt - matching);
    }
    return 0;
}
