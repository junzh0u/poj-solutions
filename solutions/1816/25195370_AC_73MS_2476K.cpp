// POJ 1816 - Wild Words
// Model: claude-opus-5
//
// Build a trie over the N patterns with 28 possible edge labels ('a'..'z',
// '?', '*'), stored left-child/right-sibling so the worst case of 600000
// nodes costs ~6 ints per node instead of a 28-way array (which would need
// 67MB against a 64MB limit).  Every pattern id is appended to a linked list
// hanging off its terminal node, because the input contains duplicate
// patterns and a plain "one id per node" field silently loses all but one.
//
// For each word, DFS the trie over states (node, position-in-word):
//   letter edge -> consume word[pos] if it matches
//   '?'    edge -> consume any one character
//   '*'    edge -> descend for every j in [pos, wlen] (zero or more chars)
// A state is memoised with a 21-bit mask per node (word length <= 20), which
// is what keeps the '*' branching from blowing up; the memo also means a
// terminal node reached along several distinct routes contributes its ids
// once.  Reaching a terminal node is *not* a stopping condition: "t***"
// still has to be walked to its end to match "t".
//
// Pruning: minC[u]/maxC[u] are the fewest/most word characters any pattern
// suffix below u can consume ('*' makes maxC unbounded).  A state whose
// remaining word length falls outside that interval cannot lead anywhere.
//
// Output: matched ids are collected into a boolean array and emitted by a
// single 0..N-1 scan, so they come out sorted and deduplicated for free.
// Per the statement each number is followed by a single blank.

#include <cstdio>
#include <cstring>

static const int MAXP = 100005;
static const int MAXNODE = 600006;
static const int INF = 1000;

static int chFirst[MAXNODE];
static int chNext[MAXNODE];
static int idHead[MAXNODE];
static int minC[MAXNODE];
static int maxC[MAXNODE];
static int markBits[MAXNODE];
static int markStamp[MAXNODE];
static char lab[MAXNODE];

static int idNext[MAXP];
static bool found[MAXP];

static int nodeCnt = 1;
static int stamp = 0;

static char word[64];
static int wlen;

static void insertPattern(const char *s, int id) {
    int u = 0;
    for (int i = 0; s[i]; ++i) {
        int c = chFirst[u];
        while (c != -1 && lab[c] != s[i]) c = chNext[c];
        if (c == -1) {
            c = nodeCnt++;
            lab[c] = s[i];
            chFirst[c] = -1;
            idHead[c] = -1;
            chNext[c] = chFirst[u];
            chFirst[u] = c;
        }
        u = c;
    }
    idNext[id] = idHead[u];
    idHead[u] = id;
}

static void dfs(int u, int pos) {
    if (markStamp[u] != stamp) {
        markStamp[u] = stamp;
        markBits[u] = 0;
    }
    if (markBits[u] & (1 << pos)) return;
    markBits[u] |= (1 << pos);

    int rem = wlen - pos;
    if (rem < minC[u] || rem > maxC[u]) return;

    if (rem == 0) {
        for (int id = idHead[u]; id != -1; id = idNext[id]) found[id] = true;
    }
    for (int c = chFirst[u]; c != -1; c = chNext[c]) {
        char L = lab[c];
        if (L == '*') {
            for (int j = pos; j <= wlen; ++j) dfs(c, j);
        } else if (L == '?') {
            if (pos < wlen) dfs(c, pos + 1);
        } else {
            if (pos < wlen && word[pos] == L) dfs(c, pos + 1);
        }
    }
}

static char obuf[1 << 16];
static int olen = 0;

static void oflush() {
    if (olen) { fwrite(obuf, 1, olen, stdout); olen = 0; }
}
static void oputc(char c) {
    if (olen == (int)sizeof(obuf)) oflush();
    obuf[olen++] = c;
}
static void oputs(const char *s) {
    while (*s) oputc(*s++);
}
static void oputint(int v) {
    char tmp[12];
    int k = 0;
    if (v == 0) tmp[k++] = '0';
    while (v > 0) { tmp[k++] = (char)('0' + v % 10); v /= 10; }
    while (k > 0) oputc(tmp[--k]);
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    chFirst[0] = -1;
    idHead[0] = -1;

    char pat[64];
    for (int i = 0; i < n; ++i) {
        if (scanf("%15s", pat) != 1) return 0;
        insertPattern(pat, i);
    }

    for (int u = nodeCnt - 1; u >= 0; --u) {
        int mn = INF, mx = -1;
        if (idHead[u] != -1) { mn = 0; mx = 0; }
        for (int c = chFirst[u]; c != -1; c = chNext[c]) {
            if (maxC[c] < 0) continue;
            if (lab[c] == '*') {
                if (minC[c] < mn) mn = minC[c];
                mx = INF;
            } else {
                if (minC[c] + 1 < mn) mn = minC[c] + 1;
                int t = maxC[c] + 1;
                if (t > INF) t = INF;
                if (t > mx) mx = t;
            }
        }
        minC[u] = mn;
        maxC[u] = mx;
        markStamp[u] = -1;
    }

    for (int q = 0; q < m; ++q) {
        if (scanf("%23s", word) != 1) break;
        wlen = (int)strlen(word);
        ++stamp;
        dfs(0, 0);
        bool any = false;
        for (int i = 0; i < n; ++i) {
            if (found[i]) {
                found[i] = false;
                any = true;
                oputint(i);
                oputc(' ');
            }
        }
        if (!any) oputs("Not match");
        oputc('\n');
    }
    oflush();
    return 0;
}
