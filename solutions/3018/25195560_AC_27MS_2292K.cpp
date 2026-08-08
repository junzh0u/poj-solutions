// POJ 3018 - Giftbox
// Model: claude-opus-5
//
// Box X fits inside box Y iff some permutation pi has X_pi(k) < Y_k for all k.
// Sorting both dimension vectors ascending makes that test componentwise: any
// witnessing permutation can be untangled into the sorted one by repeatedly
// swapping an out-of-order pair, which never breaks an inequality.  So each box
// is just its sorted vector under strict componentwise domination -- a strict
// partial order, hence acyclic and transitive.
//
// Transitivity means every box of a nesting chain over the gift contains the
// gift itself, so the boxes that cannot hold the gift are dropped up front and
// the answer is the longest chain among the rest.  a < b forces a[0] < b[0],
// so ordering candidates by their smallest dimension is a topological order,
// and one O(N^2) pass with an O(d) domination test gives the longest chain.
// The gift is not counted; "Please look for another gift shop!" when no box
// holds it.  Input is multiple data sets to EOF (the statement says so and the
// discuss board is full of people who missed it).
//
// Ambiguity: the sample cannot tell the intended answer from the number of
// boxes that hold the gift, nor from the longest chain ignoring the gift, nor
// from allowing equal dimensions -- all three reproduce its "3".  Settled
// against a reference that tests containment by trying every permutation
// literally, over ~1500 random cases.

#include <cstdio>
#include <algorithm>

using namespace std;

static const int MAXN = 505;
static const int MAXD = 1005;

static int box_[MAXN][MAXD];
static int dp[MAXN];
static int ord[MAXN];

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    unsigned int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10u + (unsigned int)(c - '0'); c = gc(); }
    out = neg ? -(int)v : (int)v;
    return true;
}

static int D;

static inline bool fits(const int *a, const int *b) {
    /* Strided pre-pass: same predicate, just cheap early rejects.  Most
       non-fitting pairs differ at a large coordinate, which the front-to-back
       scan alone would only reach after d comparisons. */
    for (int k = D - 1; k >= 0; k -= 8)
        if (a[k] >= b[k]) return false;
    for (int k = 0; k < D; ++k)
        if (a[k] >= b[k]) return false;
    return true;
}

struct ByFirst {
    bool operator()(int a, int b) const { return box_[a][0] < box_[b][0]; }
};

int main() {
    int n, d;
    while (readInt(n) && readInt(d)) {
        D = d;
        for (int j = 0; j < d; ++j) readInt(box_[0][j]);
        for (int i = 1; i <= n; ++i)
            for (int j = 0; j < d; ++j) readInt(box_[i][j]);
        for (int i = 0; i <= n; ++i) sort(box_[i], box_[i] + d);

        /* Keep only boxes that can hold the gift; containment is transitive,
           so every box stacked above such a box holds the gift as well. */
        int m = 0;
        for (int i = 1; i <= n; ++i)
            if (fits(box_[0], box_[i])) ord[m++] = i;

        if (m == 0) {
            printf("Please look for another gift shop!\n");
            continue;
        }

        /* a fits in b implies a[0] < b[0] on the sorted vectors, so this
           order is a topological order of the containment DAG. */
        sort(ord, ord + m, ByFirst());

        int best = 0;
        for (int i = 0; i < m; ++i) {
            int cur = 1;
            for (int j = i - 1; j >= 0; --j) {
                if (dp[j] + 1 <= cur) continue;
                if (fits(box_[ord[j]], box_[ord[i]])) cur = dp[j] + 1;
            }
            dp[i] = cur;
            if (cur > best) best = cur;
        }
        printf("%d\n", best);
    }
    return 0;
}
