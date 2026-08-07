// POJ 1962 - Corporative Network
// Model: claude-opus-5
//
// Weighted (distance-to-parent) union-find.
//
// Each cluster is a rooted tree whose root is the cluster's serving center.
// par[x] = parent of x (0 = x is a root/center), w[x] = length of the path
// from x to par[x].  "I i j" is guaranteed to name a current center i, so it
// is a plain link: par[i] = j, w[i] = |i-j| % 1000; the new cluster's center
// is the center of j's old cluster, which is exactly the root reached through
// j.  "E i" is the accumulated weight from i up to its root.
//
// find() is iterative with path compression (the discuss board reports the
// recursive version TLEs, and 20000 nodes can form a 20000-deep chain anyway).
// After compression w[x] is the full distance from x to the root, which stays
// consistent with the "distance to parent" invariant because par[x] is then
// the root itself.
//
// Bounds: at most N-1 <= 19999 links, each of length <= 999, so every answer
// fits comfortably in a 32-bit int (< 2*10^7).  Note the sample uses N = 4
// even though the statement claims 5 <= N, so nothing may rely on N >= 5.

#include <cstdio>
#include <cstdlib>

const int MAXN = 20005;

static int par[MAXN];
static int w[MAXN];
static int stk[MAXN];

/* Returns the distance from x up to the root of its tree, compressing. */
static int findDist(int x) {
    int top = 0;
    int cur = x;
    while (par[cur] != 0) {
        stk[top++] = cur;
        cur = par[cur];
    }
    int root = cur;
    /* Walk the recorded path backwards, accumulating from the root down. */
    int acc = 0;
    for (int i = top - 1; i >= 0; --i) {
        int node = stk[i];
        acc += w[node];
        w[node] = acc;
        par[node] = root;
    }
    return acc;
}

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

/* Next non-whitespace character, or -1 at EOF. */
static int nextToken() {
    int c = gc();
    while (c == ' ' || c == '\t' || c == '\r' || c == '\n') c = gc();
    return c;
}

static int readInt() {
    int c = nextToken();
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10 + (c - '0');
        c = gc();
    }
    return v * sign;
}

int main() {
    int T = readInt();
    while (T-- > 0) {
        int n = readInt();
        for (int i = 0; i <= n; ++i) { par[i] = 0; w[i] = 0; }
        for (;;) {
            int c = nextToken();
            if (c < 0 || c == 'O' || c == 'o') break;
            if (c == 'E' || c == 'e') {
                int i = readInt();
                printf("%d\n", findDist(i));
            } else { /* 'I' */
                int i = readInt();
                int j = readInt();
                int d = i - j;
                if (d < 0) d = -d;
                par[i] = j;
                w[i] = d % 1000;
            }
        }
    }
    return 0;
}
