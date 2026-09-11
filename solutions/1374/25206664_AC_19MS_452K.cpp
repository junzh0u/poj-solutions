// POJ 1374 - Crosswords
// Model: claude-sonnet-5
//
// Approach: number white squares per the statement's across/down-start rule
// (row-major numbering). Black squares that are 4-directionally connected to
// the border of the matrix are "edge" black squares and are drawn as nothing
// (pure whitespace, like padding); black squares fully enclosed and never
// reaching the border are drawn as solid black boxes. Each cell occupies a
// 4x6 character box that shares its border row/column with neighboring
// cells, so the whole diagram is a canvas of (3*m+1) rows by (5*n+1)
// columns. For every canvas position we look at which grid cell(s) it
// belongs to (1, 2, or 4, depending on whether it sits on a horizontal
// border, vertical border, or interior) and draw '+' if any touching cell is
// not an edge-black cell, else a space; interior content positions are
// filled with the (zero-padded 3-digit) number when applicable. Trailing
// spaces are stripped from every printed line.
//
// Ambiguity: the statement's "removed from the diagram" phrasing for edge
// black squares is only clarified by the sample - in particular the second
// sample crossword has an entire row of black squares in the middle of the
// grid, but since every one of those cells is 4-connected (through other
// black cells) back to the matrix border, that whole row disappears from the
// diagram (contributing purely blank lines), rather than being drawn solid.
// This flood-fill-from-the-border reading matches the sample output
// byte-for-byte; a naive "black square in row 0 / col 0 / last row / last
// col only" reading would not, since the all-black middle row touches no
// matrix edge directly through row/col index alone, only through
// connectivity.

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <queue>
using namespace std;

int main() {
    int m, n;
    bool first = true;
    while (scanf("%d %d", &m, &n) == 2) {
        if (m == 0 && n == 0) break;
        vector<vector<int> > a(m, vector<int>(n));
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                scanf("%d", &a[i][j]);

        // Numbering
        vector<vector<int> > num(m, vector<int>(n, 0));
        int counter = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (a[i][j] != 0) continue;
                bool downStart = (i + 1 < m && a[i + 1][j] == 0) &&
                                  (i - 1 < 0 || a[i - 1][j] == 1);
                bool acrossStart = (j + 1 < n && a[i][j + 1] == 0) &&
                                    (j - 1 < 0 || a[i][j - 1] == 1);
                if (downStart || acrossStart) {
                    ++counter;
                    num[i][j] = counter;
                }
            }
        }

        // Edge-connected black cells (removed), via BFS from border black cells.
        vector<vector<bool> > removed(m, vector<bool>(n, false));
        vector<vector<bool> > visited(m, vector<bool>(n, false));
        queue<pair<int,int> > q;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (a[i][j] == 1 && (i == 0 || i == m - 1 || j == 0 || j == n - 1) && !visited[i][j]) {
                    visited[i][j] = true;
                    removed[i][j] = true;
                    q.push(make_pair(i, j));
                }
            }
        }
        int di[4] = {-1, 1, 0, 0};
        int dj[4] = {0, 0, -1, 1};
        while (!q.empty()) {
            pair<int,int> cur = q.front(); q.pop();
            int ci = cur.first, cj = cur.second;
            for (int d = 0; d < 4; ++d) {
                int ni = ci + di[d], nj = cj + dj[d];
                if (ni < 0 || ni >= m || nj < 0 || nj >= n) continue;
                if (a[ni][nj] == 1 && !visited[ni][nj]) {
                    visited[ni][nj] = true;
                    removed[ni][nj] = true;
                    q.push(make_pair(ni, nj));
                }
            }
        }

        // type: 0 = white, 1 = removed(black,edge), 2 = solid(black,interior)
        vector<vector<int> > type(m, vector<int>(n));
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                type[i][j] = (a[i][j] == 0) ? 0 : (removed[i][j] ? 1 : 2);

        int R = 3 * m + 1;
        int C = 5 * n + 1;
        vector<string> canvas(R, string(C, ' '));

        for (int r = 0; r < R; ++r) {
            bool rowBorder = (r % 3 == 0);
            for (int c = 0; c < C; ++c) {
                bool colBorder = (c % 5 == 0);
                char ch = ' ';
                if (rowBorder && colBorder) {
                    int rb = r / 3;
                    int cb = c / 5;
                    bool any = false;
                    for (int ri = rb - 1; ri <= rb && !any; ++ri) {
                        if (ri < 0 || ri >= m) continue;
                        for (int cj = cb - 1; cj <= cb && !any; ++cj) {
                            if (cj < 0 || cj >= n) continue;
                            if (type[ri][cj] != 1) any = true;
                        }
                    }
                    ch = any ? '+' : ' ';
                } else if (rowBorder) {
                    int rb = r / 3;
                    int col = c / 5;
                    bool any = false;
                    for (int ri = rb - 1; ri <= rb; ++ri) {
                        if (ri < 0 || ri >= m) continue;
                        if (type[ri][col] != 1) any = true;
                    }
                    ch = any ? '+' : ' ';
                } else if (colBorder) {
                    int row = r / 3;
                    int cb = c / 5;
                    bool any = false;
                    for (int cj = cb - 1; cj <= cb; ++cj) {
                        if (cj < 0 || cj >= n) continue;
                        if (type[row][cj] != 1) any = true;
                    }
                    ch = any ? '+' : ' ';
                } else {
                    int row = r / 3;
                    int col = c / 5;
                    int t = type[row][col];
                    if (t == 1) {
                        ch = ' ';
                    } else if (t == 2) {
                        ch = '+';
                    } else {
                        int relR = r % 3;
                        int relC = c % 5;
                        if (relR == 1 && relC >= 1 && relC <= 3 && num[row][col] != 0) {
                            char buf[8];
                            sprintf(buf, "%03d", num[row][col]);
                            ch = buf[relC - 1];
                        } else {
                            ch = ' ';
                        }
                    }
                }
                canvas[r][c] = ch;
            }
        }

        if (!first) {
            printf("\n\n");
        }
        first = false;

        for (int r = 0; r < R; ++r) {
            string &s = canvas[r];
            int end = (int)s.size();
            while (end > 0 && s[end - 1] == ' ') --end;
            for (int i = 0; i < end; ++i) putchar(s[i]);
            putchar('\n');
        }
    }
    return 0;
}
