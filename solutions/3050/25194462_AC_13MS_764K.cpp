// POJ 3050 - Hopscotch
//
// A 5x5 grid of digits. Starting on any cell, make exactly five orthogonal
// hops (up/down/left/right, never diagonal, staying inside the grid) and read
// off the six digits visited in order. Count the *distinct* six-digit strings
// obtainable. Leading zeroes are significant, so "000201" and "201" are
// different values only in the sense that the string always has length 6 --
// comparing strings rather than integers is the safe reading.
//
// The key point in the statement is that the walk may revisit cells ("a digit
// already visited"), so this is not a simple-path search: every one of the
// 25 * 4^5 = 25600 length-6 walks is legal as long as it stays on the board.
// That makes plain exhaustive DFS with no visited-marking correct and tiny.
// The other trap is that the count is of distinct *strings*, not of distinct
// walks -- many walks spell the same number -- so the results go into a
// std::set<std::string> and the answer is its size.
//
// Under 26k walks, so it runs in a few milliseconds; verified against an
// independently written BFS-by-length search and on degenerate grids (all
// cells equal -> 1).

#include <cstdio>
#include <set>
#include <string>

int g[5][5];
std::set<std::string> seen;

const int DR[4] = { -1, 1, 0, 0 };
const int DC[4] = { 0, 0, -1, 1 };

void dfs(int r, int c, std::string s)
{
    s += (char)('0' + g[r][c]);
    if (s.size() == 6) {
        seen.insert(s);
        return;
    }
    for (int d = 0; d < 4; ++d) {
        int nr = r + DR[d], nc = c + DC[d];
        if (nr < 0 || nr > 4 || nc < 0 || nc > 4)
            continue;
        dfs(nr, nc, s);
    }
}

int main()
{
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (scanf("%d", &g[i][j]) != 1)
                return 1;

    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            dfs(i, j, std::string());

    printf("%d\n", (int)seen.size());
    return 0;
}
