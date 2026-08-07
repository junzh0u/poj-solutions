// POJ 1451 - T9
// Model: claude-opus-5
//
// Two tries.  The letter trie holds the dictionary; every node accumulates the
// summed probability of all words that begin with its string ("the probability
// of a character combination is the sum of the probabilities of all words in
// the dictionary that begin with this character combination").  A second trie
// is keyed by the digit encoding (2=abc .. 9=wxyz): each letter node is filed
// under the digit string of its own string, and the digit node remembers the
// best letter node filed there.  A query is then a plain O(len) walk of the
// digit trie, printing the remembered node's string after every keystroke.
//
// Two details the statement is easy to get wrong, both confirmed against the
// samples and the discuss board:
//   * the answer at each keystroke is chosen independently, so it may jump to
//     an unrelated word (sample: "i", "id", then "hel").  Board case
//     "abyb 3 / abye 13 / aaza 4" with query 22921 answers a, ab, aby, aaza --
//     the depth-4 winner is not a descendant of the depth-3 winner.
//   * ties go to the alphabetically first combination.  Pre-order traversal of
//     the letter trie with children kept in ascending letter order visits nodes
//     in lexicographic order, so a strict '>' when filing keeps the first one.
// A number sequence may be nothing but the terminating '1'; it still emits its
// terminating blank line (and each scenario ends with one extra blank line).
//
// Memory limit is 10000K, so the tries use left-child/right-sibling links
// rather than 26- or 8-way child arrays: 1000 words x 100 letters is up to
// 100001 nodes, which as arrays of pointers would not fit.

#include <cstdio>

const int MAXN = 150005;

/* letter trie */
static int lchild[MAXN], lsib[MAXN], lpar[MAXN], lprob[MAXN];
static char lch[MAXN];
static int lcnt;

/* digit trie */
static int dchild[MAXN], dsib[MAXN], dbestnode[MAXN], dbestprob[MAXN];
static char dch[MAXN];
static int dcnt;

static int digitOf[26];

static void initDigits() {
    static const char *keys[8] = {"abc", "def", "ghi", "jkl",
                                  "mno", "pqrs", "tuv", "wxyz"};
    for (int k = 0; k < 8; ++k)
        for (const char *q = keys[k]; *q; ++q)
            digitOf[*q - 'a'] = k + 2;
}

static void insertWord(const char *w, int p) {
    int cur = 0;
    for (int i = 0; w[i]; ++i) {
        int c = w[i] - 'a';
        int prev = -1, ch = lchild[cur];
        while (ch != -1 && lch[ch] < c) { prev = ch; ch = lsib[ch]; }
        if (ch == -1 || lch[ch] != c) {
            int nn = lcnt++;
            lchild[nn] = -1;
            lsib[nn] = ch;
            lch[nn] = (char)c;
            lpar[nn] = cur;
            lprob[nn] = 0;
            if (prev == -1) lchild[cur] = nn; else lsib[prev] = nn;
            ch = nn;
        }
        cur = ch;
        lprob[cur] += p;
    }
}

static int digitChild(int u, int d) {
    int prev = -1, ch = dchild[u];
    while (ch != -1 && dch[ch] < d) { prev = ch; ch = dsib[ch]; }
    if (ch != -1 && dch[ch] == d) return ch;
    int nn = dcnt++;
    dchild[nn] = -1;
    dsib[nn] = ch;
    dch[nn] = (char)d;
    dbestprob[nn] = -1;
    dbestnode[nn] = -1;
    if (prev == -1) dchild[u] = nn; else dsib[prev] = nn;
    return nn;
}

static int digitFind(int u, int d) {
    for (int ch = dchild[u]; ch != -1 && dch[ch] <= d; ch = dsib[ch])
        if (dch[ch] == d) return ch;
    return -1;
}

/* Pre-order walk with alphabetically ordered children visits the letter
   trie nodes in lexicographic order of their strings, so a strict '>'
   keeps the alphabetically first node among equal probabilities. */
static void mapToDigits(int v, int du) {
    for (int c = lchild[v]; c != -1; c = lsib[c]) {
        int u = digitChild(du, digitOf[(int)lch[c]]);
        if (lprob[c] > dbestprob[u]) {
            dbestprob[u] = lprob[c];
            dbestnode[u] = c;
        }
        mapToDigits(c, u);
    }
}

static char word[1024], line[1024], out[1024];

int main() {
    initDigits();
    int scenarios;
    if (scanf("%d", &scenarios) != 1) return 0;
    for (int s = 1; s <= scenarios; ++s) {
        lcnt = 1;
        lchild[0] = -1;
        lsib[0] = -1;
        lpar[0] = 0;
        lprob[0] = 0;
        dcnt = 1;
        dchild[0] = -1;
        dsib[0] = -1;
        dbestprob[0] = -1;
        dbestnode[0] = -1;

        int w;
        scanf("%d", &w);
        for (int i = 0; i < w; ++i) {
            int p;
            scanf("%1000s %d", word, &p);
            insertWord(word, p);
        }
        mapToDigits(0, 0);

        printf("Scenario #%d:\n", s);
        int m;
        scanf("%d", &m);
        for (int q = 0; q < m; ++q) {
            scanf("%1000s", line);
            int cur = 0;
            int dead = 0;
            for (int i = 0; line[i] && line[i] != '1'; ++i) {
                if (!dead) {
                    int nxt = digitFind(cur, line[i] - '0');
                    if (nxt == -1) dead = 1; else cur = nxt;
                }
                if (dead) {
                    printf("MANUALLY\n");
                } else {
                    int len = 0;
                    for (int x = dbestnode[cur]; x != 0; x = lpar[x])
                        out[len++] = (char)('a' + lch[x]);
                    for (int a = 0, b = len - 1; a < b; ++a, --b) {
                        char t = out[a]; out[a] = out[b]; out[b] = t;
                    }
                    out[len] = '\0';
                    printf("%s\n", out);
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
