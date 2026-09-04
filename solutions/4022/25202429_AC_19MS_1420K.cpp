// POJ 4022 - ASCII Area
// Model: claude-sonnet-5
//
// Each unit cell of the picture is either empty ('.') or split by a
// diagonal ('/' or '\') into two right triangles.  Treat every cell as
// contributing one or two "half-cell" nodes to a graph: a '.' cell is a
// single node touching all four of its edges (top/bottom/left/right); a
// '/' cell has a top-left node (touching top+left) and a bottom-right
// node (touching bottom+right); a '\' cell has a top-right node
// (touching top+right) and a bottom-left node (touching bottom+left).
// Connect a cell's node touching a given side to the neighboring cell's
// node touching the opposite side (no wall exists except along the
// drawn diagonals themselves), and connect every node that touches the
// picture's outer border to a virtual OUTSIDE node.  A BFS/DFS from
// OUTSIDE marks every node that is actually outside the polygon; every
// unmarked node is inside.  Summing 1 unit for an inside '.' cell and
// 1/2 unit for each inside triangle gives the polygon's area directly.
// Verified by hand against the sample (expected 8) before coding.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int h, w;
    if (scanf("%d %d", &h, &w) != 2) return 0;
    vector<string> grid(h);
    for (int r = 0; r < h; ++r) {
        char buf[110];
        scanf("%s", buf);
        grid[r] = buf;
    }

    int n = h * w;
    int OUTSIDE = 2 * n;
    vector<vector<int> > adj(2 * n + 1);

    vector<vector<int> > topId(h, vector<int>(w)), botId(h, vector<int>(w));
    vector<vector<int> > leftId(h, vector<int>(w)), rightId(h, vector<int>(w));

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            int idx = r * w + c;
            int lo = 2 * idx, hi = 2 * idx + 1;
            char ch = grid[r][c];
            if (ch == '.') {
                topId[r][c] = botId[r][c] = leftId[r][c] = rightId[r][c] = lo;
            } else if (ch == '/') {
                topId[r][c] = leftId[r][c] = lo;
                botId[r][c] = rightId[r][c] = hi;
            } else { // '\'
                topId[r][c] = rightId[r][c] = lo;
                botId[r][c] = leftId[r][c] = hi;
            }
        }
    }

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (c + 1 < w) {
                adj[rightId[r][c]].push_back(leftId[r][c + 1]);
                adj[leftId[r][c + 1]].push_back(rightId[r][c]);
            } else {
                adj[rightId[r][c]].push_back(OUTSIDE);
                adj[OUTSIDE].push_back(rightId[r][c]);
            }
            if (r + 1 < h) {
                adj[botId[r][c]].push_back(topId[r + 1][c]);
                adj[topId[r + 1][c]].push_back(botId[r][c]);
            } else {
                adj[botId[r][c]].push_back(OUTSIDE);
                adj[OUTSIDE].push_back(botId[r][c]);
            }
            if (c == 0) {
                adj[leftId[r][c]].push_back(OUTSIDE);
                adj[OUTSIDE].push_back(leftId[r][c]);
            }
            if (r == 0) {
                adj[topId[r][c]].push_back(OUTSIDE);
                adj[OUTSIDE].push_back(topId[r][c]);
            }
        }
    }

    vector<char> visited(2 * n + 1, 0);
    queue<int> q;
    visited[OUTSIDE] = 1;
    q.push(OUTSIDE);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = 1;
                q.push(v);
            }
        }
    }

    long twiceArea = 0;
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            int idx = r * w + c;
            int lo = 2 * idx, hi = 2 * idx + 1;
            char ch = grid[r][c];
            if (ch == '.') {
                if (!visited[lo]) twiceArea += 2;
            } else {
                if (!visited[lo]) twiceArea += 1;
                if (!visited[hi]) twiceArea += 1;
            }
        }
    }

    printf("%ld\n", twiceArea / 2);
    return 0;
}
