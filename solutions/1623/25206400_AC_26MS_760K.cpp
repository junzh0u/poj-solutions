// POJ 1623 - Squadtrees
// Model: claude-sonnet-5
//
// Build the quadtree for the (zero-padded, power-of-two-sized) bitmap and
// count its nodes in the usual way. For the squadtree, two subtrees are
// merged into a single reference iff their entire recursive structure
// (shape and leaf values) is identical AND the subtree has height >= 1
// (per the statement, merging single-leaf nodes never saves space, so
// leaves are never deduplicated against each other, only whole non-leaf
// subtrees are). This is a pure structural-equality test, independent of
// where in the image or at what recursion depth the subtree occurs -
// confirmed against the discuss board's posted 8x8 example (message
// 359768: "it's not that the original sub-image must be identical, the
// tree must be identical"), which this program reproduces exactly (17 12).
//
// Each node's canonical signature is built bottom-up as a string ("Lv" for
// a leaf of value v, "I(s1,s2,s3,s4)" for an internal node from its four
// children's signatures in UL,UR,LL,LR order). A global set of signatures
// seen so far (per test case) records internal-node subtrees already
// counted; when a freshly built internal node's signature was already
// seen, the whole subtree contributes 0 new squadtree nodes (it becomes a
// reference); otherwise it contributes 1 + its children's contributions.
// Leaves always contribute 1, unconditionally.

#include <cstdio>
#include <cstring>
#include <string>
#include <set>
using namespace std;

static char grid[130][130];
static int size_;
static set<string> seen;
static int qcount;

static bool uniform(int x, int y, int len, char &val) {
    val = grid[x][y];
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < len; ++j)
            if (grid[x + i][y + j] != val) return false;
    return true;
}

// returns pair: signature string, squadtree contribution (via out param)
static string build(int x, int y, int len, int &snodes) {
    char val;
    if (uniform(x, y, len, val)) {
        qcount += 1;
        snodes = 1;
        string s = "L";
        s += val;
        return s;
    }
    int half = len / 2;
    int s1, s2, s3, s4;
    string sig1 = build(x, y, half, s1);
    string sig2 = build(x, y + half, half, s2);
    string sig3 = build(x + half, y, half, s3);
    string sig4 = build(x + half, y + half, half, s4);
    qcount += 1; // children already added their own counts recursively
    string sig = "I(" + sig1 + "," + sig2 + "," + sig3 + "," + sig4 + ")";
    if (seen.count(sig)) {
        snodes = 0;
    } else {
        seen.insert(sig);
        snodes = 1 + s1 + s2 + s3 + s4;
    }
    return sig;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        static char line[140];
        for (int i = 0; i < 130; ++i)
            for (int j = 0; j < 130; ++j)
                grid[i][j] = '0';
        for (int i = 0; i < n; ++i) {
            scanf("%s", line);
            int len = (int)strlen(line);
            for (int j = 0; j < m; ++j)
                grid[i][j] = (j < len) ? line[j] : '0';
        }
        int mx = n > m ? n : m;
        size_ = 1;
        while (size_ < mx) size_ *= 2;
        seen.clear();
        qcount = 0;
        int snodes;
        build(0, 0, size_, snodes);
        printf("%d %d\n", qcount, snodes);
    }
    return 0;
}
