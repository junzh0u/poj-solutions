// POJ 1383 - Labyrinth
// Model: claude-opus-5
//
// The free blocks of the labyrinth form a forest (the statement guarantees
// exactly one path between any two free blocks), so the answer is the largest
// tree diameter over the components, in edges.  Sample case 2's answer of 8 on
// a 9-block path settles that "measured in blocks" means edge count, not block
// count; an isolated block gives 0.
//
// Per component, the standard double sweep: BFS from any cell to a farthest
// cell u, then BFS from u; the farthest cell from u is a diameter endpoint.
// The BFS queue is already in nondecreasing-distance order, so the last node
// enqueued is a farthest one and no extra scan is needed.  Between the two
// sweeps the first sweep's distances are cleared using the queue itself, so
// the whole run is O(R*C) regardless of how many components there are.
//
// Two traps the discuss board flags:
//   * the data does not always honour the one-component reading -- the board's
//     case ".#. / .#. / #.." is a forest of two trees and the answer, 3, lives
//     in the second one.  So every component is swept, not just the first.
//   * recursion overflows the stack at 1000x1000; the traversal here is an
//     explicit queue, so the language choice does not matter.
// Input is read with a buffered byte reader ("Huge input, scanf is
// recommended"), and a row shorter than C -- trailing rocks trimmed by the
// data generator -- is padded with '#'.

#include <cstdio>

const int MAXN = 1000 * 1000;

static char g[MAXN + 16];
static int dst[MAXN];
static int q[MAXN];

static int C, R;

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int readInt(int *out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return 0;
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    *out = v;
    return 1;
}

/* reads a maximal run of '#' and '.' into p, storing at most cap of them;
   returns how many were stored */
static int readRow(char *p, int cap) {
    int c = gc();
    while (c != -1 && c != '#' && c != '.') c = gc();
    int n = 0;
    while (c == '#' || c == '.') {
        if (n < cap) p[n++] = (char)c;
        c = gc();
    }
    return n;
}

/* BFS over the component of src; queue order is by nondecreasing distance,
   so the last node dequeued is a farthest one. Returns component size. */
static int bfs(int src, int *farthest) {
    int head = 0, tail = 0;
    dst[src] = 0;
    q[tail++] = src;
    while (head < tail) {
        int cur = q[head++];
        int r = cur / C, c = cur - r * C;
        int d = dst[cur] + 1;
        int nx;
        if (r > 0)      { nx = cur - C; if (g[nx] == '.' && dst[nx] < 0) { dst[nx] = d; q[tail++] = nx; } }
        if (r < R - 1)  { nx = cur + C; if (g[nx] == '.' && dst[nx] < 0) { dst[nx] = d; q[tail++] = nx; } }
        if (c > 0)      { nx = cur - 1; if (g[nx] == '.' && dst[nx] < 0) { dst[nx] = d; q[tail++] = nx; } }
        if (c < C - 1)  { nx = cur + 1; if (g[nx] == '.' && dst[nx] < 0) { dst[nx] = d; q[tail++] = nx; } }
    }
    *farthest = q[tail - 1];
    return tail;
}

int main() {
    int T;
    if (!readInt(&T)) return 0;
    while (T-- > 0) {
        if (!readInt(&C)) break;
        if (!readInt(&R)) break;
        int total = R * C;
        for (int r = 0; r < R; ++r) {
            int n = readRow(g + (long)r * C, C);
            for (int c = n; c < C; ++c) g[(long)r * C + c] = '#';
        }
        for (int i = 0; i < total; ++i) dst[i] = -1;

        int best = 0;
        for (int i = 0; i < total; ++i) {
            if (g[i] != '.' || dst[i] >= 0) continue;
            int f;
            int n = bfs(i, &f);
            for (int k = 0; k < n; ++k) dst[q[k]] = -1;
            int f2;
            bfs(f, &f2);
            if (dst[f2] > best) best = dst[f2];
        }
        printf("Maximum rope length is %d.\n", best);
    }
    return 0;
}
