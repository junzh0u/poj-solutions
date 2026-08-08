// POJ 2949 - Word Rings
// Model: claude-opus-5
//
// Maximum mean cycle.  Each word of length L becomes an arc of weight L from
// the node named by its first two letters to the node named by its last two
// letters, so there are 26*26 = 676 nodes.  A ring is a closed walk with
// distinct words, i.e. a closed trail; its mean is a mediant of the means of
// the simple cycles it decomposes into, so the optimum is always attained on a
// simple cycle.  That also makes it safe to keep only the longest word per
// ordered pair of endpoints, since a simple cycle uses each pair at most once.
//
// The answer is then the maximum mean cycle, computed exactly by Karp's
// algorithm run on each strongly connected component (Karp needs strong
// connectivity; components come from a bitset transitive closure over the 676
// nodes, cheap at this size).  Karp costs |V_c| * |E_c| per component, bounded
// overall by 676 * n, and the final min/max over ratios is done with exact
// integer cross-multiplication -- every quantity fits in int, since a simple
// cycle has at most 676 arcs of weight at most 1000.
//
// Output ambiguity: the statement asks for two decimals with +-0.01 tolerance,
// and the sample prints 21.66 for 65/3 = 21.6667, so the reference truncates
// rather than rounds.  Truncating the exact fraction with integer arithmetic
// reproduces the sample byte for byte and stays strictly within 0.01 of the
// true value either way; printing %.2f would give 21.67, exactly 0.01 from the
// sample's own number.
//
// Words shorter than two letters have no two-letter head or tail and cannot
// take part in any ring, so they are dropped.

#include <cstdio>
#include <cstring>

static const int NV = 676;
static const int WORDS = (NV + 31) / 32;
static const int NEG = -1000000000;

/* ---- fast input ---- */
static const int IBUFSZ = 1 << 16;
static char ibuf[IBUFSZ];
static int ipos = 0, ilen = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, IBUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
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
/* reads one whitespace-delimited token, keeping only length and the
   first two / last two characters */
static bool readWord(int &len, int &c0, int &c1, int &p, int &q) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    len = 0; c0 = c1 = p = q = -1;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        if (len == 0) c0 = c;
        else if (len == 1) c1 = c;
        p = q; q = c;
        ++len;
        c = gc();
    }
    return true;
}

/* ---- per-test-case state ---- */
static int best[NV * NV];       /* best[u*NV+v] = longest word for that arc, 0 = absent */
static int usedPair[100005];
static int eu[100005], ev[100005], ew[100005];
static unsigned int reach[NV][WORDS];
static int comp[NV], localIdx[NV];
static int sortedU[100005], sortedV[100005], sortedW[100005];
static int sccStart[NV + 1], sccCnt[NV + 1];
static int nodeOfScc[NV], sccNodeStart[NV + 1], sccNodeCnt[NV + 1];
static int d[NV + 1][NV];

