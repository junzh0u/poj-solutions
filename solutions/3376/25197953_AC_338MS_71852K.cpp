// POJ 3376 - Finding Palindromes
// Model: claude-sonnet-5
//
// For n strings, count ordered pairs (i,j) (i may equal j) such that
// s_i + s_j is a palindrome (n*n candidate pairs, letting duplicate
// contents count with their own multiplicity per pair of indices).
//
// Let A=s_i (len a), B=s_j (len b). Splitting the concatenation A+B and
// matching characters symmetric about the centre gives two disjoint cases:
//   a >= b: need reverse(B) == A[0..b) and the leftover suffix A[b..a) must
//           itself be a palindrome.
//   a <  b: need reverse(A) == B[b-a..b) and the leftover prefix B[0..b-a)
//           must itself be a palindrome.
// (a==b falls only in the first case, avoiding double counting.)
//
// reverse(A[0..d)) equals the length-d suffix of reverse(A), and
// reverse(B[b-e..b)) equals the length-e prefix of reverse(B) (standard
// reverse/substring identities), so for every string X we only need, for
// every prefix and suffix length of reverse(X), the count of stored
// strings with that exact content -- answered via a hash table keyed by
// (length, double polynomial hash). "isPalindrome(prefix/suffix of X)" is
// answered for every length in O(len X) total via the Z-function trick:
// s[0:k] is a palindrome iff LCP(s, reverse(s)[len-k:]) >= k, and
// s[len-k:] is a palindrome iff LCP(reverse(s), s[len-k:]) >= k, both
// read off a Z-array of s+'#'+reverse(s) (resp. reverse(s)+'#'+s).
//
// A plain trie or std::map/unordered_map here either blows the 256MB
// memory limit or TLEs (both repeatedly reported on the discuss board for
// this problem); a flat open-addressing hash table keyed by (len, hash)
// avoids both. Board also confirms n can reach 2,000,000 (2,000,000 copies
// of "a") and that the answer needs 64-bit (up to n^2 = 4*10^12), hence
// building under G++ for long long / unsigned long long support (POJ's
// C++ (language 4) predates long long).
//
// Ambiguity: the statement doesn't bound n separately (board thread "n
// 范围多大" got no official answer); board's own stress data uses
// n=2,000,000 (all length-1 strings), consistent with only the aggregate
// length bound of 2,000,000, so all buffers are sized off n and off the
// running total length rather than off any assumed per-field bound.

#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned int u32;
typedef unsigned long long u64;
typedef long long ll;

static const u32 MOD1 = 1000000007u;
static const u32 MOD2 = 998244353u;
static const u32 BASE1 = 131u;
static const u32 BASE2 = 137u;

static char *gdata; // whole input, in memory

static inline void skipSpace(char *&p) {
    while (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t') p++;
}
static inline long parseInt(char *&p) {
    skipSpace(p);
    long x = 0;
    while (*p >= '0' && *p <= '9') { x = x * 10 + (*p - '0'); p++; }
    return x;
}

// ---- open addressing hash table: key=(len, combinedHash) -> count ----
static int tabBits;
static u64 tabMask;
static int *tabLen = NULL;
static u64 *tabKey = NULL;
static int *tabCnt = NULL;

static inline u64 mixHash(u64 h) {
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h;
}

static inline void tabInsert(int len, u64 key) {
    u64 idx = mixHash(key ^ ((u64)(u32)len * 0x9E3779B97F4A7C15ULL)) & tabMask;
    for (;;) {
        if (tabLen[idx] == -1) {
            tabLen[idx] = len;
            tabKey[idx] = key;
            tabCnt[idx] = 1;
            return;
        }
        if (tabLen[idx] == len && tabKey[idx] == key) {
            tabCnt[idx]++;
            return;
        }
        idx = (idx + 1) & tabMask;
    }
}

static inline int tabQuery(int len, u64 key) {
    u64 idx = mixHash(key ^ ((u64)(u32)len * 0x9E3779B97F4A7C15ULL)) & tabMask;
    for (;;) {
        if (tabLen[idx] == -1) return 0;
        if (tabLen[idx] == len && tabKey[idx] == key) return tabCnt[idx];
        idx = (idx + 1) & tabMask;
    }
}

// ---- Z-function ----
static void zfunction(const char *t, int n, int *z) {
    z[0] = 0;
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        int zi = 0;
        if (i < r) {
            int cand = r - i;
            int mir = z[i - l];
            zi = (mir < cand) ? mir : cand;
        }
        while (i + zi < n && t[zi] == t[i + zi]) zi++;
        z[i] = zi;
        if (i + zi > r) { l = i; r = i + zi; }
    }
}

