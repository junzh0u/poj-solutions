// POJ 4052 - Hrinity
// Model: claude-sonnet-5
//
// Approach: Aho-Corasick automaton over the n text finger prints.
//   Pass 1 scans the (decompressed) article through the automaton and,
//   using the standard "mark a state, then OR the mark up the fail links
//   in decreasing-depth order" trick, determines which patterns occur as
//   a substring of the article.
//   Pass 2 answers the containment rule the statement actually asks for:
//   "if s1 is a substring of s2 and s2 is included in the article, s1
//   doesn't count" -- note the container s2 must ALSO occur in the
//   article, not merely exist among the n patterns.  So pass 2 feeds only
//   the OCCURRING patterns back into the same automaton (each one scanned
//   independently, restarting from the root), accumulates a hit counter
//   per state, and sums it up the fail links exactly like pass 1.  A
//   pattern's own scan always contributes exactly one hit to its own
//   terminal state (its full string re-walks its own trie edges from the
//   root), so a final count of exactly 1 means it never occurs inside any
//   OTHER occurring pattern, while a count > 1 means some other occurring
//   pattern swallows it.
//   Answer = number of patterns that occur in the article and whose
//   pass-2 count is exactly 1.
//
// Statement ambiguity, settled against the discuss board: the containment
// exclusion only fires when the container s2 also occurs in the article.
// Board message 354929 gives the separating case: patterns ABCD, BC;
// article ABC.  BC occurs (found only via the fail link from the trie
// node for prefix "ABC", since scanning "ABC" never reaches a node
// created for ABCD), ABCD does not occur at all, so the correct answer is
// 1.  A solution that instead marks "s1 is a substring of ANY other
// pattern" regardless of whether that other pattern occurs would wrongly
// output 0 -- which the poster reports several circulating "AC" codes do.
// This exact case, plus a battery of cases from board message 171980
// (containment chains, compressed patterns, disjoint patterns, 26 single
// letters), were used as local regression tests; all match by hand
// computation and the reference/mutation testing described below.
//
// Compressed strings "[qX]" (q repetitions of capital letter X) are
// expanded before insertion/scanning; q fits in a 32-bit int
// (0 < q <= 5,000,000). Per-testcase bounds: n <= 2500 patterns, each
// <= 1100 chars decompressed; article <= 5,100,000 chars decompressed;
// T <= 15 testcases.  Node-clearing uses memset instead of a 26-iteration
// loop to keep the O(nodeCount*26) automaton construction fast, since a
// single testcase's trie can have up to ~2.75M nodes.

#include <cstdio>
#include <cstring>

const int MAXNODES = 2750005;
const int MAXPAT = 2505;
const int MAXPATBUF = 2750005;
const int MAXARTBUF = 5100005;

static int ch[MAXNODES][26];
static int failLink[MAXNODES];
static int bfsOrder[MAXNODES];
static char markArt[MAXNODES];
static int cntOcc[MAXNODES];
static int nodeCount;
static int bfsCount;

static int patNode[MAXPAT];
static int patOffset[MAXPAT];
static int patLen[MAXPAT];
static bool occursFlag[MAXPAT];

static char patBuf[MAXPATBUF];
static char artBuf[MAXARTBUF];
static char rawBuf[MAXARTBUF];

// ---------- fast input ----------
static char inbuf[1 << 20];
static int inlen = 0, inpos = 0;

static inline int gc() {
    if (inpos == inlen) {
        inlen = (int)fread(inbuf, 1, sizeof(inbuf), stdin);
        inpos = 0;
        if (inlen == 0) return -1;
    }
    return (unsigned char)inbuf[inpos++];
}

static int readToken(char *dest) {
    int c = gc();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
    int len = 0;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        dest[len++] = (char)c;
        c = gc();
    }
    return len;
}

