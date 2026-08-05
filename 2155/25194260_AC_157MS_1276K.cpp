// POJ 2155 - Matrix
//
// An N*N 0/1 matrix (N <= 1000) starts all zero. Two operations, up to T = 50000
// of them per test case, up to 10 test cases: "C x1 y1 x2 y2" flips every cell of
// a rectangle, "Q x y" asks for a single cell.
//
// Approach: 2D Fenwick tree over GF(2). Because a flip is its own inverse, the
// value of a cell is just the parity of how many flips covered it, so the usual
// difference-array trick for range-update/point-query carries over with XOR in
// place of addition: a rectangle flip toggles the four corners
//   (x1,y1), (x1,y2+1), (x2+1,y1), (x2+1,y2+1)
// of a difference grid, and A[x][y] is the XOR of that grid over the prefix
// [1..x] x [1..y]. The Fenwick tree maintains those prefix XORs in O(log^2 N)
// per update and per query; corner indices of n+1 simply fall outside the tree
// and are dropped by the loop bound, which is exactly the wanted behaviour.
//
// The nodes only ever hold one bit, so the tree is a 1005x1005 array of unsigned
// char (~1 MB, well under the 65536K limit) and each node update is a byte XOR.
// Worst-case input (10 cases x 50000 rectangle flips at N = 1000) runs in a small
// fraction of the 3000MS limit.
//
// Statement note: "There is a blank line between every two continuous test cases"
// means a separator, not a trailing blank line, so the newline is emitted before
// every test case except the first. Note also that the query output is per query,
// not per test case, and a test case may contain no queries at all — the blank
// line still separates it from its neighbours.

#include <cstdio>
#include <cstring>

static const int MAXN = 1005;
static unsigned char bit[MAXN][MAXN];
static int n;

static void upd(int x, int y) {
    for (int i = x; i <= n; i += i & (-i))
        for (int j = y; j <= n; j += j & (-j))
            bit[i][j] ^= 1;
}

static int qry(int x, int y) {
    int s = 0;
    for (int i = x; i > 0; i -= i & (-i))
        for (int j = y; j > 0; j -= j & (-j))
            s ^= bit[i][j];
    return s;
}

int main() {
    int X;
    if (scanf("%d", &X) != 1) return 0;
    for (int tc = 0; tc < X; ++tc) {
        if (tc) putchar('\n');
        int t;
        scanf("%d %d", &n, &t);
        memset(bit, 0, sizeof(bit));
        char op[8];
        for (int k = 0; k < t; ++k) {
            scanf("%s", op);
            if (op[0] == 'C') {
                int x1, y1, x2, y2;
                scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
                upd(x1, y1);
                upd(x1, y2 + 1);
                upd(x2 + 1, y1);
                upd(x2 + 1, y2 + 1);
            } else {
                int x, y;
                scanf("%d %d", &x, &y);
                putchar('0' + qry(x, y));
                putchar('\n');
            }
        }
    }
    return 0;
}
