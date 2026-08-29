// POJ 4045 - Power Station
// Model: claude-sonnet-5
//
// The city's n communities form a tree (n-1 cables). Every cable has the
// SAME resistance R (a single constant given per test case, not per edge),
// so the resistance along the path from community i to the station equals
// R times the number of edges (hops) on that path. The energy loss for
// community i is I^2 * R_i, so the total loss for a station choice s is
//     I^2 * R * sum_i dist(i, s)
// where dist is the number of edges. Minimizing the total loss is exactly
// the classic "1-median of a tree" problem: minimize the sum of distances
// to all other nodes. Solved with the standard two-pass rerooting technique
// (root at 1, compute subtree sizes and the root's sum of depths, then
// reroot in O(1) per edge using size[child] and n - size[child]). All ties
// achieving the minimum sum are reported, since the statement does not
// bound how many communities may be optimal (verified against both sample
// cases: case 1 has a unique optimum, case 2 has two tied nodes).
//
// Iterative BFS is used throughout (not recursion) since n can be 50000 and
// the tree can degenerate into a path, which would risk a recursion-depth
// stack overflow.
//
// Board note (message 340402) says a %I64d vs long long confusion cost
// submissions under old MSVC-style C++; this archive's own accepted
// sources show POJ's judge accepts plain `long long`/`%lld` normally, so
// no contortion is needed here. The total loss can reach ~3.1e12
// (minSumDist ~6.25e8 for a path of 50000 nodes, times I^2<=100, R<=50),
// which needs 64-bit arithmetic regardless of language.

#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 50005;

int n;
long long I, R;
vector<int> adj[MAXN];
int bfsOrder[MAXN];
int parentArr[MAXN];
long long depthArr[MAXN];
long long sizeArr[MAXN];
long long sumDist[MAXN];
int ansBuf[MAXN];

static inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return 0;
        c = getchar();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sgn;
}

char outBuf[1 << 22];
int outPos = 0;

static inline void writeLL(long long v) {
    if (v == 0) { outBuf[outPos++] = '0'; return; }
    char tmp[24];
    int t = 0;
    if (v < 0) { outBuf[outPos++] = '-'; v = -v; }
    while (v > 0) { tmp[t++] = char('0' + v % 10); v /= 10; }
    while (t > 0) outBuf[outPos++] = tmp[--t];
}

int main() {
    int T = readInt();
    while (T--) {
        n = readInt();
        I = readInt();
        R = readInt();
        for (int i = 1; i <= n; i++) adj[i].clear();
        for (int i = 0; i < n - 1; i++) {
            int x = readInt(), y = readInt();
            adj[x].push_back(y);
            adj[y].push_back(x);
        }

        // BFS from node 1 to build order + parent + depth
        int qTail = 0;
        bfsOrder[qTail++] = 1;
        parentArr[1] = 0;
        depthArr[1] = 0;
        for (int idx = 0; idx < qTail; idx++) {
            int u = bfsOrder[idx];
            for (size_t k = 0; k < adj[u].size(); k++) {
                int v = adj[u][k];
                if (v != parentArr[u]) {
                    parentArr[v] = u;
                    depthArr[v] = depthArr[u] + 1;
                    bfsOrder[qTail++] = v;
                }
            }
        }

        // subtree sizes: process in reverse BFS order (children before parents)
        for (int i = 1; i <= n; i++) sizeArr[i] = 1;
        for (int idx = n - 1; idx >= 1; idx--) {
            int v = bfsOrder[idx];
            sizeArr[parentArr[v]] += sizeArr[v];
        }

        long long total = 0;
        for (int i = 1; i <= n; i++) total += depthArr[i];
        sumDist[1] = total;

        // reroot: process in BFS order (root first) so parent's sumDist is ready
        for (int idx = 1; idx < n; idx++) {
            int v = bfsOrder[idx];
            int p = parentArr[v];
            sumDist[v] = sumDist[p] - sizeArr[v] + (long long)(n - sizeArr[v]);
        }

        long long minVal = sumDist[1];
        for (int i = 2; i <= n; i++) if (sumDist[i] < minVal) minVal = sumDist[i];

        int cnt = 0;
        for (int i = 1; i <= n; i++) if (sumDist[i] == minVal) ansBuf[cnt++] = i;

        long long loss = minVal * I * I * R;
        writeLL(loss);
        outBuf[outPos++] = '\n';
        for (int i = 0; i < cnt; i++) {
            if (i) outBuf[outPos++] = ' ';
            writeLL(ansBuf[i]);
        }
        outBuf[outPos++] = '\n';
        outBuf[outPos++] = '\n';

        if (outPos > (1 << 22) - 1024) {
            fwrite(outBuf, 1, outPos, stdout);
            outPos = 0;
        }
    }
    fwrite(outBuf, 1, outPos, stdout);
    return 0;
}
