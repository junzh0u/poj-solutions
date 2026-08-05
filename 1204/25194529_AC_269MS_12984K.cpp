// POJ 1204 - Word Puzzles
//
// Locate each of W <= 1000 words in an L x C <= 1000 x 1000 letter grid and
// report the coordinates of its first letter plus the direction it runs in.
// Directions are lettered clockwise from A = north, so
// A=(-1,0) B=(-1,+1) C=(0,+1) D=(+1,+1) E=(+1,0) F=(+1,-1) G=(0,-1) H=(-1,-1).
//
// Approach: one Aho-Corasick automaton over all the words, then walk every
// maximal line of the grid in each of the 8 directions.  Each direction visits
// every cell exactly once (a line starts at a cell whose predecessor along the
// direction is off the board), so the whole search is 8*L*C = 8e6 automaton
// steps regardless of how many or how long the words are.  A match reported at
// line offset i for a word of length n started at offset i-n+1, which maps back
// to the grid cell (r0 + (i-n+1)*dr, c0 + (i-n+1)*dc) of the line's origin.
//
// Only one occurrence per word is needed (the judge is special), so word-end
// nodes are retired once located: the suffix-output chain is path-compressed
// past nodes with nothing left to report, and a `remaining` counter aborts the
// whole scan as soon as every word has been placed.  Identical words appear as
// several ids hanging off the same node and are all answered at once.
//
// Memory is the one real constraint (65536K).  The sum of the word lengths can
// in principle reach 1e6, and a dense 26-way goto table over that many nodes
// would be 104MB.  So the trie is built two ways: the flat goto table (O(1)
// transitions) when the node bound fits in ~400k nodes, and linked-list
// children with an explicit fail-link loop otherwise.  The fail loop is
// amortised O(1) per character, so the slow path stays comfortably in time.
//
// Statement ambiguities: the origin is the upper-left corner and coordinates
// are 0-based "line column"; the statement does not say what to do when a word
// occurs more than once, but the problem is judged specially and any valid
// occurrence is accepted.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static int L, C, W;
static char *grid;

/* ---- Aho-Corasick ------------------------------------------------------- */
static int nodeCnt;
static int *go_;      /* full goto table (fast path), 26 per node          */
static int *thead, *tsib; /* linked-list children (low-memory path)        */
static char *tchr;
static int *fail_;
static int *nextOut;  /* path-compressed suffix chain of word-end nodes    */
static char *hasUnf;  /* node holds a word not yet located                 */
static int *wordHead; /* first word id ending at node, -1 if none          */
static int *wordNext; /* next word id sharing the same node                */
static int *wlen;
static bool useTable;

static int ansR[1005], ansC[1005];
static char ansD[1005];
static int remaining;

static inline int childOf(int u, int c) {
    int v = thead[u];
    while (v != -1) {
        if (tchr[v] == (char)c) return v;
        v = tsib[v];
    }
    return 0;
}

static inline int stepAC(int u, int c) {
    if (useTable) return go_[u * 26 + c];
    while (u && !childOf(u, c)) u = fail_[u];
    int v = childOf(u, c);
    return v ? v : 0;
}

static int getOut(int u) {
    if (u == 0) return 0;
    if (hasUnf[u]) return u;
    return nextOut[u] = getOut(nextOut[u]);
}

