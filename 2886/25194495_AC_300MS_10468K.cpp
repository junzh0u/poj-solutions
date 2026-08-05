// POJ 2886 - Who Gets the Most Candies?
//
// Two independent halves.
//
// 1. Elimination order (Josephus with a signed, variable jump).
//    A Fenwick tree over 1..N holds 1 for every child still in the circle, so
//    the k-th survivor is found by descending the tree in O(log N).  Keep the
//    rank `pos` of the child about to leave.  After removing it and letting
//    m be the number left, everything behind it shifts down one rank, so the
//    child at rank `pos` is exactly the 1st one "to the left" and the child at
//    rank `pos-1` the 1st "to the right".  Hence
//        A > 0  ->  pos = ((pos + A - 2) mod m) + 1
//        A < 0  ->  pos = ((pos + A - 1) mod m) + 1
//    with a mathematical (non-negative) mod — the two signs wrap differently,
//    off by one, which is the whole subtlety here.  |A| <= 1e8 fits an int
//    alongside pos, so no pre-reduction is needed; only the sign of C's %
//    has to be repaired.
//
// 2. Who to stop at.  F(p) is the divisor count of p, independent of the
//    circle, so a sieve (for i: for j = i, 2i, ... : d[j]++) up to the maximum
//    N gives every F(p) once at startup, and the winner is the first p <= N
//    attaining the maximum d.  The simulation therefore only has to run that
//    many rounds, not all N.  d(p) <= 200 for p <= 500000, so a byte per
//    entry is enough.
//
// Ambiguity in the statement: "left"/"right" are never tied to the clockwise
// numbering.  The sample settles it — positive A walks in the direction of
// increasing index (clockwise), negative A walks back — and it exercises both
// signs, so the derivation above is checked rather than guessed.  The counting
// also starts at the child adjacent to the one who just left, i.e. the "1st to
// the left" is the immediate successor, not the departed child itself.
//
// Ties in F(p) go to whoever leaves first, which is why the first maximal p is
// taken.  The input holds several test cases and runs to ~10 MB, so reading
// goes through a fread buffer.
#include <cstdio>
#include <cstring>

static const int MAXN = 500005;

static int n, K;
static int bitree[MAXN];
static int card[MAXN];
static char nm[MAXN][12];
static unsigned char dv[MAXN];
static int LOGP;

static void bitAdd(int i, int v) {
    for (; i <= n; i += i & (-i)) bitree[i] += v;
}

/* index of the k-th still-present child (1-based rank) */
static int kth(int k) {
    int pos = 0;
    for (int pw = LOGP; pw; pw >>= 1) {
        int np = pos + pw;
        if (np <= n && bitree[np] < k) { pos = np; k -= bitree[np]; }
    }
    return pos + 1;
}

/* ---- fast input ---- */
static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;
static inline int gc() {
    if (ipos == ilen) { ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin); ipos = 0; if (ilen <= 0) return -1; }
    return ibuf[ipos++];
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
static bool readWord(char *dst) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    int i = 0;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        if (i < 11) dst[i++] = (char)c;
        c = gc();
    }
    dst[i] = '\0';
    return true;
}

int main() {
    /* divisor counts once, for every p up to the maximum N */
    {
        int lim = MAXN - 5;
        for (int i = 1; i <= lim; i++)
            for (int j = i; j <= lim; j += i)
                dv[j]++;
    }

    while (readInt(n)) {
        if (!readInt(K)) break;
        for (int i = 1; i <= n; i++) {
            readWord(nm[i]);
            readInt(card[i]);
        }

        /* Fenwick preloaded with all ones */
        for (int i = 1; i <= n; i++) bitree[i] = i & (-i);
        LOGP = 1;
        while ((LOGP << 1) <= n) LOGP <<= 1;

        /* the round with the most divisors, earliest on ties */
        int best = 1;
        for (int p = 2; p <= n; p++)
            if (dv[p] > dv[best]) best = p;

        int m = n, pos = K, ans = K;
        for (int p = 1; p <= best; p++) {
            int idx = kth(pos);
            if (p == best) { ans = idx; break; }
            bitAdd(idx, -1);
            m--;
            int a = card[idx];
            int t = (a > 0) ? (pos + a - 2) % m : (pos + a - 1) % m;
            if (t < 0) t += m;
            pos = t + 1;
        }

        printf("%s %d\n", nm[ans], (int)dv[best]);
    }
    return 0;
}
