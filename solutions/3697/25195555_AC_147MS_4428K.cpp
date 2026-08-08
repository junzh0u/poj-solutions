// POJ 3697 - USTC campus network
// Model: claude-opus-5
//
// The network is the COMPLETE graph on N vertices minus the M damaged links,
// so the graph to search is the complement of the input graph. Answer is the
// size of vertex 1's component, minus one.
//
// Complement BFS in O(N + M): keep the not-yet-visited vertices in a doubly
// linked list. For each dequeued u, stamp u's damaged neighbours, then walk the
// list once; every unstamped v is joined to u by a surviving link, so it is
// unlinked and enqueued. A list entry is either removed (charged to one of the
// N removals) or skipped because (u,v) is damaged (charged to one of the 2M
// damaged endpoints), so the total walk cost is linear.
//
// No statement ambiguity. Input is read to EOF over multiple test cases,
// terminated by a line of two zeros. M can be 1,000,000 per case, so input goes
// through a fread buffer; the damaged-edge adjacency lists are the only large
// allocation (2 * 2e6 ints = 16MB, inside the 64MB limit that the discuss board
// reports several people exceeding).

#include <cstdio>

static const int MAXN = 10005;
static const int MAXE = 2000005;

static int head[MAXN];
static int eto[MAXE];
static int enext[MAXE];

static int nxt[MAXN], prv[MAXN];
static int mark_[MAXN];
static int q[MAXN];

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

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int r = 0;
    while (c >= '0' && c <= '9') { r = r * 10 + (c - '0'); c = gc(); }
    out = r * sign;
    return true;
}

int main() {
    int n, m;
    int caseNo = 0;
    int stamp = 0;
    for (int i = 0; i < MAXN; i++) mark_[i] = 0;

    while (readInt(n)) {
        if (!readInt(m)) break;
        if (n == 0) break;

        for (int i = 1; i <= n; i++) head[i] = -1;
        int ec = 0;
        for (int i = 0; i < m; i++) {
            int a, b;
            readInt(a);
            readInt(b);
            if (a >= 1 && a <= n && b >= 1 && b <= n) {
                eto[ec] = b; enext[ec] = head[a]; head[a] = ec; ec++;
                eto[ec] = a; enext[ec] = head[b]; head[b] = ec; ec++;
            }
        }

        /* Doubly linked list of not-yet-visited vertices, sentinel 0 .. n+1. */
        for (int i = 0; i <= n + 1; i++) {
            nxt[i] = i + 1;
            prv[i] = i - 1;
        }
        nxt[n + 1] = n + 1;
        /* remove vertex 1 (the BBS server) */
        nxt[0] = 2 <= n ? 2 : n + 1;
        prv[nxt[0]] = 0;

        int qh = 0, qt = 0;
        q[qt++] = 1;
        int cnt = 0;

        while (qh < qt) {
            int u = q[qh++];
            ++stamp;
            for (int e = head[u]; e != -1; e = enext[e]) mark_[eto[e]] = stamp;
            int v = nxt[0];
            while (v <= n) {
                int w = nxt[v];
                if (mark_[v] != stamp) {
                    /* link u-v survives => v is reachable */
                    nxt[prv[v]] = nxt[v];
                    prv[nxt[v]] = prv[v];
                    q[qt++] = v;
                    cnt++;
                }
                v = w;
            }
        }

        printf("Case %d: %d\n", ++caseNo, cnt);
    }
    return 0;
}
