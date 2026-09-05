// POJ 2733 - The Game of Efil
// Model: claude-sonnet-5
// Approach: reverse Game of Life on a toroidal grid with m*n <= 16.
// Brute-force every one of the 2^(m*n) possible parent configurations,
// apply Conway's rule with wraparound (counting a wrapped neighbor with
// its full multiplicity, as the statement explicitly requires for small
// grids where an offset can map onto the same cell more than once), and
// compare the resulting generation against the given target. Count how
// many parent masks reproduce it exactly.
// Ambiguity: none found in the statement itself; the sample's note about
// double-counted wrapped neighbors on a 2x3 grid is the only subtlety,
// and is handled by enumerating the 8 directional offsets independently
// per cell (rather than deduplicating them) when counting neighbors.
#include <cstdio>
#include <cstring>

int main() {
    int m, n;
    int caseNo = 0;
    while (scanf("%d %d", &m, &n) == 2) {
        if (m == 0 && n == 0) break;
        ++caseNo;
        int k;
        scanf("%d", &k);
        int cells = m * n;
        unsigned int target = 0;
        for (int i = 0; i < k; ++i) {
            int r, c;
            scanf("%d %d", &r, &c);
            int idx = r * n + c;
            target |= (1u << idx);
        }

        // For each cell, the list of its 8 toroidal neighbor indices,
        // keeping repeats (a wrapped offset may land on the same cell
        // more than once on a small grid, and each occurrence counts).
        int neigh[16][8];
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                int idx = r * n + c;
                int cnt = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int rr = ((r + dr) % m + m) % m;
                        int cc = ((c + dc) % n + n) % n;
                        neigh[idx][cnt++] = rr * n + cc;
                    }
                }
            }
        }

        unsigned int total = 1u << cells;
        long ancestors = 0;
        for (unsigned int mask = 0; mask < total; ++mask) {
            unsigned int result = 0;
            for (int idx = 0; idx < cells; ++idx) {
                int cnt = 0;
                for (int j = 0; j < 8; ++j) {
                    if (mask & (1u << neigh[idx][j])) ++cnt;
                }
                int occupied = (mask >> idx) & 1;
                int newOccupied = 0;
                if (occupied) {
                    if (cnt == 2 || cnt == 3) newOccupied = 1;
                } else {
                    if (cnt == 3) newOccupied = 1;
                }
                if (newOccupied) result |= (1u << idx);
            }
            if (result == target) ++ancestors;
        }

        if (ancestors == 0) {
            printf("Case %d: Garden of Eden.\n", caseNo);
        } else {
            printf("Case %d: %ld possible ancestors.\n", caseNo, ancestors);
        }
    }
    return 0;
}
