// POJ 2805 - Pegs
// Model: claude-sonnet-5
// Approach: represent each 5x5 board as a 25-bit peg mask, precompute the
// jump-move bitmasks valid for that board's layout, then DFS from the
// initial mask memoizing every visited mask in a direct-addressed array
// (2^25 slots, generation-stamped so it needs no per-case clearing). The
// answer is the min popcount over all visited masks, which equals the min
// over terminal masks since every jump strictly reduces the peg count.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

const int MASK_BITS = 25;
const int MASK_SIZE = 1 << MASK_BITS; // 33,554,432

static unsigned short gen[MASK_SIZE]; // generation stamp per mask (0 = never touched)
static int stackBuf[1 << 22]; // generous DFS stack (reachable-state counts observed are far smaller)

int idx(int r, int c) { return r * 5 + c; }

int popcount(int x) {
    int cnt = 0;
    while (x) { x &= x - 1; cnt++; }
    return cnt;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    memset(gen, 0, sizeof(gen));
    for (int tc = 1; tc <= n; tc++) {
        char grid[5][6];
        for (int r = 0; r < 5; r++) {
            scanf("%s", grid[r]);
        }
        int block = 0, peg = 0;
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                char ch = grid[r][c];
                int id = idx(r, c);
                if (ch == '#') block |= (1 << id);
                else if (ch == 'o') peg |= (1 << id);
            }
        }
        // Precompute valid move masks: for each move, (needMask = a|b must be
        // set, mustBeEmpty = c must be clear), and the resulting toggle mask.
        int moveNeed[64], moveClearBit[64], moveToggle[64];
        int nmoves = 0;
        // horizontal
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c + 2 < 5; c++) {
                int a = idx(r, c), b = idx(r, c + 1), cc = idx(r, c + 2);
                int m = (1 << a) | (1 << b) | (1 << cc);
                if (!(block & m)) {
                    int ba = 1 << a, bb = 1 << b, bc = 1 << cc;
                    moveNeed[nmoves] = ba | bb; moveClearBit[nmoves] = bc; moveToggle[nmoves] = ba | bb | bc; nmoves++;
                    moveNeed[nmoves] = bc | bb; moveClearBit[nmoves] = ba; moveToggle[nmoves] = ba | bb | bc; nmoves++;
                }
            }
        }
        // vertical
        for (int c = 0; c < 5; c++) {
            for (int r = 0; r + 2 < 5; r++) {
                int a = idx(r, c), b = idx(r + 1, c), cc = idx(r + 2, c);
                int m = (1 << a) | (1 << b) | (1 << cc);
                if (!(block & m)) {
                    int ba = 1 << a, bb = 1 << b, bc = 1 << cc;
                    moveNeed[nmoves] = ba | bb; moveClearBit[nmoves] = bc; moveToggle[nmoves] = ba | bb | bc; nmoves++;
                    moveNeed[nmoves] = bc | bb; moveClearBit[nmoves] = ba; moveToggle[nmoves] = ba | bb | bc; nmoves++;
                }
            }
        }

        int sp = 0;
        stackBuf[sp++] = peg;
        gen[peg] = (unsigned short)tc;
        int best = popcount(peg);
        while (sp > 0) {
            int mask = stackBuf[--sp];
            int pc = popcount(mask);
            if (pc < best) best = pc;
            for (int i = 0; i < nmoves; i++) {
                if ((mask & moveNeed[i]) == moveNeed[i] && !(mask & moveClearBit[i])) {
                    int nm = mask ^ moveToggle[i];
                    if (gen[nm] != (unsigned short)tc) {
                        gen[nm] = (unsigned short)tc;
                        stackBuf[sp++] = nm;
                    }
                }
            }
        }
        printf("The best case ends with %d pegs.\n", best);
    }
    return 0;
}
