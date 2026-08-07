// POJ 3581 - Sequence
// Model: claude-opus-5
//
// Cut A[1..n] into three non-empty pieces, reverse each, concatenate; minimise
// lexicographically.  A[1] is guaranteed strictly greater than every other
// element (the discuss board is emphatic that inputs violating this - 5 0 5 0 2 3
// and friends - are illegal data, and the whole greedy below rests on it).
//
// Work on b = reverse(A).  Then reverse(A[1..i]) is exactly the suffix of b of
// length i, so the first piece is a suffix of b.
//
// First cut.  Because b[n] = A[1] is the unique maximum, no suffix of b is a
// prefix of another, so any two candidate first pieces already differ inside the
// shorter one: the choice is decided without looking at the tail, and the best i
// is simply the smallest suffix of b.  At least two elements must be left over,
// i.e. the suffix must start at index >= 2 (0-based), which is just the smallest
// suffix of b[2..n-1] - the last factor of its Lyndon factorization (Duval, O(n)).
//
// Second cut.  Let e = b[0..m-1] be what is left (e is the reverse of the
// remaining part of A).  Splitting it and reversing both halves yields
// e[p..m-1] ++ e[0..p-1] - a *rotation* of e - with p in [1, m-1]; p = 0 is
// excluded because the third piece must be non-empty.  So the tail is the
// smallest rotation of e other than e itself.
//
// The usual O(m) two-pointer minimum-representation cannot be restricted to
// p >= 1: its elimination step compares candidate i+t against j+t, and j+t wraps
// onto the forbidden position 0 (e = 0 1 1 1 is the smallest counterexample - it
// answers p = 2 where the truth is p = 3).  Instead build the suffix array of
// V = e[1..m-1] ++ e[0..m-1] ++ sentinel: rotation p is the length-m window of V
// at index p-1, and windows of equal length compare like their suffixes unless
// they tie, in which case the two outputs are identical anyway.  Then take the
// candidate index in [0, m-2] of least rank.
//
// Values are only ever compared, so they are radix-sorted to ranks first (also
// keeps the suffix array's counting sort on a small alphabet, and dodges the
// board's reports of std::sort being too slow here).  Exactly ONE test case per
// input - the board is unanimous that looping to EOF gets Wrong Answer.

#include <cstdio>

static const int MAXN = 200005;
static const int MAXL = 2 * MAXN + 5;

static int a[MAXN], b[MAXN], cb[MAXN];
static int outv[MAXN], outn;

/* ---------- value compression (LSD radix sort, no std::sort) ---------- */
static int ord[MAXN], ord2[MAXN], bucket[256];

static void compress(const int *src, int len, int *dst) {
    int i, pass;
    for (i = 0; i < len; i++) ord[i] = i;
    int *cur = ord, *nxt = ord2;
    for (pass = 0; pass < 4; pass++) {
        int shift = pass * 8;
        for (i = 0; i < 256; i++) bucket[i] = 0;
        for (i = 0; i < len; i++) {
            unsigned int key = (unsigned int)src[cur[i]] ^ 0x80000000u;
            bucket[(key >> shift) & 255u]++;
        }
        for (i = 1; i < 256; i++) bucket[i] += bucket[i - 1];
        for (i = len - 1; i >= 0; i--) {
            unsigned int key = (unsigned int)src[cur[i]] ^ 0x80000000u;
            nxt[--bucket[(key >> shift) & 255u]] = cur[i];
        }
        int *t = cur; cur = nxt; nxt = t;
    }
    int r = 1;
    dst[cur[0]] = r;
    for (i = 1; i < len; i++) {
        if (src[cur[i]] != src[cur[i - 1]]) r++;
        dst[cur[i]] = r;
    }
}

/* ---------- smallest suffix of s[0..L-1]: start of the last Lyndon factor ---------- */
static int minSuffixStart(const int *s, int L) {
    int i = 0, last = 0;
    while (i < L) {
        int j = i + 1, k = i;
        while (j < L && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i; else k++;
            j++;
        }
        while (i <= k) { last = i; i += j - k; }
    }
    return last;
}

/* ---------- suffix array (doubling + counting sort) ---------- */
static int sa[MAXL], rk[MAXL], rk2[MAXL], sa2[MAXL], cnt[MAXL], str[MAXL];

