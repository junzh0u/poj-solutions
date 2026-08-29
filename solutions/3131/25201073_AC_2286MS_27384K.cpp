// POJ 3131 - Cubic Eight-Puzzle
// Model: claude-sonnet-5
//
// Each cube has 3 pairs of opposite faces (White/Red/Blue), so its visible
// state is fully described by (U,N) = (top color, north color); south=north
// color, east/west = the third color (since opposite faces share color).
// There are only 6 such states.  Figure 1 + Figure 3 (a worked roll example)
// give the transition when a cube rolls in the direction that changes its
// y-coordinate (north/south): (U,N) -> (N,U).  Rolling in the direction that
// changes its x-coordinate (east/west): (U,N) -> (third(U,N), N) [new top
// becomes the old east/west color, north unchanged].  Direction sign (north
// vs south, east vs west) does not matter for the resulting color state,
// only the axis does, because opposite faces are always equal in color.
// Figure 2 gives the initial orientation for every cube: top=White,
// south(=north)=Red, east(=west)=Blue.
//
// Since all 8 cubes are physically identical and indistinguishable, the
// whole board state is just: which of the 9 cells is empty, plus for each
// of the other 8 cells, one of the 6 orientation ids.  That is a state
// space of 9*6^8 = 15,116,544 states, small enough for plain BFS from the
// (fixed, once the empty cell is known) initial configuration to the first
// state whose top-color pattern (and empty position) matches the target.
// A parity argument (both transitions are transpositions of the 3-cycle
// (U,N,E), so every move flips the parity of the moving cube's permutation,
// while every move also flips the checkerboard color of the blank) shows
// only at most half of the 15.1M states are reachable from a given start,
// which bounds the BFS queue to N/2 entries and keeps memory well under the
// 64MB limit.
//
// Statement ambiguity: none found beyond the figures (the coloring axes and
// rolling direction, both of which only the images specify).  Sample I/O
// verified locally, including the depth-30 and both -1 cases.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static int VT[6] = {2,4,0,5,1,3};   // vertical (N-S axis) roll transition
static int HT[6] = {5,3,4,1,2,0};   // horizontal (E-W axis) roll transition
static int UCOL[6] = {0,0,1,1,2,2}; // top color of each orientation id
// orientation ids: 0:(W,R) 1:(W,B) 2:(R,W) 3:(R,B) 4:(B,W) 5:(B,R)
// colors: 0=White,1=Red,2=Blue

static int pow6[9];
static int order_[9][8]; // for each blank index, the other 8 indices ascending

static unsigned char dist_[9*1679616]; // 9*6^8 = 15,116,544

int encode(const int board[9], int blankIdx) {
    int code = 0;
    const int *ord = order_[blankIdx];
    for (int i = 0; i < 8; i++) code = code*6 + board[ord[i]];
    return blankIdx * pow6[8] + code;
}

void decode(int id, int board[9], int &blankIdx) {
    blankIdx = id / pow6[8];
    int code = id % pow6[8];
    const int *ord = order_[blankIdx];
    for (int i = 7; i >= 0; i--) {
        board[ord[i]] = code % 6;
        code /= 6;
    }
}

int main() {
    pow6[0] = 1;
    for (int i = 1; i < 9; i++) pow6[i] = pow6[i-1]*6;
    for (int b = 0; b < 9; b++) {
        int k = 0;
        for (int i = 0; i < 9; i++) if (i != b) order_[b][k++] = i;
    }

    static unsigned int qbuf[7558272 + 16];

    int x, y;
    while (scanf("%d %d", &x, &y) == 2) {
        if (x == 0 && y == 0) break;
        int emptyIdx = (y-1)*3 + (x-1);
        int target[9]; // -1 = empty, else 0/1/2 color
        int targetEmptyIdx = -1;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                char buf[8];
                scanf("%s", buf);
                int idx = j*3+i;
                if (buf[0] == 'E') { target[idx] = -1; targetEmptyIdx = idx; }
                else if (buf[0] == 'W') target[idx] = 0;
                else if (buf[0] == 'R') target[idx] = 1;
                else target[idx] = 2; // 'B'
            }
        }

        int board[9];
        for (int i = 0; i < 9; i++) board[i] = 0; // unused for blank cell but harmless
        int startId = encode(board, emptyIdx);

        memset(dist_, 0xFF, sizeof(dist_));
        int qh = 0, qt = 0;
        dist_[startId] = 0;
        qbuf[qt++] = (unsigned int)startId;

        int answer = -1;
        // check start itself
        {
            int bi = emptyIdx;
            bool ok = (bi == targetEmptyIdx);
            if (ok) {
                for (int i = 0; i < 9 && ok; i++) {
                    if (i == bi) continue;
                    if (target[i] != UCOL[board[i]]) ok = false;
                }
            }
            if (ok) answer = 0;
        }

        int curBoard[9];
        while (answer == -1 && qh < qt) {
            int id = (int)qbuf[qh++];
            int d = dist_[id];
            if (d >= 30) continue; // no point expanding beyond cap
            int bi;
            decode(id, curBoard, bi);
            int bx = bi % 3, by = bi / 3;
            static const int dx4[4] = {-1,1,0,0};
            static const int dy4[4] = {0,0,-1,1};
            for (int dir = 0; dir < 4; dir++) {
                int nx = bx + dx4[dir], ny = by + dy4[dir];
                if (nx < 0 || nx > 2 || ny < 0 || ny > 2) continue;
                int ni = ny*3+nx;
                int o = curBoard[ni];
                int no = (dx4[dir] != 0) ? HT[o] : VT[o];
                // build new board: same as curBoard but ni becomes blank,
                // and bi becomes value 'no'
                int savedBi = curBoard[bi];
                int savedNi = curBoard[ni];
                curBoard[bi] = no;
                // curBoard[ni] content doesn't matter (blank), encode skips it
                int newId = encode(curBoard, ni);
                curBoard[bi] = savedBi;
                curBoard[ni] = savedNi;

                if (dist_[newId] == 0xFF) {
                    dist_[newId] = (unsigned char)(d+1);
                    // check match
                    if (ni == targetEmptyIdx) {
                        bool ok = true;
                        for (int i = 0; i < 9 && ok; i++) {
                            if (i == ni) continue;
                            int val = (i == bi) ? no : curBoard[i];
                            if (target[i] != UCOL[val]) ok = false;
                        }
                        if (ok) { answer = d+1; break; }
                    }
                    qbuf[qt++] = (unsigned int)newId;
                }
            }
        }

        if (answer == -1 || answer > 30) printf("%d\n", -1);
        else printf("%d\n", answer);
    }
    return 0;
}
