// POJ 2734 - Queens, Knights and Pawns
// Model: claude-opus-5
//
// Count squares that are unoccupied and attacked by no queen and no knight.
// Board is up to 1000x1000 (1e6 cells) but there are at most ~100 pieces of
// each kind, and the number of test cases is not bounded by the statement, so
// nothing here is O(n*m): the answer is
//     n*m - (distinct occupied cells) - (distinct attacked, unoccupied cells)
// Each queen walks its 8 rays (<= 8*1000 cells, stopping at the first occupied
// square, which does block it), each knight marks its 8 jump targets (never
// blocked). Marks are recorded in a list and cleared cell by cell afterwards,
// so per-test work is O(pieces * board_side) with no full-board sweep or memset.
//
// Pawns only occupy squares and block queens; they attack nothing.
// Pieces are only used if they lie on the board (defensive; the data is sane).

#include <cstdio>

static const int MAXCELL = 1000000;
static const int MAXP = 100000;

static unsigned char occ[MAXCELL];
static unsigned char att[MAXCELL];
static int attList[MAXCELL];

static int qr[MAXP], qc[MAXP];
static int kr[MAXP], kc[MAXP];
static int pr[MAXP], pc[MAXP];

static const int QD[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
};

static const int KD[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    { 1, -2}, { 1, 2}, { 2, -1}, { 2, 1}
};

static int readPieces(int *rows, int *cols)
{
    int k = 0;
    if (scanf("%d", &k) != 1) return 0;
    for (int i = 0; i < k; ++i) {
        int r = 0, c = 0;
        if (scanf("%d %d", &r, &c) != 2) return i;
        if (i < MAXP) { rows[i] = r; cols[i] = c; }
    }
    if (k > MAXP) k = MAXP;
    return k;
}

int main()
{
    int n = 0, m = 0;
    int board = 0;

    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        ++board;

        int nq = readPieces(qr, qc);
        int nk = readPieces(kr, kc);
        int np = readPieces(pr, pc);

        if (n < 0) n = 0;
        if (m < 0) m = 0;

        int occupied = 0;
        int nAtt = 0;

        if (n > 0 && m > 0) {
            /* mark every piece as an occupied (and therefore blocking) square */
            for (int t = 0; t < 3; ++t) {
                const int *rs = (t == 0) ? qr : (t == 1) ? kr : pr;
                const int *cs = (t == 0) ? qc : (t == 1) ? kc : pc;
                int cnt = (t == 0) ? nq : (t == 1) ? nk : np;
                for (int i = 0; i < cnt; ++i) {
                    int r = rs[i], c = cs[i];
                    if (r < 1 || r > n || c < 1 || c > m) continue;
                    int idx = (r - 1) * m + (c - 1);
                    if (!occ[idx]) { occ[idx] = 1; ++occupied; }
                }
            }

            /* queens: ray attacks, blocked by the first occupied square */
            for (int i = 0; i < nq; ++i) {
                int r0 = qr[i], c0 = qc[i];
                if (r0 < 1 || r0 > n || c0 < 1 || c0 > m) continue;
                for (int d = 0; d < 8; ++d) {
                    int r = r0 + QD[d][0], c = c0 + QD[d][1];
                    while (r >= 1 && r <= n && c >= 1 && c <= m) {
                        int idx = (r - 1) * m + (c - 1);
                        if (occ[idx]) break;
                        if (!att[idx]) { att[idx] = 1; attList[nAtt++] = idx; }
                        r += QD[d][0];
                        c += QD[d][1];
                    }
                }
            }

            /* knights: eight jump targets, never blocked */
            for (int i = 0; i < nk; ++i) {
                int r0 = kr[i], c0 = kc[i];
                if (r0 < 1 || r0 > n || c0 < 1 || c0 > m) continue;
                for (int d = 0; d < 8; ++d) {
                    int r = r0 + KD[d][0], c = c0 + KD[d][1];
                    if (r < 1 || r > n || c < 1 || c > m) continue;
                    int idx = (r - 1) * m + (c - 1);
                    if (occ[idx]) continue;
                    if (!att[idx]) { att[idx] = 1; attList[nAtt++] = idx; }
                }
            }
        }

        int safe = n * m - occupied - nAtt;
        printf("Board %d has %d safe squares.\n", board, safe);

        /* clear only what was touched */
        for (int i = 0; i < nAtt; ++i) att[attList[i]] = 0;
        if (n > 0 && m > 0) {
            for (int t = 0; t < 3; ++t) {
                const int *rs = (t == 0) ? qr : (t == 1) ? kr : pr;
                const int *cs = (t == 0) ? qc : (t == 1) ? kc : pc;
                int cnt = (t == 0) ? nq : (t == 1) ? nk : np;
                for (int i = 0; i < cnt; ++i) {
                    int r = rs[i], c = cs[i];
                    if (r < 1 || r > n || c < 1 || c > m) continue;
                    occ[(r - 1) * m + (c - 1)] = 0;
                }
            }
        }
    }

    return 0;
}