static void buildSA(int L, int K) {   /* str[0..L-1] in [0,K], str[L-1] unique min */
    int i, k, r, M = K + 1;
    for (i = 0; i < M; i++) cnt[i] = 0;
    for (i = 0; i < L; i++) cnt[str[i]]++;
    for (i = 1; i < M; i++) cnt[i] += cnt[i - 1];
    for (i = L - 1; i >= 0; i--) sa[--cnt[str[i]]] = i;
    rk[sa[0]] = 0; r = 0;
    for (i = 1; i < L; i++) {
        if (str[sa[i]] != str[sa[i - 1]]) r++;
        rk[sa[i]] = r;
    }
    for (k = 1; r + 1 < L; k <<= 1) {
        int p = 0;
        for (i = L - k; i < L; i++) sa2[p++] = i;
        for (i = 0; i < L; i++) if (sa[i] >= k) sa2[p++] = sa[i] - k;
        M = r + 1;
        for (i = 0; i < M; i++) cnt[i] = 0;
        for (i = 0; i < L; i++) cnt[rk[i]]++;
        for (i = 1; i < M; i++) cnt[i] += cnt[i - 1];
        for (i = L - 1; i >= 0; i--) sa[--cnt[rk[sa2[i]]]] = sa2[i];
        rk2[sa[0]] = 0; r = 0;
        for (i = 1; i < L; i++) {
            int x = sa[i], y = sa[i - 1];
            int xk = x + k < L ? rk[x + k] : -1;
            int yk = y + k < L ? rk[y + k] : -1;
            if (rk[x] != rk[y] || xk != yk) r++;
            rk2[x] = r;
        }
        for (i = 0; i < L; i++) rk[i] = rk2[i];
    }
}

static void computeAnswer(int n) {
    int i;
    outn = 0;
    if (n < 3) { for (i = 0; i < n; i++) outv[outn++] = a[i]; return; }
    for (i = 0; i < n; i++) b[i] = a[n - 1 - i];
    compress(b, n, cb);

    /* first cut: smallest suffix of b that leaves at least two elements in front */
    int kb = 2 + minSuffixStart(cb + 2, n - 2);
    for (i = kb; i < n; i++) outv[outn++] = b[i];

    /* second cut: smallest rotation of e = b[0..m-1] starting at p in [1, m-1] */
    int m = kb, p;
    if (m == 2) {
        p = 1;
    } else {
        int K = 0, L = 2 * m;
        /* str = e[1..m-1] ++ e[0..m-1] ++ sentinel 0 */
        for (i = 0; i < m - 1; i++) { str[i] = cb[i + 1]; if (str[i] > K) K = str[i]; }
        for (i = 0; i < m; i++) { str[m - 1 + i] = cb[i]; if (cb[i] > K) K = cb[i]; }
        str[L - 1] = 0;
        buildSA(L, K);
        p = 1;
        int bestRank = rk[0];
        for (i = 1; i <= m - 2; i++)
            if (rk[i] < bestRank) { bestRank = rk[i]; p = i + 1; }
    }
    for (i = p; i < m; i++) outv[outn++] = b[i];
    for (i = 0; i < p; i++) outv[outn++] = b[i];
}

/* ---------- fast I/O ---------- */
static char ibuf[1 << 16];
static int ipos, ilen;
static int gc() {
    if (ipos == ilen) { ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin); ipos = 0; if (ilen <= 0) return -1; }
    return ibuf[ipos++];
}
static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    unsigned int v = 0;                       /* unsigned so that -2147483648 parses */
    while (c >= '0' && c <= '9') { v = v * 10u + (unsigned int)(c - '0'); c = gc(); }
    out = neg ? (int)((unsigned int)0 - v) : (int)v;
    return true;
}
static char obuf[1 << 16];
static int opos;
static void flushOut() { fwrite(obuf, 1, opos, stdout); opos = 0; }
static void writeInt(int v) {
    if (opos > (int)sizeof(obuf) - 16) flushOut();
    if (v < 0) obuf[opos++] = '-';
    unsigned int u = v < 0 ? (unsigned int)0 - (unsigned int)v : (unsigned int)v;
    char tmp[12];
    int t = 0;
    do { tmp[t++] = (char)('0' + u % 10); u /= 10; } while (u);
    while (t) obuf[opos++] = tmp[--t];
    obuf[opos++] = '\n';
}

int main() {
    int n;
    if (!readInt(n)) return 0;
    for (int i = 0; i < n; i++) readInt(a[i]);
    computeAnswer(n);
    for (int i = 0; i < outn; i++) writeInt(outv[i]);
    flushOut();
    return 0;
}
