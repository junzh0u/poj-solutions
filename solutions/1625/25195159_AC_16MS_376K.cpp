// POJ 1625 - Censored!
// Model: claude-opus-5
//
// Count length-M strings over an N-letter alphabet that contain none of P
// forbidden words: Aho-Corasick automaton + DP, with big integers.
//
// Build the goto automaton (trie graph) over the P words (<= 10 words of <= 10
// letters, so <= 101 nodes including the root).  A node is "bad" if it ends a
// forbidden word, or if its fail link leads to a bad node -- the propagation
// matters because one forbidden word may be a substring of another, and without
// it a longer word's node would hide the shorter word's match.  dp[i][u] counts
// length-i prefixes that end in state u having never touched a bad state; the
// answer is the sum over all good u of dp[M][u].
//
// The answer can be as large as 50^50 (85 decimal digits), so it is accumulated
// in base-10^9 big integers.
//
// Two input traps, both from the discuss board rather than the statement:
//   * although the statement promises every letter has ASCII code > 32, the real
//     data contains bytes above 127, which are negative as plain `char`.  Every
//     character is indexed through `unsigned char`, so the alphabet lookup table
//     is 256 wide and never indexed negatively.
//   * the alphabet and the words are read as whole lines (fgets) rather than
//     with scanf("%s"), so a byte that happens to be whitespace cannot split a
//     line or desynchronise the pairing.  Trailing CR is stripped.
//
// Words containing a character outside the given alphabet, or empty word lines,
// can never occur in a sentence and are skipped rather than inserted.

#include <cstdio>
#include <cstring>

const int MAXNODE = 105;   /* P<=10 words of length <=10 -> 100 nodes + root */
const int MAXA = 55;
const int LIMB = 12;       /* 50^50 < 10^85 -> 10 limbs of 9 digits is enough */
const unsigned int BASE = 1000000000u;

struct Big {
    int n;
    unsigned int d[LIMB];
};

static void bigZero(Big &a) { a.n = 1; memset(a.d, 0, sizeof(a.d)); }
static void bigOne(Big &a) { bigZero(a); a.d[0] = 1u; }

static void bigAdd(Big &a, const Big &b) {   /* a += b */
    int n = a.n > b.n ? a.n : b.n;
    unsigned int carry = 0;
    for (int i = 0; i < n; ++i) {
        unsigned int s = a.d[i] + (i < b.n ? b.d[i] : 0u) + carry;
        if (s >= BASE) { s -= BASE; carry = 1; } else carry = 0;
        a.d[i] = s;
    }
    if (carry) { a.d[n] = 1u; ++n; }
    a.n = n;
}

static void bigPrint(const Big &a) {
    printf("%u", a.d[a.n - 1]);
    for (int i = a.n - 2; i >= 0; --i) printf("%09u", a.d[i]);
    printf("\n");
}

static int go[MAXNODE][MAXA];
static int fail_[MAXNODE];
static bool bad[MAXNODE];
static int nodeCount;

static int alphaIdx[256];        /* indexed by unsigned char */
static int nAlpha;

static char lineBuf[1024];

static char *readLine(void) {
    if (!fgets(lineBuf, (int)sizeof(lineBuf), stdin)) return 0;
    int L = (int)strlen(lineBuf);
    while (L > 0 && (lineBuf[L - 1] == '\n' || lineBuf[L - 1] == '\r')) lineBuf[--L] = '\0';
    return lineBuf;
}

int main() {
    int N = 0, M = 0, P = 0;
    if (!readLine()) return 0;
    if (sscanf(lineBuf, "%d %d %d", &N, &M, &P) != 3) return 0;

    for (int i = 0; i < 256; ++i) alphaIdx[i] = -1;

    if (!readLine()) return 0;
    int L = (int)strlen(lineBuf);
    nAlpha = 0;
    for (int i = 0; i < L && nAlpha < N; ++i) {
        unsigned char c = (unsigned char)lineBuf[i];
        if (alphaIdx[c] < 0) alphaIdx[c] = nAlpha++;
    }

    /* build trie */
    nodeCount = 1;
    memset(go, -1, sizeof(go));
    memset(bad, 0, sizeof(bad));
    for (int w = 0; w < P; ++w) {
        if (!readLine()) break;
        int wl = (int)strlen(lineBuf);
        bool ok = (wl > 0);
        for (int i = 0; i < wl; ++i)
            if (alphaIdx[(unsigned char)lineBuf[i]] < 0) { ok = false; break; }
        if (!ok) continue;                 /* word can never occur */
        int cur = 0;
        for (int i = 0; i < wl; ++i) {
            int c = alphaIdx[(unsigned char)lineBuf[i]];
            if (go[cur][c] < 0) {
                go[cur][c] = nodeCount;
                ++nodeCount;
            }
            cur = go[cur][c];
        }
        bad[cur] = true;
    }

    /* BFS: build goto automaton (trie graph) + fail links, propagate bad */
    int queue_[MAXNODE], qh = 0, qt = 0;
    for (int c = 0; c < nAlpha; ++c) {
        int v = go[0][c];
        if (v < 0) go[0][c] = 0;
        else { fail_[v] = 0; queue_[qt++] = v; }
    }
    fail_[0] = 0;
    while (qh < qt) {
        int u = queue_[qh++];
        if (bad[fail_[u]]) bad[u] = true;
        for (int c = 0; c < nAlpha; ++c) {
            int v = go[u][c];
            if (v < 0) {
                go[u][c] = go[fail_[u]][c];
            } else {
                fail_[v] = go[fail_[u]][c];
                queue_[qt++] = v;
            }
        }
    }

    /* dp over sentence length */
    static Big cur[MAXNODE], nxt[MAXNODE];
    for (int i = 0; i < nodeCount; ++i) bigZero(cur[i]);
    if (bad[0]) { printf("0\n"); return 0; }
    bigOne(cur[0]);

    for (int step = 0; step < M; ++step) {
        for (int i = 0; i < nodeCount; ++i) bigZero(nxt[i]);
        for (int u = 0; u < nodeCount; ++u) {
            if (bad[u]) continue;
            if (cur[u].n == 1 && cur[u].d[0] == 0u) continue;
            for (int c = 0; c < nAlpha; ++c) {
                int v = go[u][c];
                if (bad[v]) continue;
                bigAdd(nxt[v], cur[u]);
            }
        }
        for (int i = 0; i < nodeCount; ++i) cur[i] = nxt[i];
    }

    Big ans;
    bigZero(ans);
    for (int i = 0; i < nodeCount; ++i)
        if (!bad[i]) bigAdd(ans, cur[i]);
    bigPrint(ans);
    return 0;
}
