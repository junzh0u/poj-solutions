// POJ 2458 - Rigging the Bovine Election
// Model: claude-opus-5
//
// Count the 7-cell subsets of a 5x5 grid that are connected under
// 4-adjacency and hold strictly more 'J' than 'H'.
//
// The whole search space is C(25,7) = 480700 subsets, so plain enumeration
// retires the problem: recurse over increasing cell indices building a
// 25-bit mask, and for each complete 7-subset test the majority (7 cells,
// so "the Jerseys outnumber the Holsteins" means at least 4 J) and then
// connectivity by flooding the mask from its lowest set bit.  Roughly
// 5e5 masks, each flooded in at most 7 rounds of a 25-bit scan.
//
// "Contiguous" is spelled out by the statement as vertical/horizontal only,
// so diagonals do not connect.  Nothing else in the statement is loose.
//
// The answer is at most the 3546 connected 7-cell subsets of a 5x5 grid,
// so int suffices and no long long / %lld is needed.
#include <cstdio>

char g[8][16];
int isJ[25];
int adj[25];   /* adj[i] = bitmask of the 4-neighbours of cell i */
int ans;       /* C(25,7) = 480700, so int is ample */

/* flood-fill inside the chosen set, starting from its lowest cell */
static int connected(int mask)
{
    int start = 0;
    while (!((mask >> start) & 1)) ++start;
    int seen = 1 << start;
    int frontier = seen;
    while (frontier) {
        int nxt = 0;
        for (int i = 0; i < 25; ++i)
            if ((frontier >> i) & 1) nxt |= adj[i];
        nxt &= mask;
        nxt &= ~seen;
        seen |= nxt;
        frontier = nxt;
    }
    return seen == mask;
}

static void rec(int idx, int start, int mask, int j)
{
    if (idx == 7) {
        if (j >= 4 && connected(mask)) ++ans;
        return;
    }
    /* prune: not enough cells left to finish the district */
    for (int c = start; c <= 25 - (7 - idx); ++c)
        rec(idx + 1, c + 1, mask | (1 << c), j + isJ[c]);
}

int main()
{
    for (int r = 0; r < 5; ++r)
        if (scanf("%s", g[r]) != 1) return 0;

    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c) {
            int id = r * 5 + c;
            isJ[id] = (g[r][c] == 'J') ? 1 : 0;
            int m = 0;
            if (r > 0) m |= 1 << (id - 5);
            if (r < 4) m |= 1 << (id + 5);
            if (c > 0) m |= 1 << (id - 1);
            if (c < 4) m |= 1 << (id + 1);
            adj[id] = m;
        }

    ans = 0;
    rec(0, 0, 0, 0);
    printf("%d\n", ans);
    return 0;
}
