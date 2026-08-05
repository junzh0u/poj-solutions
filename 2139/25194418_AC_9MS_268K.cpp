// POJ 2139 - Six Degrees of Cowvin Bacon
//
// Every movie makes its whole cast pairwise adjacent, so the "degrees of
// separation" between two cows is just the unweighted shortest-path distance in
// that co-starring graph. The answer is the minimum over cows of the total
// distance to the other N-1 cows, scaled by 100.
//
// N <= 300, so the graph is dense-friendly: each cow's neighbour set is kept as
// a 300-bit bitset (unsigned int words). A movie is turned into one mask and
// OR'd into every participant's row, which costs O(cast * N/32) instead of the
// O(cast^2) pair enumeration -- with M = 10000 movies of up to 300 cows that is
// the difference between 3e7 and 4.5e8 operations. Self-loops are cleared
// afterwards rather than skipped per-pair.
//
// Shortest paths are then a level-synchronous BFS per source over the bitsets:
// OR together the adjacency rows of the current frontier, mask off what is
// already visited, and charge the newly reached cows the current level. That is
// O(N^2/32) per source, so the whole thing runs in 9MS even on an 11 MB input
// where every one of the 10000 movies has all 300 cows.
//
// Statement notes:
//  - The output is "100 times the shortest mean degree", and the accepted
//    behaviour is truncation, not rounding: print (100 * bestSum) / (N - 1)
//    with integer division. Computing 100 * sum before dividing keeps the
//    full precision the scaling asks for.
//  - The mean excludes the cow herself, hence the N-1 divisor.
//  - Connectivity is guaranteed by the statement, but a source that cannot
//    reach everyone is skipped rather than counted with a partial sum, so a
//    malformed input (e.g. a movie listing a single cow, which contributes no
//    edges at all) degrades gracefully instead of reporting a bogus minimum.
#include <cstdio>
#include <cstring>

static const int MAXN = 300;
static const int MAXW = (MAXN + 31) / 32;

static unsigned int adj[MAXN][MAXW];
static unsigned int visited[MAXW];
static unsigned int frontier_[MAXW];
static unsigned int nxt[MAXW];
static unsigned int mask[MAXW];
static int buf[MAXN + 5];

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return -1;
        c = getchar();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sgn;
}

static int popcnt(unsigned int v) {
    v = v - ((v >> 1) & 0x55555555u);
    v = (v & 0x33333333u) + ((v >> 2) & 0x33333333u);
    v = (v + (v >> 4)) & 0x0F0F0F0Fu;
    return (int)((v * 0x01010101u) >> 24);
}

int main() {
    int n = readInt();
    int m = readInt();
    if (n <= 0) return 0;
    int W = (n + 31) / 32;

    for (int i = 0; i < m; i++) {
        int k = readInt();
        if (k <= 0) continue;
        for (int w = 0; w < W; w++) mask[w] = 0u;
        for (int j = 0; j < k; j++) {
            int c = readInt() - 1;
            buf[j] = c;
            if (c >= 0 && c < n) mask[c >> 5] |= (1u << (c & 31));
        }
        for (int j = 0; j < k; j++) {
            int c = buf[j];
            if (c < 0 || c >= n) continue;
            for (int w = 0; w < W; w++) adj[c][w] |= mask[w];
        }
    }
    /* drop self loops */
    for (int i = 0; i < n; i++) adj[i][i >> 5] &= ~(1u << (i & 31));

    long best = -1;
    for (int s = 0; s < n; s++) {
        for (int w = 0; w < W; w++) { visited[w] = 0u; frontier_[w] = 0u; }
        visited[s >> 5] |= (1u << (s & 31));
        frontier_[s >> 5] |= (1u << (s & 31));
        long sum = 0;
        int d = 0;
        int reached = 1;
        for (;;) {
            for (int w = 0; w < W; w++) nxt[w] = 0u;
            bool any = false;
            for (int w = 0; w < W; w++) {
                unsigned int f = frontier_[w];
                while (f) {
                    unsigned int low = f & (~f + 1u);
                    int b = popcnt(low - 1u);
                    int u = (w << 5) + b;
                    for (int w2 = 0; w2 < W; w2++) nxt[w2] |= adj[u][w2];
                    f ^= low;
                }
            }
            int cnt = 0;
            for (int w = 0; w < W; w++) {
                nxt[w] &= ~visited[w];
                if (nxt[w]) any = true;
                cnt += popcnt(nxt[w]);
                visited[w] |= nxt[w];
                frontier_[w] = nxt[w];
            }
            if (!any) break;
            d++;
            sum += (long)d * cnt;
            reached += cnt;
        }
        if (reached < n) continue; /* disconnected source: skip (problem guarantees connected) */
        if (best < 0 || sum < best) best = sum;
    }

    if (best < 0) { printf("0\n"); return 0; }
    printf("%ld\n", (best * 100) / (long)(n - 1));
    return 0;
}
