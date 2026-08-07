// POJ 2724 - Purifying Machine
// Model: claude-opus-5
//
// Each recorded operation infects one cheese (no '*') or two cheeses whose
// numbers differ in exactly one bit (one '*').  Collect the DISTINCT infected
// numbers -- duplicates across operations are common and counting them twice
// inflates the answer.  A legal cleaning operation is a single infected number,
// or a pair of infected numbers differing in one bit (a '*' over a bit whose
// both settings are infected); anything else would spoil a clean cheese.
//
// So the answer is a minimum edge cover of the graph on infected numbers with
// edges between numbers at Hamming distance 1, where isolated vertices cost one
// operation each.  By Gallai that is |V| - (maximum matching).  Two numbers at
// Hamming distance 1 have opposite popcount parity, so the graph is bipartite
// (even popcount on one side, odd on the other) and Kuhn's augmenting-path
// algorithm suffices: |V| <= 2^10 = 1024 and each vertex has at most N <= 10
// neighbours.
//
// Input: several test cases terminated by "0 0"; also tolerates EOF without it.
// The discuss board reports RE from scanf("%s") because the state lines may
// carry stray blanks, so the reader below takes exactly N characters from
// {0,1,*} and ignores every other byte (spaces, blank lines, CR).

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static int idxOf[1 << 10];
static vector<int> adjList[1 << 10];
static int mate[1 << 10];
static bool used[1 << 10];
static int values[1 << 10];

static bool augment(int u) {
    for (size_t i = 0; i < adjList[u].size(); ++i) {
        int v = adjList[u][i];
        if (used[v]) continue;
        used[v] = true;
        if (mate[v] < 0 || augment(mate[v])) {
            mate[v] = u;
            mate[u] = v;
            return true;
        }
    }
    return false;
}

/* Read exactly n characters drawn from {'0','1','*'}, ignoring any other
   byte (spaces, CR, LF).  The discuss board reports RE for scanf("%s"),
   claiming stray blanks inside the state lines. */
static bool readState(int n, char *buf) {
    int got = 0;
    while (got < n) {
        int c = getchar();
        if (c == EOF) return false;
        if (c == '0' || c == '1' || c == '*') buf[got++] = (char)c;
    }
    buf[n] = '\0';
    return true;
}

int main() {
    int n, m;
    char buf[64];
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        int full = 1 << n;
        for (int i = 0; i < full; ++i) idxOf[i] = -1;
        int k = 0;
        for (int op = 0; op < m; ++op) {
            if (!readState(n, buf)) return 0;
            int base = 0, star = -1;
            for (int i = 0; i < n; ++i) {
                if (buf[i] == '*') star = n - 1 - i;
                else if (buf[i] == '1') base |= 1 << (n - 1 - i);
            }
            int reps = (star < 0) ? 1 : 2;
            for (int r = 0; r < reps; ++r) {
                int v = base;
                if (star >= 0 && r == 1) v |= 1 << star;
                if (idxOf[v] < 0) {
                    idxOf[v] = k;
                    values[k] = v;
                    ++k;
                }
            }
        }
        for (int i = 0; i < k; ++i) {
            adjList[i].clear();
            mate[i] = -1;
        }
        /* Numbers differing in one bit have opposite popcount parity, so the
           "differ in exactly one bit" graph is bipartite; split on parity. */
        for (int i = 0; i < k; ++i) {
            int v = values[i], p = 0;
            for (int b = 0; b < n; ++b) p ^= (v >> b) & 1;
            if (p) continue; /* only expand from the even side */
            for (int b = 0; b < n; ++b) {
                int w = v ^ (1 << b);
                if (idxOf[w] >= 0) adjList[i].push_back(idxOf[w]);
            }
        }
        int matching = 0;
        for (int i = 0; i < k; ++i) {
            if (adjList[i].empty()) continue;
            for (int j = 0; j < k; ++j) used[j] = false;
            if (augment(i)) ++matching;
        }
        printf("%d\n", k - matching);
    }
    return 0;
}
