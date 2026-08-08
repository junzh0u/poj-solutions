// POJ 3398 - Perfect Service
// Model: claude-opus-5
//
// Tree DP.  A set S of servers is a perfect service iff every vertex outside S
// has exactly one neighbour in S.  Root the tree at 1 and give each vertex u
// three states, distinguished by how u gets served:
//
//   dp0[u]  u is a server.  Then u serves every client neighbour it has, so a
//           child is either a server itself (dp0) or a client served by u and
//           by nobody else (dp1).  dp0[u] = 1 + sum min(dp0[c], dp1[c]).
//   dp1[u]  u is a client and its parent is a server, so u is already served.
//           "Exactly one" then forbids any server child, and each child, whose
//           parent u is not a server, must be served from below:
//           dp1[u] = sum dp2[c].
//   dp2[u]  u is a client and its parent is not a server, so exactly one child
//           must be a server and the others must be served from below:
//           dp2[u] = min over c of (dp0[c] + sum_{c' != c} dp2[c']).
//
// The root has no parent, so the answer is min(dp0[1], dp2[1]).  A leaf has
// dp2 = INF, which is the sentinel for "impossible"; INF exceeds any real
// answer, which is at most N, so it never competes with a feasible value.
//
// Note this is NOT the minimum dominating set (POJ 3659): "exactly one" is
// strictly stronger than "at least one".  On the discuss board's tree
// 1-4, 2-4, 3-4, 4-5, 5-6, 6-7, 6-8, 6-9 the dominating set {4,6} has size 2
// but leaves 5 adjacent to two servers; the perfect service number is 3.
//
// Input framing: a 0 terminates a test case and -1 terminates the input, but
// the statement's own sample ends the last case with -1 and no preceding 0.
// The reader therefore treats 0 as a skippable separator and stops at any
// negative token, which accepts both shapes.
//
// The traversal is an explicit queue rather than recursion: N is up to 10000
// and a path input would otherwise recurse 10000 deep.

#include <cstdio>
#include <algorithm>

static const int MAXN = 10005;
static const int INF = 20001; /* > any real answer, which is at most N */

static int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], ecnt;
static int parent_[MAXN], order_[MAXN];
static int dp0[MAXN], dp1[MAXN], dp2[MAXN];
static int ea[MAXN], eb[MAXN];

static char buf[1 << 16];
static int blen = 0, bpos = 0;

static int gc() {
    if (bpos == blen) {
        blen = (int)fread(buf, 1, sizeof(buf), stdin);
        bpos = 0;
        if (blen <= 0) return -1;
    }
    return (unsigned char)buf[bpos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return true;
}

static void addEdge(int a, int b) {
    to[ecnt] = b; nxt[ecnt] = head[a]; head[a] = ecnt++;
}

int main() {
    int n;
    while (readInt(n)) {
        if (n < 0) break;     /* -1 ends the whole input */
        if (n == 0) continue; /* 0 separates two test cases */
        if (n >= MAXN) n = MAXN - 1;
        int m = n;
        int i;
        for (i = 0; i < n - 1; i++) {
            int a = 0, b = 0;
            readInt(a); readInt(b);
            /* an endpoint outside 1..MAXN-1 never occurs in valid input; drop
               the edge rather than index out of the arrays */
            if (a < 1 || a >= MAXN || b < 1 || b >= MAXN || a == b) a = b = 0;
            ea[i] = a; eb[i] = b;
            if (a > m) m = a;
            if (b > m) m = b;
        }
        for (i = 1; i <= m; i++) head[i] = -1;
        ecnt = 0;
        for (i = 0; i < n - 1; i++) {
            if (ea[i] == 0) continue;
            addEdge(ea[i], eb[i]);
            addEdge(eb[i], ea[i]);
        }
        /* breadth-first order from the root: parents precede their children */
        int cnt = 0;
        parent_[1] = 0;
        order_[cnt++] = 1;
        for (i = 0; i < cnt; i++) {
            int u = order_[i];
            for (int e = head[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (v == parent_[u]) continue;
                parent_[v] = u;
                order_[cnt++] = v;
            }
        }
        for (i = cnt - 1; i >= 0; i--) {
            int u = order_[i];
            int s0 = 1;      /* u is a server: it pays for itself */
            int sum2 = 0;    /* sum of dp2 over the children */
            int best2 = INF; /* one child is a server, the rest are dp2 */
            int e;
            for (e = head[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (v == parent_[u]) continue;
                s0 += std::min(dp0[v], dp1[v]);
                sum2 += dp2[v];
            }
            for (e = head[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (v == parent_[u]) continue;
                int cand = sum2 - dp2[v] + dp0[v];
                if (cand < best2) best2 = cand;
            }
            dp0[u] = std::min(s0, INF);
            dp1[u] = std::min(sum2, INF);
            dp2[u] = std::min(best2, INF);
        }
        printf("%d\n", std::min(dp0[1], dp2[1]));
    }
    return 0;
}
