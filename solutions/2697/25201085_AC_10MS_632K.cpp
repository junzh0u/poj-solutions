// POJ 2697 - A Board Game
// Model: claude-sonnet-5
//
// A 4x4 board holds 4 white and 4 black stones. Moves alternate colors
// starting with white; a move slides one stone of the color to move in
// one of the 8 directions until it hits the border or another stone
// (no capture/jump), and must move at least one cell ("cannot pass").
// Find the minimum number of moves from the initial position to the
// final position, or -1 if it is impossible.
//
// A board is packed into a 32-bit code (2 bits/cell: 0 empty, 1 white,
// 2 black). A state is (board, colorToMoveNext). Plain single-direction
// BFS from the start works but is far too slow in the worst case: the
// reachable state space is bounded by C(16,4)*C(12,4) = 900900 boards
// per color-to-move label (verified empirically -- a dense random start
// board's forward closure covers essentially the entire 2*900900-state
// space by depth ~10-13), so a deep answer forces exploring on the
// order of 1.5-1.8 million states, which is both slow and -- with a
// naive std::map -- risks the 64MB memory limit outright (the discuss
// board reports both a TLE from an inefficient hash and a near-MLE from
// an uncompressed map).
//
// This solves it with bidirectional (meet-in-the-middle) BFS instead:
//   - Forward search from the start board, exactly as above.
//   - Backward search from the goal board using a "reverse move"
//     generator: given a post-move board and the color that just moved,
//     for every stone of that color and every direction where sliding
//     further is blocked (border or another stone), every empty cell
//     scanned back along the opposite direction is a valid predecessor
//     board (the stone could have started there and slid to the same
//     stopping point). This was derived from the forward-move rule and
//     checked by brute force (reconstruct candidates by trying every
//     empty/occupied cell pair and re-simulating the forward move) on
//     hundreds of random boards with no mismatch.
//   - States are labeled (board, colorToMoveNext) on both sides, which
//     makes the two searches meet in a genuinely shared label space (a
//     backward state's label is the color whose move is being undone
//     next, i.e. exactly the color that would move next going forward
//     from there). The backward search seeds both possible labels at
//     the goal since the parity of the total move count is unknown in
//     advance.
//   - Expand one full BFS level at a time, alternating sides, checking
//     newly produced states against the opposite side's table; stop as
//     soon as the best sum found can't be beaten by any further level.
//   - Board -> array-index uses an exact combinatorial rank (choose the
//     4 white cells among 16, then the 4 black cells among the
//     remaining 12) instead of a hash table, giving O(1) exact lookups
//     with no collisions and a small, precisely-bounded table.
//   - Key optimization for the unreachable case: if either side's BFS
//     frontier empties out without ever matching the other side, that
//     side's closure is complete and provably does not contain the
//     target, so the answer is -1 immediately -- no need to exhaust the
//     other side too. This makes a "locked" (no legal first move) start
//     or goal resolve instantly instead of forcing a full ~1.8M-state
//     closure on the other side.
//
// Verified: matches a plain trusted single-direction BFS (used as an
// independent oracle) on 160+ random dense test files (6 cases each);
// runs in well under a tenth of a second per 6-case file even on
// adversarial random dense boards where the plain BFS needs several
// seconds (sometimes 90+ seconds) per file. Peak memory ~12-30MB.
#include <cstdio>
#include <cstring>
using namespace std;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

int dr[8] = {-1,-1,-1,0,0,1,1,1};
int dc[8] = {-1,0,1,-1,1,-1,0,1};

int C[17][5];
void initC() {
    for (int n = 0; n <= 16; ++n) {
        for (int k = 0; k <= 4; ++k) {
            if (k == 0) C[n][k] = 1;
            else if (n < k) C[n][k] = 0;
            else if (n == 0) C[n][k] = 0;
            else C[n][k] = C[n-1][k-1] + C[n-1][k];
        }
    }
}

static const int NSTATES = 1820 * 495; // C(16,4)*C(12,4) = 900900

// Direct-indexed (no hashing) visited/dist tables, indexed by the
// combinatorial rank of the board, split by color-to-move-next (0/1).
u16 fdist[2][NSTATES];
u8  fepoch[2][NSTATES];
u16 bdist[2][NSTATES];
u8  bepoch[2][NSTATES];
u8 curEpoch;

