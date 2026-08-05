// POJ 2446 - Chessboard
//
// An m x n board with k holes; tile every non-hole cell with 1x2 dominoes.
//
// Colour the board like a chessboard: a domino always covers one black and one
// white cell, so this is a perfect matching on the bipartite graph whose parts
// are the free black cells and the free white cells, edges joining orthogonally
// adjacent free cells. Necessary counting condition first — if the two parts
// differ in size (which also covers an odd number of free cells) the answer is
// NO without running any matching. Otherwise run Kuhn's augmenting-path
// algorithm from every black cell and answer YES iff the matching saturates
// them all. With m,n <= 32 there are at most 512 vertices per side and 4 edges
// per vertex, so O(V*E) is far inside the 2s limit (12MS on the judge).
//
// Statement ambiguity: the first line is "m n k" with m = rows and n = columns,
// but each hole is given as "x y" meaning column x, row y — the pair is
// transposed relative to the dimensions. Holes may also repeat, so they are
// stored in a boolean grid rather than counted.
#include <cstdio>
#include <cstring>

static const int MAXR = 34, MAXC = 34, MAXN = MAXR * MAXC;

static bool hole[MAXR][MAXC];
static int id[MAXR][MAXC];
static int m, n, k;

static int adj[MAXN][4];
static int deg[MAXN];
static int matchTo[MAXN];
static bool used[MAXN];

static bool tryKuhn(int v) {
    for (int i = 0; i < deg[v]; ++i) {
        int to = adj[v][i];
        if (used[to]) continue;
        used[to] = true;
        if (matchTo[to] == -1 || tryKuhn(matchTo[to])) {
            matchTo[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    if (scanf("%d %d %d", &m, &n, &k) != 3) return 0;
    memset(hole, 0, sizeof(hole));
    for (int i = 0; i < k; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        /* x = column, y = row */
        if (y >= 1 && y <= m && x >= 1 && x <= n) hole[y][x] = true;
    }

    int cntBlack = 0, cntWhite = 0;
    memset(id, -1, sizeof(id));
    for (int r = 1; r <= m; ++r)
        for (int c = 1; c <= n; ++c) {
            if (hole[r][c]) continue;
            if (((r + c) & 1) == 0) id[r][c] = cntBlack++;
            else id[r][c] = cntWhite++;
        }

    if (cntBlack != cntWhite) { printf("NO\n"); return 0; }

    for (int i = 0; i < cntBlack; ++i) deg[i] = 0;
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    for (int r = 1; r <= m; ++r)
        for (int c = 1; c <= n; ++c) {
            if (hole[r][c] || ((r + c) & 1) != 0) continue;
            int u = id[r][c];
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 1 || nr > m || nc < 1 || nc > n) continue;
                if (hole[nr][nc]) continue;
                adj[u][deg[u]++] = id[nr][nc];
            }
        }

    for (int i = 0; i < cntWhite; ++i) matchTo[i] = -1;
    int res = 0;
    for (int v = 0; v < cntBlack; ++v) {
        for (int i = 0; i < cntWhite; ++i) used[i] = false;
        if (tryKuhn(v)) ++res;
    }

    printf("%s\n", res == cntBlack ? "YES" : "NO");
    return 0;
}
