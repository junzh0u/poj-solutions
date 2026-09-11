// POJ 3333 - Co-workers from Hell
// Model: claude-fable-5-1
// Reformulation: the total time equals sum(d) plus, for every trick that
// actually fires, a fixed weight w[c] = td[c] + sum(d[tc..c-1]) for a
// backward trick (tc <= c) or td[c] - sum(d[c..tc-1]) for a forward one,
// independent of the order in which tricks fire (flow conservation on the
// watchman's walk).  Choosing tricks lazily, the walk is a game on the set of
// still-unvisited chambers ("free set"): from pointer p pick a free c >= p,
// kill [p, c], jump to tc[c].  Exact memoized branch-and-bound over
// (pointer, free set) with the free set as a 128-bit mask.
// Statement reading: a trick planted in the final chamber n fires on the
// first visit like any other; the watchman leaves only when he checks n
// without a fresh trick (required to reproduce the sample's answer 10).
// n = 0 prints 0.
#include <cstdio>
#include <cstring>
#include <iostream>
typedef long long ll;
typedef unsigned long long ull;

int n;
ll d[128], td[128], w[128];
int tc[128];

// Memo entry packed into 24 bytes: the second mask word only needs 37 bits,
// so its upper bits carry the pointer, the generation stamp and the exact flag.
struct Entry { ull a, b; ll val; };
static const int HBITS = 20;
static const int HSIZE = 1 << HBITS;
static const ull BMASK = (1ULL << 38) - 1;
static Entry table[HSIZE];
static int tableCount;
static unsigned curGen = 0;
static ll globalBest;
static bool lastExact;

static inline bool freeb(ull a, ull b, int i) { return i < 64 ? ((a >> i) & 1ULL) != 0 : ((b >> (i - 64)) & 1ULL) != 0; }