int main() {
    int n;
    while (readInt(n)) {
        if (n == 0) break;
        int nUsed = 0;
        for (int i = 0; i < n; ++i) {
            int len, c0, c1, p, q;
            if (!readWord(len, c0, c1, p, q)) { len = 0; }
            if (len < 2) continue;             /* no two-letter head/tail: unusable */
            if (c0 < 'a' || c0 > 'z' || c1 < 'a' || c1 > 'z' ||
                p < 'a' || p > 'z' || q < 'a' || q > 'z') continue;
            int u = (c0 - 'a') * 26 + (c1 - 'a');
            int v = (p - 'a') * 26 + (q - 'a');
            int id = u * NV + v;
            if (best[id] == 0) usedPair[nUsed++] = id;
            if (len > best[id]) best[id] = len;   /* parallel arcs: keep the longest */
        }

        int m = 0;
        memset(reach, 0, sizeof(reach));
        for (int i = 0; i < nUsed; ++i) {
            int id = usedPair[i];
            int u = id / NV, v = id % NV;
            eu[m] = u; ev[m] = v; ew[m] = best[id]; ++m;
            reach[u][v >> 5] |= 1u << (v & 31);
            best[id] = 0;
        }

        /* transitive closure over 676 nodes (bitset Floyd) */
        for (int k = 0; k < NV; ++k) {
            unsigned int mask = 1u << (k & 31);
            int wk = k >> 5;
            const unsigned int *rk = reach[k];
            for (int u = 0; u < NV; ++u) {
                if (reach[u][wk] & mask) {
                    unsigned int *ru = reach[u];
                    for (int w = 0; w < WORDS; ++w) ru[w] |= rk[w];
                }
            }
        }

        /* strongly connected components that actually contain a cycle */
        int nscc = 0;
        for (int i = 0; i < NV; ++i) comp[i] = -1;
        for (int v = 0; v < NV; ++v) {
            if (comp[v] != -1) continue;
            if (!(reach[v][v >> 5] & (1u << (v & 31)))) continue;
            comp[v] = nscc;
            for (int u = v + 1; u < NV; ++u) {
                if (comp[u] != -1) continue;
                if ((reach[v][u >> 5] & (1u << (u & 31))) &&
                    (reach[u][v >> 5] & (1u << (v & 31))))
                    comp[u] = nscc;
            }
            ++nscc;
        }

        int ansNum = 0, ansDen = 0;   /* den == 0 means "no cycle found" */

        if (nscc > 0) {
            /* bucket the nodes of each component */
            for (int c = 0; c <= nscc; ++c) sccNodeCnt[c] = 0;
            for (int v = 0; v < NV; ++v) if (comp[v] >= 0) ++sccNodeCnt[comp[v]];
            sccNodeStart[0] = 0;
            for (int c = 0; c < nscc; ++c) sccNodeStart[c + 1] = sccNodeStart[c] + sccNodeCnt[c];
            {
                int pos[NV];
                for (int c = 0; c < nscc; ++c) pos[c] = sccNodeStart[c];
                for (int v = 0; v < NV; ++v)
                    if (comp[v] >= 0) { localIdx[v] = pos[comp[v]] - sccNodeStart[comp[v]];
                                        nodeOfScc[pos[comp[v]]++] = v; }
            }

            /* bucket the intra-component arcs */
            for (int c = 0; c <= nscc; ++c) sccCnt[c] = 0;
            for (int i = 0; i < m; ++i) {
                int cu = comp[eu[i]];
                if (cu >= 0 && cu == comp[ev[i]]) ++sccCnt[cu];
            }
            sccStart[0] = 0;
            for (int c = 0; c < nscc; ++c) sccStart[c + 1] = sccStart[c] + sccCnt[c];
            {
                int pos[NV];
                for (int c = 0; c < nscc; ++c) pos[c] = sccStart[c];
                for (int i = 0; i < m; ++i) {
                    int cu = comp[eu[i]];
                    if (cu >= 0 && cu == comp[ev[i]]) {
                        int t = pos[cu]++;
                        sortedU[t] = localIdx[eu[i]];
                        sortedV[t] = localIdx[ev[i]];
                        sortedW[t] = ew[i];
                    }
                }
            }

            /* Karp's maximum mean cycle, per strongly connected component */
            for (int c = 0; c < nscc; ++c) {
                int sz = sccNodeCnt[c];
                int e0 = sccStart[c], e1 = sccStart[c + 1];
                if (e0 == e1) continue;
                for (int i = 0; i < sz; ++i) d[0][i] = 0;
                for (int k = 1; k <= sz; ++k) {
                    int *dk = d[k], *dp = d[k - 1];
                    for (int i = 0; i < sz; ++i) dk[i] = NEG;
                    for (int i = e0; i < e1; ++i) {
                        int du = dp[sortedU[i]];
                        if (du == NEG) continue;
                        int cand = du + sortedW[i];
                        if (cand > dk[sortedV[i]]) dk[sortedV[i]] = cand;
                    }
                }
                for (int i = 0; i < sz; ++i) {
                    if (d[sz][i] == NEG) continue;
                    int bn = 0, bd = 0;   /* min over k of (d[sz]-d[k])/(sz-k) */
                    for (int k = 0; k < sz; ++k) {
                        if (d[k][i] == NEG) continue;
                        int nu = d[sz][i] - d[k][i];
                        int de = sz - k;
                        if (bd == 0 || nu * bd < bn * de) { bn = nu; bd = de; }
                    }
                    if (bd == 0) continue;
                    if (ansDen == 0 || bn * ansDen > ansNum * bd) { ansNum = bn; ansDen = bd; }
                }
            }
        }

        if (ansDen == 0) {
            printf("No solution.\n");
        } else {
            /* truncate to two decimals with exact integer arithmetic: the
               sample's 21.66 for 65/3 shows the reference truncates, and a
               truncated value is still within the allowed +-0.01 anyway */
            int t = ansNum * 100 / ansDen;
            printf("%d.%02d\n", t / 100, t % 100);
        }
    }
    return 0;
}