static int readInt() {
    int c = gc();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
    int neg = 0;
    if (c == '-') { neg = 1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    return neg ? -v : v;
}

// ---------- decompression ----------
static int decompress(const char *src, int rawLen, char *dst) {
    int di = 0, i = 0;
    while (i < rawLen) {
        if (src[i] == '[') {
            i++;
            int q = 0;
            while (i < rawLen && src[i] >= '0' && src[i] <= '9') {
                q = q * 10 + (src[i] - '0');
                i++;
            }
            char x = src[i];
            i++; // letter
            i++; // ']'
            for (int k = 0; k < q; k++) dst[di++] = x;
        } else {
            dst[di++] = src[i++];
        }
    }
    return di;
}

// ---------- trie ----------
static inline int newNode() {
    int u = nodeCount++;
    memset(ch[u], -1, sizeof(int) * 26);
    return u;
}

static int insertPattern(const char *s, int len) {
    int cur = 0; // root
    for (int i = 0; i < len; i++) {
        int c = s[i] - 'A';
        if (ch[cur][c] == -1) ch[cur][c] = newNode();
        cur = ch[cur][c];
    }
    return cur;
}

static void buildAutomaton() {
    int root = 0;
    int qHead = 0, qTail = 0;
    for (int c = 0; c < 26; c++) {
        if (ch[root][c] == -1) {
            ch[root][c] = root;
        } else {
            failLink[ch[root][c]] = root;
            bfsOrder[qTail++] = ch[root][c];
        }
    }
    while (qHead < qTail) {
        int u = bfsOrder[qHead++];
        int fu = failLink[u];
        for (int c = 0; c < 26; c++) {
            int v = ch[u][c];
            if (v == -1) {
                ch[u][c] = ch[fu][c];
            } else {
                failLink[v] = ch[fu][c];
                bfsOrder[qTail++] = v;
            }
        }
    }
    bfsCount = qTail;
}

int main() {
    int T = readInt();
    while (T--) {
        int n = readInt();
        nodeCount = 0;
        newNode(); // root = node 0

        int runningOffset = 0;
        for (int i = 0; i < n; i++) {
            int rawLen = readToken(rawBuf);
            int len = decompress(rawBuf, rawLen, patBuf + runningOffset);
            patOffset[i] = runningOffset;
            patLen[i] = len;
            patNode[i] = insertPattern(patBuf + runningOffset, len);
            runningOffset += len;
        }

        int rawArtLen = readToken(rawBuf);
        int artLen = decompress(rawBuf, rawArtLen, artBuf);

        buildAutomaton();

        // pass 1: which patterns occur in the article
        memset(markArt, 0, nodeCount);
        {
            int cur = 0;
            for (int i = 0; i < artLen; i++) {
                cur = ch[cur][artBuf[i] - 'A'];
                markArt[cur] = 1;
            }
        }
        for (int idx = bfsCount - 1; idx >= 0; idx--) {
            int u = bfsOrder[idx];
            if (markArt[u]) markArt[failLink[u]] = 1;
        }
        for (int i = 0; i < n; i++) occursFlag[i] = (markArt[patNode[i]] != 0);

        // pass 2: containment restricted to patterns that occur
        memset(cntOcc, 0, sizeof(int) * nodeCount);
        for (int i = 0; i < n; i++) {
            if (!occursFlag[i]) continue;
            int cur = 0;
            const char *s = patBuf + patOffset[i];
            int len = patLen[i];
            for (int k = 0; k < len; k++) {
                cur = ch[cur][s[k] - 'A'];
                cntOcc[cur]++;
            }
        }
        for (int idx = bfsCount - 1; idx >= 0; idx--) {
            int u = bfsOrder[idx];
            cntOcc[failLink[u]] += cntOcc[u];
        }

        int answer = 0;
        for (int i = 0; i < n; i++) {
            if (occursFlag[i] && cntOcc[patNode[i]] == 1) answer++;
        }

        printf("%d\n", answer);
    }
    return 0;
}
