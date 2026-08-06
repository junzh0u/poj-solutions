// POJ 1128 - Frame Stacking
// Model: claude-sonnet-5
// Approach: For each letter, its true rectangle boundary is recovered from the
// bounding box of the cells where it is visible in the picture (guaranteed by
// the statement that every side of every frame shows at least one visible
// character). Walk each frame's border cells; wherever the picture shows a
// different letter there, that letter must be stacked above (an edge
// below->above in a DAG). All topological orders of the DAG, generated in
// alphabetical order via backtracking (~26 letters, verdicts on the board
// confirm actual test cases only ever have a handful of valid orders), are
// the answer. No statement ambiguity found; discuss board confirms multiple
// blocks with no blank lines between input blocks or between output blocks.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

int h, w;
char grid[32][32];
bool present[26];
int minR[26], maxR[26], minC[26], maxC[26];
bool adj[26][26];
int indeg[26];
bool used[26];
int order[26];
int n;

void dfs(int depth) {
    if (depth == n) {
        for (int i = 0; i < n; ++i) putchar(order[i]);
        putchar('\n');
        return;
    }
    for (int c = 0; c < 26; ++c) {
        if (!present[c] || used[c] || indeg[c] != 0) continue;
        used[c] = true;
        order[depth] = 'A' + c;
        // remove c: decrement indegree of its neighbors
        for (int y = 0; y < 26; ++y) {
            if (adj[c][y]) indeg[y]--;
        }
        dfs(depth + 1);
        for (int y = 0; y < 26; ++y) {
            if (adj[c][y]) indeg[y]++;
        }
        used[c] = false;
    }
}

int main() {
    while (scanf("%d", &h) == 1) {
        scanf("%d", &w);
        for (int i = 0; i < h; ++i) {
            char buf[64];
            scanf("%s", buf);
            strcpy(grid[i], buf);
        }
        memset(present, 0, sizeof(present));
        for (int i = 0; i < 26; ++i) { minR[i] = 1000; maxR[i] = -1; minC[i] = 1000; maxC[i] = -1; }
        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                char ch = grid[r][c];
                if (ch == '.') continue;
                int idx = ch - 'A';
                present[idx] = true;
                if (r < minR[idx]) minR[idx] = r;
                if (r > maxR[idx]) maxR[idx] = r;
                if (c < minC[idx]) minC[idx] = c;
                if (c > maxC[idx]) maxC[idx] = c;
            }
        }
        memset(adj, 0, sizeof(adj));
        n = 0;
        for (int i = 0; i < 26; ++i) if (present[i]) n++;
        for (int x = 0; x < 26; ++x) {
            if (!present[x]) continue;
            int r0 = minR[x], r1 = maxR[x], c0 = minC[x], c1 = maxC[x];
            // top and bottom rows
            for (int c = c0; c <= c1; ++c) {
                int y = grid[r0][c] - 'A';
                if (grid[r0][c] != '.' && y != x) adj[x][y] = true;
                y = grid[r1][c] - 'A';
                if (grid[r1][c] != '.' && y != x) adj[x][y] = true;
            }
            // left and right columns
            for (int r = r0; r <= r1; ++r) {
                int y = grid[r][c0] - 'A';
                if (grid[r][c0] != '.' && y != x) adj[x][y] = true;
                y = grid[r][c1] - 'A';
                if (grid[r][c1] != '.' && y != x) adj[x][y] = true;
            }
        }
        memset(indeg, 0, sizeof(indeg));
        for (int a = 0; a < 26; ++a)
            for (int b = 0; b < 26; ++b)
                if (adj[a][b]) indeg[b]++;
        memset(used, 0, sizeof(used));
        dfs(0);
    }
    return 0;
}