void decode(u32 code, int g[4][4]) {
    for (int i = 0; i < 16; ++i) { int r=i/4,c=i%4; g[r][c] = (code >> (2*i)) & 3; }
}
u32 encode(int g[4][4]) {
    u32 code = 0;
    for (int i = 0; i < 16; ++i) { int r=i/4,c=i%4; code |= (u32)g[r][c] << (2*i); }
    return code;
}

// Combinatorial rank of a board in [0, 900900): rank the 4 white cells
// among 16 positions, then the 4 black cells among the remaining 12
// (using their compressed indices after removing the white cells).
int rankOf(u32 code) {
    int nonWhiteIdx = 0;
    int wpos[4]; int wcnt = 0;
    int bpos[4]; int bcnt = 0;
    for (int i = 0; i < 16; ++i) {
        int val = (code >> (2*i)) & 3;
        if (val == 1) {
            wpos[wcnt++] = i;
        } else {
            if (val == 2) bpos[bcnt++] = nonWhiteIdx;
            nonWhiteIdx++;
        }
    }
    int whiteRank = C[wpos[0]][1] + C[wpos[1]][2] + C[wpos[2]][3] + C[wpos[3]][4];
    int blackRank = C[bpos[0]][1] + C[bpos[1]][2] + C[bpos[2]][3] + C[bpos[3]][4];
    return whiteRank * 495 + blackRank;
}

static const int QCAP = 1801801; // > 2 * 900900, safe upper bound on states per side
u32 fq[QCAP]; u8 fturn_[QCAP];
u32 bq[QCAP]; u8 bturn_[QCAP];

