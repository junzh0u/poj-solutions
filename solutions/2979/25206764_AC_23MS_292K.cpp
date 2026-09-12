// POJ 2979 - RSI
// Model: claude-sonnet-5
//
// Keypad layout (row,col), 11 valid physical cells (row4,col3 does not exist):
//   7 8 9        (1,1)(1,2)(1,3)
//   4 5 6        (2,1)(2,2)(2,3)
//   1 2 3        (3,1)(3,2)(3,3)
//   0 0           (4,1)(4,2)      <- both cells are the SAME "0" key
//
// Cell indices 0..10:
//   0:(1,1)7 1:(1,2)8 2:(1,3)9
//   3:(2,1)4 4:(2,2)5 5:(2,3)6
//   6:(3,1)1 7:(3,2)2 8:(3,3)3
//   9:(4,1)0 10:(4,2)0
//
// During each time unit each finger independently either: stays put, moves to
// an orthogonally-adjacent valid cell, or presses the key it is sitting on
// (only if that key is the next digit still needed, and at most one finger
// may press per time unit). The left finger's column must stay strictly less
// than the right finger's column after every time unit. This is exactly a
// shortest-path problem over the state (digits typed so far, leftCell,
// rightCell) with every edge of weight 1, so a plain BFS from the start state
// gives the minimum number of time units to finish typing the whole string
// (the discuss board reports people getting silent 1-off errors from ad hoc
// DP that special-cased the "0" key incorrectly; BFS over the full state
// avoids that entirely by construction).
#include <cstdio>
#include <cstring>
#include <string>
#include <queue>
using namespace std;

static const int NEI[11][4] = {
    {1,3,-1,-1},
    {0,2,4,-1},
    {1,5,-1,-1},
    {0,4,6,-1},
    {1,3,5,7},
    {2,4,8,-1},
    {3,7,9,-1},
    {4,6,8,10},
    {5,7,-1,-1},
    {6,10,-1,-1},
    {7,9,-1,-1}
};
static const int NEICNT[11] = {2,3,2,3,4,3,3,4,2,2,2};
static const int COL[11] = {1,2,3,1,2,3,1,2,3,1,2};

// digit -> cell(s) that key occupies
static const int DPOS[10][2] = {
    {9,10}, // '0'
    {6,-1}, // '1'
    {7,-1}, // '2'
    {8,-1}, // '3'
    {3,-1}, // '4'
    {4,-1}, // '5'
    {5,-1}, // '6'
    {0,-1}, // '7'
    {1,-1}, // '8'
    {2,-1}  // '9'
};
static const int DCNT[10] = {2,1,1,1,1,1,1,1,1,1};

static bool isDigitCell(int digit, int cell) {
    for (int k = 0; k < DCNT[digit]; ++k)
        if (DPOS[digit][k] == cell) return true;
    return false;
}

static int dist_[102][11][11];

struct State { int i, l, r; };

static int solve(const string &s) {
    int n = (int)s.size();
    memset(dist_, -1, sizeof(dist_));
    queue<State> q;
    dist_[0][3][4] = 0; // left on '4' (cell3), right on '5' (cell4)
    State st0; st0.i = 0; st0.l = 3; st0.r = 4;
    q.push(st0);

    while (!q.empty()) {
        State cur = q.front(); q.pop();
        int i = cur.i, l = cur.l, r = cur.r;
        int d = dist_[i][l][r];
        if (i == n) {
            return d; // BFS visits states in non-decreasing distance order
        }
        int digit = s[i] - '0';

        // Build option lists: "stay" plus each neighbor.
        int lOpts[5], lCnt = 0;
        lOpts[lCnt++] = l;
        for (int k = 0; k < NEICNT[l]; ++k) lOpts[lCnt++] = NEI[l][k];
        int rOpts[5], rCnt = 0;
        rOpts[rCnt++] = r;
        for (int k = 0; k < NEICNT[r]; ++k) rOpts[rCnt++] = NEI[r][k];

        // 1) Pure move step (no press), i unchanged.
        for (int a = 0; a < lCnt; ++a) {
            int nl = lOpts[a];
            for (int b = 0; b < rCnt; ++b) {
                int nr = rOpts[b];
                if (nl == l && nr == r) continue; // no-op, never useful
                if (COL[nl] >= COL[nr]) continue;
                if (dist_[i][nl][nr] == -1) {
                    dist_[i][nl][nr] = d + 1;
                    State ns; ns.i = i; ns.l = nl; ns.r = nr;
                    q.push(ns);
                }
            }
        }

        // 2) Left finger presses the needed digit; right finger stays/moves.
        if (isDigitCell(digit, l)) {
            for (int b = 0; b < rCnt; ++b) {
                int nr = rOpts[b];
                if (COL[l] >= COL[nr]) continue;
                if (dist_[i + 1][l][nr] == -1) {
                    dist_[i + 1][l][nr] = d + 1;
                    State ns; ns.i = i + 1; ns.l = l; ns.r = nr;
                    q.push(ns);
                }
            }
        }
        // 3) Right finger presses the needed digit; left finger stays/moves.
        if (isDigitCell(digit, r)) {
            for (int a = 0; a < lCnt; ++a) {
                int nl = lOpts[a];
                if (COL[nl] >= COL[r]) continue;
                if (dist_[i + 1][nl][r] == -1) {
                    dist_[i + 1][nl][r] = d + 1;
                    State ns; ns.i = i + 1; ns.l = nl; ns.r = r;
                    q.push(ns);
                }
            }
        }
    }
    return -1; // unreachable for valid input
}

int main() {
    char buf[256];
    while (scanf("%s", buf) == 1) {
        if (strcmp(buf, "eof") == 0) break;
        string s(buf);
        printf("%d\n", solve(s));
    }
    return 0;
}
