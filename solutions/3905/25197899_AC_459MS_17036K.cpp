// POJ 3905 - Perfect Election
// Model: claude-sonnet-5
// Approach: 2-SAT. Variable i in [1,N]: x_i true means "i is elected".
// A poll answer "+i +j" / "-i -j" / "+i -j" / "-i +j" is read directly as a
// clause (literal(i) OR literal(j)) where a '+' token is x_i and a '-' token
// is NOT x_i (matches the accepted-answer table verbatim). For each clause
// (l1 OR l2) we add the two standard implications NOT(l1)->l2, NOT(l2)->l1
// into an implication graph over 2N nodes (node 2i = x_i true, node 2i+1 =
// x_i false), then find SCCs with a single iterative-safe recursive Tarjan
// (the discuss board reports Kosaraju is too slow for the 5s limit; node
// count is only <=2N<=2000 so recursion depth is never a concern even
// though M, and hence edge count, is up to 1e6 -> 2e6 directed edges).
// A perfect election exists for a data set iff, for every i, x_i-true and
// x_i-false land in different SCCs. Multiple data sets run to EOF; all
// per-dataset arrays are sized once for the maximum N/M and only the used
// prefix is reset between data sets, and graph edges are read directly
// (i and j need not differ, matching the statement's "it may happen that
// i=j"). No ambiguity found in the statement or the board beyond the time
// limit / SCC-algorithm-choice folklore, which the sample and this design
// already account for.
#include <cstdio>
#include <cctype>
using namespace std;

const int MAXN = 1005;
const int MAXV = 2 * MAXN + 5;
const int MAXE = 2000005; // 2 * M, M <= 1e6

int head[MAXV], nxt[MAXE], eto[MAXE], ecnt;
int dfn[MAXV], low[MAXV], sccId[MAXV], idxCnt, sccCnt;
int stkArr[MAXV], sp;
bool onStack[MAXV];
int N, M;

inline void addEdge(int u, int v) {
    eto[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

// ---- fast IO ----
static char buf[1 << 20];
static int bufLen = 0, bufPos = 0;
inline int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen == 0) return -1;
    }
    return buf[bufPos++];
}
inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && c != '+' && c != '-' && !isdigit(c)) c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '+') {
        c = gc();
    } else if (c == '-') {
        sign = -1;
        c = gc();
    }
    long val = 0;
    while (c != -1 && isdigit(c)) {
        val = val * 10 + (c - '0');
        c = gc();
    }
    out = (int)(sign * val);
    return true;
}

inline int trueNode(int i) { return 2 * i; }
inline int falseNode(int i) { return 2 * i + 1; }
inline int litNode(int s) { return s > 0 ? trueNode(s) : falseNode(-s); }
inline int negNode(int s) { return s > 0 ? falseNode(s) : trueNode(-s); }

void tarjan(int u) {
    dfn[u] = low[u] = ++idxCnt;
    stkArr[++sp] = u;
    onStack[u] = true;
    for (int e = head[u]; e != -1; e = nxt[e]) {
        int v = eto[e];
        if (!dfn[v]) {
            tarjan(v);
            if (low[v] < low[u]) low[u] = low[v];
        } else if (onStack[v]) {
            if (dfn[v] < low[u]) low[u] = dfn[v];
        }
    }
    if (low[u] == dfn[u]) {
        sccCnt++;
        for (;;) {
            int x = stkArr[sp--];
            onStack[x] = false;
            sccId[x] = sccCnt;
            if (x == u) break;
        }
    }
}

int main() {
    while (readInt(N)) {
        if (!readInt(M)) break;
        int maxNode = 2 * N + 1;
        for (int v = 0; v <= maxNode; v++) {
            head[v] = -1;
            dfn[v] = 0;
            low[v] = 0;
            onStack[v] = false;
            sccId[v] = 0;
        }
        ecnt = 0;
        idxCnt = 0;
        sccCnt = 0;
        sp = 0;

        for (int k = 0; k < M; k++) {
            int a, b;
            readInt(a);
            readInt(b);
            int l1 = litNode(a), l2 = litNode(b);
            int n1 = negNode(a), n2 = negNode(b);
            addEdge(n1, l2);
            addEdge(n2, l1);
        }

        for (int v = 2; v <= maxNode; v++) {
            if (!dfn[v]) tarjan(v);
        }

        bool ok = true;
        for (int i = 1; i <= N; i++) {
            if (sccId[trueNode(i)] == sccId[falseNode(i)]) {
                ok = false;
                break;
            }
        }
        printf(ok ? "1\n" : "0\n");
    }
    return 0;
}