static inline unsigned hashKey(int p, ull a, ull b) {
    ull h = a * 0x9E3779B97F4A7C15ULL ^ (b + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL ^ ((ull)p * 0x165667B19E3779F9ULL);
    h ^= h >> 29; h *= 0xBF58476D1CE4E5B9ULL; h ^= h >> 32;
    return (unsigned)(h & (HSIZE - 1));
}

static inline ull packKey(int p, ull b) { return (b & BMASK) | ((ull)p << 40) | ((ull)(curGen & 0xFFFF) << 48); }
static inline bool entryExact(const Entry* e) { return (e->b >> 39) & 1ULL; }
static inline void setExact(Entry* e) { e->b |= 1ULL << 39; }

static Entry* lookup(int p, ull a, ull b, bool create) {
    unsigned h = hashKey(p, a, b);
    ull key = packKey(p, b);
    for (;;) {
        Entry* e = &table[h];
        ull g = e->b >> 48;
        if (g != (ull)(curGen & 0xFFFF)) break;   // empty slot for this generation
        if (e->a == a && (e->b & ~(1ULL << 39)) == key) return e;
        h = (h + 1) & (HSIZE - 1);
    }
    if (!create) return 0;
    if (tableCount * 10 >= HSIZE * 7) return 0;
    Entry* e = &table[h];
    e->a = a; e->b = key; e->val = 0; tableCount++;
    return e;
}

// Upper bound for state (p, free set): every free chamber below p with a
// positive weight is assumed collectable, and above p the picks are relaxed
// to an increasing chain of "record" picks where a forward skip is credited
// with every positive free weight it jumps over (rescued picks) and a
// backward pick continues from the next chamber.  Every real play maps into
// this relaxation, so it never underestimates.
static ll pre[128];
static ll hh[130];
static ll upperBound(int p, ull a, ull b) {
    // a chamber that ends up below the pointer can only be picked after some
    // other free chamber lands at or below it: credit only such chambers.
    int m1 = n + 2, m2 = n + 2, arg1 = -1;
    {
        ull x = a;
        while (x) { int y = __builtin_ctzll(x); x &= x - 1; int t = tc[y]; if (t < m1) { m2 = m1; m1 = t; arg1 = y; } else if (t < m2) m2 = t; }
        x = b;
        while (x) { int y = __builtin_ctzll(x) + 64; x &= x - 1; int t = tc[y]; if (t < m1) { m2 = m1; m1 = t; arg1 = y; } else if (t < m2) m2 = t; }
    }
    pre[0] = 0;
    for (int i = 1; i <= n; i++) {
        ll v = 0;
        if (freeb(a, b, i) && w[i] > 0 && ((i == arg1) ? m2 : m1) <= i) v = w[i];
        pre[i] = pre[i - 1] + v;
    }
    hh[n + 1] = 0; hh[n + 2] = 0;
    for (int x = n; x >= p; x--) {
        ll best = hh[x + 1];
        if (freeb(a, b, x)) {
            ll v = w[x];
            if (tc[x] > x) { v += pre[tc[x] - 1] - pre[x]; v += hh[tc[x] > n + 1 ? n + 1 : tc[x]]; }
            else v += hh[x + 1];
            if (v > best) best = v;
        }
        hh[x] = best;
    }
    return pre[p - 1] + hh[p];
}

// Simulation of a fixed trick set (bit i of mask words = chamber i tricked),
// returns the total time minus sum(d), i.e. the sum of fired weights.
static ll simulate(const ull* ma, const ull* mb) {
    static char vis[128];
    memset(vis, 0, sizeof(vis));
    ll t = 0; int cur = 1;
    while (cur >= 1 && cur <= n) {
        bool trick = !vis[cur] && (cur < 64 ? ((*ma >> cur) & 1ULL) : ((*mb >> (cur - 64)) & 1ULL));
        vis[cur] = 1;
        if (trick) { t += w[cur]; cur = tc[cur]; }
        else { if (cur == n) break; cur++; }
    }
    return t;
}

// Normalisation: a free chamber x below the pointer can only be picked after
// some other free chamber y jumps to tc[y] <= x.  Chambers with no such
// rescuer are unreachable for the rest of the play and are dropped from the
// free set (iterated to a fixpoint), which merges equivalent states and
// tightens the bound.
static void normalize(int p, ull& a, ull& b) {
    if (p <= 1) return;
    ull lowA = p >= 64 ? a : (a & ((1ULL << p) - 1));
    ull lowB = p >= 64 ? (b & ((1ULL << (p - 64)) - 1)) : 0;
    if (!(lowA | lowB)) return;
    for (;;) {
        int m1 = n + 2, m2 = n + 2, arg1 = -1;
        ull x = a;
        while (x) { int y = __builtin_ctzll(x); x &= x - 1; int t = tc[y]; if (t < m1) { m2 = m1; m1 = t; arg1 = y; } else if (t < m2) m2 = t; }
        x = b;
        while (x) { int y = __builtin_ctzll(x) + 64; x &= x - 1; int t = tc[y]; if (t < m1) { m2 = m1; m1 = t; arg1 = y; } else if (t < m2) m2 = t; }
        bool changed = false;
        x = lowA;
        while (x) { int y = __builtin_ctzll(x); x &= x - 1; int m = (y == arg1) ? m2 : m1; if (m > y) { a &= ~(1ULL << y); lowA &= ~(1ULL << y); changed = true; } }
        x = lowB;
        while (x) { int y = __builtin_ctzll(x) + 64; x &= x - 1; int m = (y == arg1) ? m2 : m1; if (m > y) { b &= ~(1ULL << (y - 64)); lowB &= ~(1ULL << (y - 64)); changed = true; } }
        if (!changed) return;
    }
}

// Best additional value from (p, free set) given acc so far.  Sets lastExact
// when the returned value is the true optimum of the state; otherwise the
// state cannot beat globalBest and the return value is only a lower bound.
static ll solve(int p, ull a, ull b, ll acc) {
    while (p <= n && !freeb(a, b, p)) p++;
    if (acc > globalBest) globalBest = acc;
    if (p > n) { lastExact = true; return 0; }
    normalize(p, a, b);
    Entry* e = lookup(p, a, b, false);
    ll hi;
    if (e) {
        if (entryExact(e)) { lastExact = true; if (acc + e->val > globalBest) globalBest = acc + e->val; return e->val; }
        hi = e->val;
    } else {
        hi = upperBound(p, a, b);
        e = lookup(p, a, b, true);
        if (e) e->val = hi;
    }
    if (acc + hi <= globalBest) { lastExact = false; return 0; }
    ll best = 0;
    bool exact = true;
    // candidates in decreasing weight order (good first solutions prune more);
    // killMask[k] is the free set after picking cand[k] from p.
    int cand[128]; ull ka[128], kb[128]; int nc = 0;
    {
        ull a2 = a, b2 = b;
        for (int c = p; c <= n; c++) {
            if (c < 64) a2 &= ~(1ULL << c); else b2 &= ~(1ULL << (c - 64));
            if (!freeb(a, b, c)) continue;
            int j = nc++;
            while (j > 0 && w[cand[j - 1]] < w[c]) { cand[j] = cand[j - 1]; ka[j] = ka[j - 1]; kb[j] = kb[j - 1]; j--; }
            cand[j] = c; ka[j] = a2; kb[j] = b2;
        }
    }
    for (int k = 0; k < nc; k++) {
        int c = cand[k];
        ll child = w[c] + solve(tc[c], ka[k], kb[k], acc + w[c]);
        if (!lastExact) exact = false;
        if (child > best) best = child;
    }
    if (e) {  // entries are never evicted within a test case
        if (exact) { e->val = best; setExact(e); }
        else { ll ub = globalBest - acc; if (ub < e->val) e->val = ub; if (e->val < best) e->val = best; }
    }
    lastExact = exact;
    return best;
}

int main() {
    int T;
    if (!(std::cin >> T)) return 0;
    while (T--) {
        if (!(std::cin >> n)) break;
        ll D = 0;
        for (int i = 1; i <= n; i++) {
            std::cin >> d[i] >> td[i] >> tc[i];
            if (tc[i] < 1) tc[i] = 1;
            if (tc[i] > n + 1) tc[i] = n + 1;
            D += d[i];
        }
        if (n <= 0) { std::cout << 0 << std::endl; continue; }
        for (int i = 1; i <= n; i++) {
            ll s = 0;
            if (tc[i] <= i) { for (int j = tc[i]; j < i; j++) s += d[j]; w[i] = td[i] + s; }
            else { for (int j = i; j < tc[i]; j++) s += d[j]; w[i] = td[i] - s; }
        }
        curGen++; if ((curGen & 0xFFFF) == 0) curGen++; tableCount = 0; globalBest = 0;
        {
            // initial lower bounds: all backward tricks; all backward plus
            // every positive-weight forward trick
            ull ma = 0, mb = 0, na = 0, nb = 0;
            for (int i = 1; i <= n; i++) {
                bool back = tc[i] <= i, pos = w[i] > 0;
                if (back) { if (i < 64) ma |= 1ULL << i; else mb |= 1ULL << (i - 64); }
                if (back || pos) { if (i < 64) na |= 1ULL << i; else nb |= 1ULL << (i - 64); }
            }
            ll v1 = simulate(&ma, &mb), v2 = simulate(&na, &nb);
            if (v1 > globalBest) globalBest = v1;
            if (v2 > globalBest) globalBest = v2;
        }
        ull a = 0, b = 0;
        for (int i = 1; i <= n; i++) { if (i < 64) a |= 1ULL << i; else b |= 1ULL << (i - 64); }
        solve(1, a, b, 0);
        std::cout << D + globalBest << std::endl;
    }
    return 0;
}
