// POJ 3182 - The Grove
// Model: claude-sonnet-5
//
// Approach: The pasture minus the grove (a single hole-free blob) is an
// annulus, so the shortest closed walk that winds once around the grove
// cannot be found by a plain BFS distance (start to itself is trivially 0).
// Standard trick: pick the topmost, then leftmost tree cell (lx,ly). Row
// lx-1 is then guaranteed entirely tree-free (lx is the smallest row that
// contains any tree at all). Cut the grid along the horizontal boundary
// between row lx-1 and row lx, for columns 1..ly (i.e. from the grid's
// left edge up to directly above that tree) -- this is a curve from the
// grid boundary to the grove, slicing the annulus into a simply connected
// region. Forbidding BFS moves that cross this cut turns the graph
// distances from the start into single-valued shortest paths (no more
// winding ambiguity). The true shortest closed tour must cross the cut
// exactly once, at one of the (row lx-1, row lx) edges spanning columns
// 1..ly, so the answer is min over those crossing edges of
// dist(start,A) + 1 + dist(B,start), i.e. dist[A] + dist[B] + 1.
//
// Separately (and orthogonally to the cut), the statement warns that a
// diagonal step may not "pass through" the grove: if a diagonal move from
// (r,c) to (r+dr,c+dc) would squeeze between two trees that touch only at
// a corner -- i.e. (r+dr,c) and (r,c+dc) are both trees -- that move is
// disallowed everywhere in the grid, not just near the cut. (It can never
// affect the cut-crossing edges themselves, since one endpoint of every
// such edge lies on row lx-1, which is provably tree-free.)
//
// No statement ambiguity found; the discuss board only confirms the
// cut-a-ray-and-BFS-twice idea, without discussing the diagonal-corner
// case, so that case was worked out directly from the problem text.

#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

int R, C;
char grid[55][55];
bool pass_[55][55];
int dist_[55][55];

int main() {
    scanf("%d %d", &R, &C);
    for (int i = 1; i <= R; i++) {
        scanf("%s", grid[i] + 1);
    }

    int sx = -1, sy = -1, lx = -1, ly = -1;
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++) {
            char ch = grid[i][j];
            pass_[i][j] = (ch != 'X');
            if (ch == '*') { sx = i; sy = j; }
            if (ch == 'X' && lx == -1) { lx = i; ly = j; }
        }
    }

    memset(dist_, -1, sizeof(dist_));
    static const int dr[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
    static const int dc[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

    dist_[sx][sy] = 0;
    queue<pair<int,int> > q;
    q.push(make_pair(sx, sy));
    while (!q.empty()) {
        int r = q.front().first, c = q.front().second;
        q.pop();
        for (int k = 0; k < 8; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 1 || nr > R || nc < 1 || nc > C) continue;
            if (!pass_[nr][nc]) continue;
            if (dist_[nr][nc] != -1) continue;

            // diagonal corner-squeeze rule
            if (dr[k] != 0 && dc[k] != 0) {
                int cr = r + dr[k], cc = c;      // (r+dr, c)
                int cr2 = r, cc2 = c + dc[k];     // (r, c+dc)
                bool corner1_tree = (cr < 1 || cr > R || cc < 1 || cc > C) || !pass_[cr][cc];
                bool corner2_tree = (cr2 < 1 || cr2 > R || cc2 < 1 || cc2 > C) || !pass_[cr2][cc2];
                if (corner1_tree && corner2_tree) continue;
            }

            // cut-ray rule: forbid crossing between row lx-1 and row lx
            // for source column <= ly
            if (c <= ly && r == lx && nr == lx - 1) continue;
            if (c <= ly && r == lx - 1 && nr == lx) continue;

            dist_[nr][nc] = dist_[r][c] + 1;
            q.push(make_pair(nr, nc));
        }
    }

    const int INF = 1 << 30;
    int ans = INF;
    for (int i = 1; i <= ly; i++) {
        int a = dist_[lx][i];
        if (a == -1) continue;
        int cand[3];
        cand[0] = (i >= 1 && i <= C) ? dist_[lx - 1][i] : -1;
        cand[1] = (i + 1 >= 1 && i + 1 <= C) ? dist_[lx - 1][i + 1] : -1;
        cand[2] = (i - 1 >= 1 && i - 1 <= C) ? dist_[lx - 1][i - 1] : -1;
        for (int t = 0; t < 3; t++) {
            if (cand[t] == -1) continue;
            int total = a + cand[t] + 1;
            if (total < ans) ans = total;
        }
    }

    printf("%d\n", ans);
    return 0;
}
