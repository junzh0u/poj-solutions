// POJ 1511 - Invitation Cards
//
// P stops (1 = CCS), Q directed weighted bus lines, several test cases.
// Need: sum over all stops i != CCS of (shortest CCS->i) + (shortest i->CCS).
// Build the forward graph and its reverse (edges flipped) as CSR-style
// adjacency lists, then run Dijkstra from CCS once on each: the forward run
// gives CCS->i distances directly, and Dijkstra from CCS on the reverse
// graph gives i->CCS distances (a path i->...->CCS in the original graph is
// exactly a path CCS->...->i in the reversed one). This turns what looks
// like up to P single-source problems into two.
//
// Scale (P, Q up to 1e6) rules out an adjacency matrix or per-node Dijkstra,
// so edges live in flat int arrays with linked-list style heads, and input
// is read through a manual buffered reader instead of cin/scanf. The total
// answer can exceed 2^31 (up to ~1e6 stops times distances up to ~1e9), so
// it's accumulated in a 64-bit int and emitted with a manual digit-printing
// routine rather than printf("%lld", ...), which is unreliable on POJ's
// older compiler/runtime combination.
//
// Nothing in the statement was ambiguous once "each round trip passes
// through the CCS" is read as: the graph is such that every stop can reach
// and be reached from stop 1, which the problem also states outright.
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

typedef long long LL;
const int MAXN = 1000005;
const int MAXM = 1000005;
const LL INF = (LL)1e18;

int headF[MAXN], toF[MAXM], nxtF[MAXM], wF[MAXM];
int headR[MAXN], toR[MAXM], nxtR[MAXM], wR[MAXM];
LL dist1[MAXN], dist2[MAXN];
bool vis[MAXN];

static char ibuf[1 << 20];
int ibufLen = 0, ibufPos = 0;
inline int gc() {
    if (ibufPos == ibufLen) {
        ibufLen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ibufPos = 0;
        if (ibufLen == 0) return -1;
    }
    return ibuf[ibufPos++];
}
inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    long x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = gc();
    }
    out = neg ? (int)(-x) : (int)x;
    return true;
}

inline void printLL(LL x) {
    if (x == 0) { putchar('0'); putchar('\n'); return; }
    char buf[24];
    int len = 0;
    while (x > 0) { buf[len++] = (char)('0' + (int)(x % 10)); x /= 10; }
    while (len--) putchar(buf[len]);
    putchar('\n');
}

void dijkstra(int *head, int *to, int *nxt, int *w, int n, int src, LL *dist) {
    for (int i = 1; i <= n; ++i) { dist[i] = INF; vis[i] = false; }
    dist[src] = 0;
    priority_queue<pair<LL,int>, vector<pair<LL,int> >, greater<pair<LL,int> > > pq;
    pq.push(make_pair((LL)0, src));
    while (!pq.empty()) {
        pair<LL,int> top = pq.top(); pq.pop();
        int u = top.second;
        LL d = top.first;
        if (vis[u]) continue;
        vis[u] = true;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            LL nd = d + w[e];
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push(make_pair(nd, v));
            }
        }
    }
}

int main() {
    int N;
    if (!readInt(N)) return 0;
    while (N--) {
        int P, Q;
        readInt(P); readInt(Q);
        for (int i = 1; i <= P; ++i) { headF[i] = -1; headR[i] = -1; }
        for (int i = 0; i < Q; ++i) {
            int u, v, w;
            readInt(u); readInt(v); readInt(w);
            toF[i] = v; wF[i] = w; nxtF[i] = headF[u]; headF[u] = i;
            toR[i] = u; wR[i] = w; nxtR[i] = headR[v]; headR[v] = i;
        }
        dijkstra(headF, toF, nxtF, wF, P, 1, dist1);
        dijkstra(headR, toR, nxtR, wR, P, 1, dist2);
        LL ans = 0;
        for (int i = 2; i <= P; ++i) ans += dist1[i] + dist2[i];
        printLL(ans);
    }
    return 0;
}
