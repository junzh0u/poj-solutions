// POJ 3764 - The xor-longest Path
// Model: claude-opus-5
//
// Root the tree anywhere and let d[v] be the xor of the edge weights on the
// root->v path.  The xor-length of the path u..v is d[u] ^ d[v] (the part above
// their LCA cancels), so the answer is the maximum xor over all pairs of d
// values -- and the pair u == v gives 0, which is the right answer for n == 1.
//
// Maximum-xor-pair is done bitwise instead of with a binary trie: keep the best
// prefix `ans`, and for bit b (30..0, since w < 2^31) test whether some pair of
// masked values x, y satisfies x ^ y == ans | (1 << b) by putting every
// d[i] & mask into a hash set and looking up x ^ cand.  O(31 n) time in O(n)
// memory, which avoids the 31*n trie nodes -- worth it because the discuss
// board reports the real data exceeding the stated n <= 100000 (RE at 100000,
// AC at 200000), so the node arrays are sized well past the statement.
//
// Pitfalls from the board, both handled here: the input holds several test
// cases and ends at EOF (read until the leading n cannot be read), and it has
// trailing blank lines, so token-based reading rather than line-based.
//
// Ambiguity: the statement never says a "path" must be non-empty; taking the
// single-node path as length 0 only matters for n == 1, and the maximum over
// pairs (including u == v) already yields >= 0.

#include <cstdio>

const int MAXN = 500005;
const int MAXE = 2 * MAXN;

static int head[MAXN], nxt[MAXE], to[MAXE];
static unsigned int wt[MAXE];
static unsigned int d[MAXN];
static int order[MAXN];
static bool vis[MAXN];

const int HB = 20;
const int HSIZE = 1 << HB;
const int HMASK = HSIZE - 1;
static unsigned int hkey[HSIZE];
static int hstamp[HSIZE];
static int curstamp = 0;

inline int hpos(unsigned int x) {
    return (int)((x * 2654435761u) >> (32 - HB));
}

inline void hinsert(unsigned int x) {
    int p = hpos(x);
    while (hstamp[p] == curstamp) {
        if (hkey[p] == x) return;
        p = (p + 1) & HMASK;
    }
    hstamp[p] = curstamp;
    hkey[p] = x;
}

inline bool hfind(unsigned int x) {
    int p = hpos(x);
    while (hstamp[p] == curstamp) {
        if (hkey[p] == x) return true;
        p = (p + 1) & HMASK;
    }
    return false;
}

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static bool readUInt(unsigned int &x) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    x = 0u;
    while (c >= '0' && c <= '9') {
        x = x * 10u + (unsigned int)(c - '0');
        c = gc();
    }
    return true;
}

int main() {
    unsigned int un;
    while (readUInt(un)) {
        int n = (int)un;
        if (n < 1) n = 1;
        if (n > MAXN - 1) n = MAXN - 1; /* defensive; data is not expected here */
        int i;
        for (i = 0; i < n; ++i) { head[i] = -1; vis[i] = false; }
        int ecnt = 0;
        for (i = 0; i + 1 < n; ++i) {
            unsigned int u, v, w;
            if (!readUInt(u) || !readUInt(v) || !readUInt(w)) break;
            to[ecnt] = (int)v; wt[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt; ++ecnt;
            to[ecnt] = (int)u; wt[ecnt] = w; nxt[ecnt] = head[v]; head[v] = ecnt; ++ecnt;
        }

        /* BFS every component (a genuine tree has exactly one) */
        for (int s = 0; s < n; ++s) {
            if (vis[s]) continue;
            vis[s] = true;
            d[s] = 0u;
            int qh = 0, qt = 0;
            order[qt++] = s;
            while (qh < qt) {
                int v = order[qh++];
                for (int e = head[v]; e != -1; e = nxt[e]) {
                    int y = to[e];
                    if (!vis[y]) { vis[y] = true; d[y] = d[v] ^ wt[e]; order[qt++] = y; }
                }
            }
        }

        unsigned int ans = 0u, mask = 0u;
        for (int b = 30; b >= 0; --b) {
            mask |= (1u << b);
            unsigned int cand = ans | (1u << b);
            ++curstamp;
            bool ok = false;
            for (i = 0; i < n; ++i) hinsert(d[i] & mask);
            for (i = 0; i < n; ++i) {
                if (hfind((d[i] & mask) ^ cand)) { ok = true; break; }
            }
            if (ok) ans = cand;
        }
        printf("%u\n", ans);
    }
    return 0;
}
