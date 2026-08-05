// POJ 2828 - Buy Tickets
//
// N people join a queue one at a time; person i pushes in so that they end up
// directly behind the Pos_i-th person, i.e. at 0-based index Pos_i of the queue
// as it stands at that moment.  Output the final values in queue order.
// Several test cases run to EOF, N <= 200,000 each.
//
// Inserting forwards is O(n) per person.  The trick is to process the people in
// REVERSE order: the last person to arrive never gets displaced again, so their
// Pos_N is already their final index.  Peeling people off from the back, when
// person i is placed every person who arrived after them has been removed, so
// among the slots still free person i takes the (Pos_i + 1)-th one -- and that
// slot is final.
//
// A Fenwick tree over the n slots holds 1 for free and 0 for taken, and the
// k-th free slot is found by descending the tree with binary lifting, so each
// placement is a single O(log n) walk rather than a search plus a query.  The
// tree is seeded in O(n) (bit[i] = i & -i is exactly the all-ones prefix sum)
// since it must be rebuilt for every test case.
//
// Input runs to ~12 MB in the worst case, so parsing is a hand-rolled fread
// scanner and output goes through a write buffer; the whole thing is whitespace
// agnostic, which matters because the statement does not promise a fixed layout.
//
// Statement notes: "right behind the Pos_i-th person" with the ticket office as
// person 0 is what makes Pos_i the 0-based insertion index -- Pos_i = 0 means
// jumping to the very front, ahead of everyone.  The stated range
// Pos_i in [0, i-1] guarantees the index is always valid, so no clamping is
// needed.  Values are only in [0, 32767], but a queue may legitimately contain
// duplicates, so nothing may be deduplicated.

#include <cstdio>
#include <cstring>

static const int MAXN = 200005;

static int bit[MAXN];
static int n, LOG;
static int pos_[MAXN], val_[MAXN], ans[MAXN];

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
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
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

static char obuf[1 << 16];
static int olen = 0;

static void flushOut() {
    fwrite(obuf, 1, olen, stdout);
    olen = 0;
}

static void putCh(char c) {
    if (olen == (int)sizeof(obuf)) flushOut();
    obuf[olen++] = c;
}

static void putInt(int x) {
    char t[12];
    int k = 0;
    if (x == 0) t[k++] = '0';
    while (x > 0) { t[k++] = (char)('0' + x % 10); x /= 10; }
    while (k > 0) putCh(t[--k]);
}

static void add(int i, int d) {
    for (; i <= n; i += i & (-i)) bit[i] += d;
}

/* Find smallest index p with prefix_sum(p) == k (k-th free slot). */
static int kth(int k) {
    int p = 0;
    for (int step = LOG; step > 0; step >>= 1) {
        if (p + step <= n && bit[p + step] < k) {
            p += step;
            k -= bit[p];
        }
    }
    return p + 1;
}

int main() {
    while (readInt(n)) {
        int i;
        for (i = 1; i <= n; ++i) {
            readInt(pos_[i]);
            readInt(val_[i]);
        }
        /* BIT initialised with 1 at every position, built in O(n). */
        for (i = 1; i <= n; ++i) bit[i] = i & (-i);
        LOG = 1;
        while ((LOG << 1) <= n) LOG <<= 1;

        for (i = n; i >= 1; --i) {
            int p = kth(pos_[i] + 1);
            ans[p] = val_[i];
            add(p, -1);
        }
        for (i = 1; i <= n; ++i) {
            if (i > 1) putCh(' ');
            putInt(ans[i]);
        }
        putCh('\n');
    }
    flushOut();
    return 0;
}