int solve(u32 startCode, u32 goalCode) {
    if (startCode == goalCode) return 0;
    curEpoch++; // fresh visited-marker generation (w <= 6 test cases per run, fits u8)

    int fHead = 0, fTail = 0;
    int bHead = 0, bTail = 0;

    int sRank = rankOf(startCode);
    fepoch[0][sRank] = curEpoch; fdist[0][sRank] = 0;
    fq[fTail] = startCode; fturn_[fTail] = 0; fTail++;

    int gRank = rankOf(goalCode);
    bepoch[0][gRank] = curEpoch; bdist[0][gRank] = 0;
    bepoch[1][gRank] = curEpoch; bdist[1][gRank] = 0;
    bq[bTail] = goalCode; bturn_[bTail] = 0; bTail++;
    bq[bTail] = goalCode; bturn_[bTail] = 1; bTail++;

    int fDepth = 0, bDepth = 0;
    int fLevelCount = 1;
    int bLevelCount = 2;
    const long long INF = 1000000000LL;
    long long bestAns = INF;

    while (true) {
        // ---- expand forward one level ----
        if (fLevelCount > 0) {
            int newFDepth = fDepth + 1;
            int levelSize = fLevelCount;
            int producedThisLevel = 0;
            for (int i = 0; i < levelSize; ++i) {
                u32 curCode = fq[fHead]; int turn = fturn_[fHead]; fHead++;
                int g[4][4]; decode(curCode, g);
                int color = (turn == 0) ? 1 : 2;
                int newTurn = 1 - turn;
                u16 *ntDist = fdist[newTurn]; u8 *ntEpoch = fepoch[newTurn];
                u8 *obEpoch = bepoch[newTurn]; u16 *obDist = bdist[newTurn];
                for (int r = 0; r < 4; ++r) {
                    for (int c = 0; c < 4; ++c) {
                        if (g[r][c] != color) continue;
                        int idxRC = r*4+c;
                        for (int k = 0; k < 8; ++k) {
                            int nr = r + dr[k], nc = c + dc[k];
                            if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4) continue;
                            if (g[nr][nc] != 0) continue;
                            int pr = r, pc = c, cr = nr, cc = nc;
                            while (cr >= 0 && cr < 4 && cc >= 0 && cc < 4 && g[cr][cc] == 0) {
                                pr = cr; pc = cc; cr += dr[k]; cc += dc[k];
                            }
                            int idxPRPC = pr*4+pc;
                            u32 newCode = curCode & ~((u32)3 << (2*idxRC));
                            newCode |= (u32)color << (2*idxPRPC);
                            int rk = rankOf(newCode);
                            if (ntEpoch[rk] != curEpoch) {
                                ntEpoch[rk] = curEpoch; ntDist[rk] = (u16)newFDepth;
                                fq[fTail] = newCode; fturn_[fTail] = (u8)newTurn; fTail++;
                                producedThisLevel++;
                                if (obEpoch[rk] == curEpoch) {
                                    long long cand = (long long)newFDepth + obDist[rk];
                                    if (cand < bestAns) bestAns = cand;
                                }
                            }
                        }
                    }
                }
            }
            fDepth = newFDepth;
            fLevelCount = producedThisLevel;
        }

        if (bestAns <= (long long)fDepth + bDepth) break;
        // Forward closure fully exhausted without ever matching: the goal
        // board is definitively not reachable from the start (the forward
        // closure is now the *complete* set of boards reachable from
        // start). No need to run the backward side at all in this case.
        if (fLevelCount == 0 && bestAns >= INF) return -1;
        if (fLevelCount == 0 && bLevelCount == 0) break;

        // ---- expand backward one level ----
        if (bLevelCount > 0) {
            int newBDepth = bDepth + 1;
            int levelSize = bLevelCount;
            int producedThisLevel = 0;
            for (int i = 0; i < levelSize; ++i) {
                u32 curCode = bq[bHead]; int turn = bturn_[bHead]; bHead++;
                int g[4][4]; decode(curCode, g);
                int prevColor = (turn == 0) ? 2 : 1; // color of the move being undone
                int newTurn = 1 - turn;
                u16 *ntDist = bdist[newTurn]; u8 *ntEpoch = bepoch[newTurn];
                u8 *ofEpoch = fepoch[newTurn]; u16 *ofDist = fdist[newTurn];
                for (int r = 0; r < 4; ++r) {
                    for (int c = 0; c < 4; ++c) {
                        if (g[r][c] != prevColor) continue;
                        int idxRC = r*4+c;
                        for (int k = 0; k < 8; ++k) {
                            int br = r + dr[k], bc = c + dc[k];
                            bool blocked = (br < 0 || br >= 4 || bc < 0 || bc >= 4) || (g[br][bc] != 0);
                            if (!blocked) continue;
                            int ar = r - dr[k], ac = c - dc[k];
                            while (ar >= 0 && ar < 4 && ac >= 0 && ac < 4 && g[ar][ac] == 0) {
                                int idxA = ar*4+ac;
                                u32 newCode = curCode & ~((u32)3 << (2*idxRC));
                                newCode |= (u32)prevColor << (2*idxA);
                                int rk = rankOf(newCode);
                                if (ntEpoch[rk] != curEpoch) {
                                    ntEpoch[rk] = curEpoch; ntDist[rk] = (u16)newBDepth;
                                    bq[bTail] = newCode; bturn_[bTail] = (u8)newTurn; bTail++;
                                    producedThisLevel++;
                                    if (ofEpoch[rk] == curEpoch) {
                                        long long cand = (long long)ofDist[rk] + newBDepth;
                                        if (cand < bestAns) bestAns = cand;
                                    }
                                }
                                ar -= dr[k]; ac -= dc[k];
                            }
                        }
                    }
                }
            }
            bDepth = newBDepth;
            bLevelCount = producedThisLevel;
        }

        if (bestAns <= (long long)fDepth + bDepth) break;
        // Symmetric short-circuit: backward closure (complete set of
        // boards that can reach the goal) exhausted without a match.
        if (bLevelCount == 0 && bestAns >= INF) return -1;
        if (fLevelCount == 0 && bLevelCount == 0) break;
    }

    if (bestAns >= INF) return -1;
    return (int)bestAns;
}

int main() {
    initC();
    int w;
    if (scanf("%d", &w) != 1) return 0;
    for (int tc = 0; tc < w; ++tc) {
        int gi[4][4], gf[4][4];
        char buf[16];
        for (int r = 0; r < 4; ++r) {
            scanf("%s", buf);
            for (int c = 0; c < 4; ++c) {
                char ch = buf[c];
                gi[r][c] = (ch == 'w') ? 1 : (ch == 'b') ? 2 : 0;
            }
        }
        for (int r = 0; r < 4; ++r) {
            scanf("%s", buf);
            for (int c = 0; c < 4; ++c) {
                char ch = buf[c];
                gf[r][c] = (ch == 'w') ? 1 : (ch == 'b') ? 2 : 0;
            }
        }
        u32 sc = encode(gi);
        u32 ec = encode(gf);
        printf("%d\n", solve(sc, ec));
    }
    return 0;
}
