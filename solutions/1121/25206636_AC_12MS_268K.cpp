// POJ 1121 - Algernon's Noxious Emissions
// Model: claude-sonnet-5
// Approach: each table i has a dumped-chemical bitmask D_i and a
// neutralized-chemical bitmask K_i (chemicals arriving from upstream that
// are destroyed at this table). Let arrived[i] be the union of the output
// bitmasks of every table with an edge into i. Then
//   output[i] = D_i | (arrived[i] & ~K_i)
// The pipe graph may contain cycles, so this is a least-fixed-point
// computation: bitmasks only grow (monotone union), bounded by 26 bits per
// node, so worklist propagation over the edges converges in O(26*N*E).
// Ambiguity: the statement never explicitly says dumped chemicals bypass
// this table's own neutralization list, but it does guarantee "the same
// chemical will never appear in both lists" for a given table, so the
// distinction never actually matters for computing D_i | (arrived & ~K_i)
// vs (D_i | arrived) & ~K_i restricted appropriately - verified against the
// sample (matches exactly) and against a second data set posted on the
// discuss board (message 76809).
#include <cstdio>
#include <cstring>
using namespace std;

int N;
int dump_[55], neutral_[55];
int outp[55], arrived[55];
int headArr[55], nxt[55 * 55], to[55 * 55], edgeCnt;
int qArr[55 * 30 + 10];
bool inQueue[55];

void addEdge(int u, int v) {
    to[edgeCnt] = v;
    nxt[edgeCnt] = headArr[u];
    headArr[u] = edgeCnt++;
}

int parseMask(const char *s) {
    int m = 0;
    if (s[0] == '.' && s[1] == '\0') return 0;
    for (int i = 0; s[i]; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') m |= (1 << (s[i] - 'A'));
    }
    return m;
}

int main() {
    if (scanf("%d", &N) != 1) return 0;
    memset(headArr, -1, sizeof(headArr));
    char a[40], b[40];
    for (int i = 1; i <= N; ++i) {
        scanf("%s %s", a, b);
        dump_[i] = parseMask(a);
        neutral_[i] = parseMask(b);
    }
    int u, v;
    while (scanf("%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break;
        addEdge(u, v);
    }

    int qh = 0, qt = 0;
    for (int i = 1; i <= N; ++i) {
        outp[i] = dump_[i];
        arrived[i] = 0;
        qArr[qt++] = i;
        inQueue[i] = true;
    }
    while (qh < qt) {
        int i = qArr[qh++];
        inQueue[i] = false;
        for (int e = headArr[i]; e != -1; e = nxt[e]) {
            int j = to[e];
            int newArrived = arrived[j] | outp[i];
            if (newArrived != arrived[j]) {
                arrived[j] = newArrived;
                int newOut = dump_[j] | (arrived[j] & ~neutral_[j]);
                if (newOut != outp[j]) {
                    outp[j] = newOut;
                }
                if (!inQueue[j]) {
                    inQueue[j] = true;
                    qArr[qt++] = j;
                }
            }
        }
    }

    for (int i = 1; i <= N; ++i) {
        printf(":");
        for (int c = 0; c < 26; ++c) {
            if (outp[i] & (1 << c)) putchar('A' + c);
        }
        printf(":\n");
    }
    return 0;
}
