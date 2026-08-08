// POJ 2274 - The Race
// Model: claude-opus-5
//
// Ship i (behind) passes ship j (ahead) exactly when X_i < X_j and V_i > V_j,
// and exactly once, so the total number of passings is the number of inversions
// of the velocity sequence -- counted here with a Fenwick tree over velocities
// and accumulated mod 1e6, since the true total reaches ~3e10.
//
// For the first 10000 passings, the key fact is that the earliest remaining
// passing is always between two ships that are currently adjacent on the
// course: anything sitting between two ships that are about to meet would have
// to cross one of them strictly earlier.  So keep the current course order in
// an array and a min-heap of the crossing events of adjacent pairs.  Pop the
// earliest, print it, swap the two ships, and push the (at most two) events
// created by the two new adjacencies.  Only 10000 events are ever processed, so
// the heap stays at O(N) regardless of how many passings there really are.
//
// Events of pairs that stop being adjacent are left in the heap and discarded
// on pop via slot_[a] + 1 == slot_[b]; such a pair is always re-pushed when it
// becomes adjacent again, which necessarily happens strictly before it crosses.
// After a pair has crossed the check fails forever, which also discards the
// duplicate copy that re-pushing can create.
//
// All comparisons are exact.  For a pair the crossing time is dx/dv and the
// crossing position is (V_a*X_b - V_b*X_a)/dv with dx, dv > 0, so ordering two
// events is a cross-multiplication of integers below ~1e10 -- held exactly by
// double's 53-bit mantissa, which keeps the program free of long long and of
// any epsilon.  The discuss board's repeated "double comparison" bugs come from
// dividing instead.
//
// Ambiguity: the statement guarantees at most two ships ever share a position,
// so simultaneous passings always differ in position and the "closer to the
// starting line first" rule totally orders them.  Two board posts nevertheless
// suspect the data contains three ships meeting at one point, where the
// statement fixes no order; the comparator falls back to ship index there, so
// such a moment is emitted as a deterministic left-to-right bubble pass.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

static const int MAXN = 250005;
static const int MAXOUT = 10000;
static const int MOD = 1000000;

static int X[MAXN], V[MAXN];
static int arr_[MAXN], slot_[MAXN], ord_[MAXN];

static bool byX(int p, int q) { return X[p] < X[q]; }

struct Ev { int a, b, dx, dv; double pn; };
static Ev heap_[MAXN + 4 * MAXOUT + 10];
static int hsz;

static inline bool earlier(const Ev &e1, const Ev &e2) {
    double l = (double)e1.dx * (double)e2.dv;
    double r = (double)e2.dx * (double)e1.dv;
    if (l != r) return l < r;
    double lp = e1.pn * (double)e2.dv;
    double rp = e2.pn * (double)e1.dv;
    if (lp != rp) return lp < rp;
    /* Same time and same place: the statement says this never happens (never
       more than two ships at one position).  Break the tie deterministically
       anyway, leftmost swap first, so the degenerate case is a fixed bubble
       pass rather than whatever order the heap happens to hold. */
    if (e1.a != e2.a) return e1.a < e2.a;
    return e1.b < e2.b;
}

static inline void hpush(int a, int b) {
    Ev e;
    e.a = a; e.b = b;
    e.dx = X[b] - X[a];
    e.dv = V[a] - V[b];
    e.pn = (double)V[a] * (double)X[b] - (double)V[b] * (double)X[a];
    int i = hsz++;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (earlier(e, heap_[p])) { heap_[i] = heap_[p]; i = p; }
        else break;
    }
    heap_[i] = e;
}

static inline Ev hpop() {
    Ev top = heap_[0];
    Ev e = heap_[--hsz];
    int i = 0;
    for (;;) {
        int c = 2 * i + 1;
        if (c >= hsz) break;
        if (c + 1 < hsz && earlier(heap_[c + 1], heap_[c])) c++;
        if (earlier(heap_[c], e)) { heap_[i] = heap_[c]; i = c; }
        else break;
    }
    heap_[i] = e;
    return top;
}

/* Fenwick over velocity values, for counting inversions. */
static const int MAXV = 100002;
static int bit_[MAXV + 1];

static inline void bupd(int v) { for (int i = v + 1; i <= MAXV; i += i & (-i)) bit_[i]++; }
static inline int bqry(int v) { int s = 0; for (int i = v + 1; i > 0; i -= i & (-i)) s += bit_[i]; return s; }

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;
static inline int gc() {
    if (ipos == ilen) { ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin); ipos = 0; if (ilen <= 0) return -1; }
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

int main() {
    int n;
    while (readInt(n)) {
        if (n <= 0) break;
        int i;
        for (i = 0; i < n; i++) { readInt(X[i]); readInt(V[i]); }

        /* The statement says X is already increasing; sort defensively so a
           file that violates it still gets the right initial course order. */
        for (i = 0; i < n; i++) ord_[i] = i;
        std::stable_sort(ord_, ord_ + n, byX);

        /* Total passes = pairs (i before j on the course) with V[i] > V[j],
           modulo 1e6. */
        for (i = 0; i <= MAXV; i++) bit_[i] = 0;
        int inv = 0;
        for (i = n - 1; i >= 0; i--) {
            int v = V[ord_[i]];
            if (v < 0) v = 0;
            if (v > MAXV - 1) v = MAXV - 1;
            if (v > 0) inv = (inv + bqry(v - 1)) % MOD;
            bupd(v);
        }
        printf("%d\n", inv);

        /* Kinetic simulation: the earliest remaining pass is always between two
           ships currently adjacent on the course. */
        for (i = 0; i < n; i++) { arr_[i] = ord_[i]; slot_[ord_[i]] = i; }
        hsz = 0;
        for (i = 0; i + 1 < n; i++)
            if (V[arr_[i]] > V[arr_[i + 1]]) hpush(arr_[i], arr_[i + 1]);

        int out = 0;
        while (hsz > 0 && out < MAXOUT) {
            Ev e = hpop();
            int a = e.a, b = e.b;
            if (slot_[a] + 1 != slot_[b]) continue; /* stale */
            printf("%d %d\n", a + 1, b + 1);
            out++;
            int k = slot_[a];
            arr_[k] = b; arr_[k + 1] = a; slot_[b] = k; slot_[a] = k + 1;
            if (k > 0) { int p = arr_[k - 1]; if (V[p] > V[b]) hpush(p, b); }
            if (k + 2 < n) { int q = arr_[k + 2]; if (V[a] > V[q]) hpush(a, q); }
        }
    }
    return 0;
}