int main() {
    FILE *f = stdin;
    // Read all of stdin without relying on it being seekable (a pipe is
    // not, and fseek/ftell silently misbehave on it), growing a buffer.
    size_t cap = 1 << 20;
    size_t len = 0;
    gdata = (char *)malloc(cap);
    for (;;) {
        if (len + (1 << 16) + 1 > cap) {
            cap *= 2;
            gdata = (char *)realloc(gdata, cap);
        }
        size_t got = fread(gdata + len, 1, 1 << 16, f);
        len += got;
        if (got < (size_t)(1 << 16)) break;
    }
    gdata[len] = 0;

    char *p = gdata;
    long n = parseInt(p);
    if (n <= 0) { printf("0\n"); return 0; }

    int *startArr = (int *)malloc(sizeof(int) * n);
    int *lenArr = (int *)malloc(sizeof(int) * n);
    int maxlen = 0;

    for (long i = 0; i < n; i++) {
        long l = parseInt(p);
        skipSpace(p);
        startArr[i] = (int)(p - gdata);
        lenArr[i] = (int)l;
        p += l;
        if ((int)l > maxlen) maxlen = (int)l;
    }
    if (maxlen < 1) maxlen = 1;

    // ---- hash table sized off n ----
    {
        u64 want = (u64)n * 2 + 16;
        tabBits = 4;
        while (((u64)1 << tabBits) < want) tabBits++;
        u64 tabSize = (u64)1 << tabBits;
        tabMask = tabSize - 1;
        tabLen = (int *)malloc(sizeof(int) * tabSize);
        tabKey = (u64 *)malloc(sizeof(u64) * tabSize);
        tabCnt = (int *)malloc(sizeof(int) * tabSize);
        for (u64 i = 0; i < tabSize; i++) tabLen[i] = -1;
    }

    // ---- pass 1: insert every string's own hash ----
    for (long i = 0; i < n; i++) {
        const char *s = gdata + startArr[i];
        int l = lenArr[i];
        u32 h1 = 0, h2 = 0;
        for (int k = 0; k < l; k++) {
            u32 c = (u32)(s[k] - 'a' + 1);
            h1 = (u32)(((u64)h1 * BASE1 + c) % MOD1);
            h2 = (u32)(((u64)h2 * BASE2 + c) % MOD2);
        }
        u64 key = (u64)h1 * MOD2 + h2;
        tabInsert(l, key);
    }

    // ---- precompute powers up to 2*maxlen+5 ----
    int powLen = 2 * maxlen + 5;
    u32 *pow1 = (u32 *)malloc(sizeof(u32) * powLen);
    u32 *pow2 = (u32 *)malloc(sizeof(u32) * powLen);
    pow1[0] = 1; pow2[0] = 1;
    for (int i = 1; i < powLen; i++) {
        pow1[i] = (u32)(((u64)pow1[i - 1] * BASE1) % MOD1);
        pow2[i] = (u32)(((u64)pow2[i - 1] * BASE2) % MOD2);
    }

    // ---- reusable per-string buffers ----
    char *revA = (char *)malloc(maxlen + 1);
    char *tcat = (char *)malloc((size_t)(2 * maxlen + 3));
    int *zarr = (int *)malloc(sizeof(int) * (2 * maxlen + 3));
    char *isPalinPrefix = (char *)malloc(maxlen + 1);
    char *isPalinSuffix = (char *)malloc(maxlen + 1);
    u32 *FH1 = (u32 *)malloc(sizeof(u32) * (maxlen + 1));
    u32 *FH2 = (u32 *)malloc(sizeof(u32) * (maxlen + 1));

    ll ans = 0;

    for (long i = 0; i < n; i++) {
        const char *s = gdata + startArr[i];
        int l = lenArr[i];

        for (int k = 0; k < l; k++) revA[k] = s[l - 1 - k];

        // t = s + '#' + revA  -> isPalinPrefix
        memcpy(tcat, s, l);
        tcat[l] = '#';
        memcpy(tcat + l + 1, revA, l);
        zfunction(tcat, 2 * l + 1, zarr);
        isPalinPrefix[0] = 1;
        for (int k = 1; k <= l; k++) {
            int pos = 2 * l + 1 - k;
            isPalinPrefix[k] = (zarr[pos] >= k) ? 1 : 0;
        }

        // t2 = revA + '#' + s -> isPalinSuffix
        memcpy(tcat, revA, l);
        tcat[l] = '#';
        memcpy(tcat + l + 1, s, l);
        zfunction(tcat, 2 * l + 1, zarr);
        isPalinSuffix[0] = 1;
        for (int k = 1; k <= l; k++) {
            int pos = 2 * l + 1 - k;
            isPalinSuffix[k] = (zarr[pos] >= k) ? 1 : 0;
        }

        // prefix hash of revA
        FH1[0] = 0; FH2[0] = 0;
        for (int k = 0; k < l; k++) {
            u32 c = (u32)(revA[k] - 'a' + 1);
            FH1[k + 1] = (u32)(((u64)FH1[k] * BASE1 + c) % MOD1);
            FH2[k + 1] = (u32)(((u64)FH2[k] * BASE2 + c) % MOD2);
        }

        // Loop1: d = 1..l, B has length d, reverse(B) == A[0:d) == suffix
        // of revA of length d. leftover suffix A[d:l) length l-d must be
        // a palindrome -> isPalinSuffix[l-d].
        for (int d = 1; d <= l; d++) {
            // hash of revA[l-d .. l)
            u32 h1 = (u32)(((u64)FH1[l] + (u64)MOD1 - ((u64)FH1[l - d] * pow1[d]) % MOD1) % MOD1);
            u32 h2 = (u32)(((u64)FH2[l] + (u64)MOD2 - ((u64)FH2[l - d] * pow2[d]) % MOD2) % MOD2);
            u64 key = (u64)h1 * MOD2 + h2;
            int cnt = tabQuery(d, key);
            if (cnt && isPalinSuffix[l - d]) ans += (ll)cnt;
        }

        // Loop2: e = 1..l-1, A(candidate) has length e < l, reverse(A) ==
        // X[l-e:l) == prefix of revX of length e. leftover prefix
        // X[0:l-e) length l-e must be a palindrome -> isPalinPrefix[l-e].
        for (int e = 1; e < l; e++) {
            u32 h1 = FH1[e];
            u32 h2 = FH2[e];
            u64 key = (u64)h1 * MOD2 + h2;
            int cnt = tabQuery(e, key);
            if (cnt && isPalinPrefix[l - e]) ans += (ll)cnt;
        }
    }

    printf("%lld\n", ans);
    return 0;
}