int main() {
    if (scanf("%d %d %d", &L, &C, &W) != 3) return 0;
    grid = (char *)malloc((size_t)L * C);
    char *row = (char *)malloc(C + 16);
    for (int r = 0; r < L; r++) {
        if (scanf("%s", row) != 1) break;
        memcpy(grid + (size_t)r * C, row, C);
    }
    free(row);

    /* read all words first: their total length bounds the node count */
    std::vector<char> wbuf;
    std::vector<int> woff(W), wl(W);
    {
        std::vector<char> tmp(1000006);
        for (int i = 0; i < W; i++) {
            if (scanf("%s", &tmp[0]) != 1) { tmp[0] = 0; }
            int len = (int)strlen(&tmp[0]);
            woff[i] = (int)wbuf.size();
            wl[i] = len;
            wbuf.insert(wbuf.end(), tmp.begin(), tmp.begin() + len);
        }
    }
    int bound = (int)wbuf.size() + 2;

    wlen = (int *)malloc(sizeof(int) * (W + 1));
    for (int i = 0; i < W; i++) wlen[i] = wl[i];

    useTable = (bound <= 400000);

    fail_ = (int *)malloc(sizeof(int) * bound);
    nextOut = (int *)malloc(sizeof(int) * bound);
    hasUnf = (char *)malloc(bound);
    wordHead = (int *)malloc(sizeof(int) * bound);
    wordNext = (int *)malloc(sizeof(int) * (W + 1));
    nodeCnt = 1;
    wordHead[0] = -1;

    if (useTable) {
        go_ = (int *)calloc((size_t)bound * 26, sizeof(int));
    } else {
        thead = (int *)malloc(sizeof(int) * bound);
        tsib = (int *)malloc(sizeof(int) * bound);
        tchr = (char *)malloc(bound);
        thead[0] = -1;
    }

    /* insert */
    for (int i = 0; i < W; i++) {
        int u = 0;
        const char *s = wbuf.empty() ? "" : &wbuf[woff[i]];
        int len = wl[i];
        bool bad = false;
        for (int k = 0; k < len; k++) {
            int c = s[k] - 'A';
            if (c < 0 || c > 25) { bad = true; break; }
            int v;
            if (useTable) {
                v = go_[u * 26 + c];
                if (!v) {
                    v = nodeCnt++;
                    wordHead[v] = -1;
                    go_[u * 26 + c] = v;
                }
            } else {
                v = childOf(u, c);
                if (!v) {
                    v = nodeCnt++;
                    wordHead[v] = -1;
                    thead[v] = -1;
                    tchr[v] = (char)c;
                    tsib[v] = thead[u];
                    thead[u] = v;
                }
            }
            u = v;
        }
        if (bad || len == 0) { wordNext[i] = -1; continue; }
        wordNext[i] = wordHead[u];
        wordHead[u] = i;
    }

    /* BFS: fail links, goto completion, suffix-output links */
    {
        std::vector<int> q(nodeCnt);
        int qh = 0, qt = 0;
        fail_[0] = 0;
        nextOut[0] = 0;
        hasUnf[0] = 0;
        if (useTable) {
            for (int c = 0; c < 26; c++) {
                int v = go_[c];
                if (v) { fail_[v] = 0; q[qt++] = v; }
            }
        } else {
            for (int v = thead[0]; v != -1; v = tsib[v]) { fail_[v] = 0; q[qt++] = v; }
        }
        while (qh < qt) {
            int u = q[qh++];
            nextOut[u] = wordHead[fail_[u]] != -1 ? fail_[u] : nextOut[fail_[u]];
            hasUnf[u] = (char)(wordHead[u] != -1);
            if (useTable) {
                for (int c = 0; c < 26; c++) {
                    int v = go_[u * 26 + c];
                    int f = go_[fail_[u] * 26 + c];
                    if (!v) {
                        go_[u * 26 + c] = f;
                    } else {
                        fail_[v] = f;
                        q[qt++] = v;
                    }
                }
            } else {
                for (int v = thead[u]; v != -1; v = tsib[v]) {
                    int f = fail_[u], c = tchr[v];
                    while (f && !childOf(f, c)) f = fail_[f];
                    int g = childOf(f, c);
                    fail_[v] = (g && g != v) ? g : 0;
                    q[qt++] = v;
                }
            }
        }
    }

    remaining = 0;
    for (int i = 0; i < W; i++) { ansR[i] = 0; ansC[i] = 0; ansD[i] = 'A'; }
    {
        /* count words that can actually be located */
        for (int i = 0; i < W; i++) remaining++;
    }

    static const int DR[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    static const int DC[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int d = 0; d < 8 && remaining > 0; d++) {
        int dr = DR[d], dc = DC[d];
        char letter = (char)('A' + d);
        for (int r = 0; r < L && remaining > 0; r++) {
            for (int c = 0; c < C && remaining > 0; c++) {
                int pr = r - dr, pc = c - dc;
                if (pr >= 0 && pr < L && pc >= 0 && pc < C) continue;
                int u = 0;
                int rr = r, cc = c, i = 0;
                while (rr >= 0 && rr < L && cc >= 0 && cc < C) {
                    int ch = grid[(size_t)rr * C + cc] - 'A';
                    u = (ch < 0 || ch > 25) ? 0 : stepAC(u, ch);
                    int v = getOut(u);
                    while (v) {
                        int id = wordHead[v];
                        int st = i - wlen[id] + 1;
                        int sr = r + st * dr, sc = c + st * dc;
                        while (id != -1) {
                            ansR[id] = sr; ansC[id] = sc; ansD[id] = letter;
                            remaining--;
                            id = wordNext[id];
                        }
                        hasUnf[v] = 0;
                        v = getOut(nextOut[v]);
                    }
                    i++;
                    rr += dr; cc += dc;
                }
            }
        }
    }

    for (int i = 0; i < W; i++) printf("%d %d %c\n", ansR[i], ansC[i], ansD[i]);
    return 0;
}
