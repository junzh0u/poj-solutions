// POJ 3415 - Common Substrings
//
// Count triples (i, j, k) with k >= K such that A(i,k) = B(j,k), i.e. the
// number of (position-in-A, position-in-B, length) matches of length >= K,
// counted with multiplicity (not distinct substrings).
//
// Approach: build the suffix array of S = A + sep + B (sep a character
// smaller than every letter and unique to both strings) plus one more
// sentinel for the doubling construction, then the LCP (height) array via
// Kasai's algorithm. For any two suffixes p (from A) and q (from B),
// LCP(p, q) is exactly the length of the longest common prefix of A's
// suffix at p and B's suffix at q, since the unique separator stops any
// suffix from crossing between the two strings. The number of valid k for
// that pair is max(0, LCP(p, q) - K + 1). The answer is the sum of this
// over every pair of suffixes with opposite origin (one from A, one from B).
//
// To compute that sum in O(n) after the suffix array is built, sweep the
// suffix array left to right maintaining a monotonic stack of "groups":
// each group holds a height h and counts (cntA, cntB) of earlier suffixes
// whose LCP with the *next* suffix to be compared is currently h (LCP(i, j)
// for i < j equals the minimum height in (i, j], so as new heights arrive,
// old groups get capped down to the new, smaller height and merge). Two
// running totals, totalA and totalB, track sum of max(0, h-K+1) contributions
// from all A-origin / B-origin suffixes seen so far against "whatever comes
// next". When we reach suffix j, if it's an A-suffix we add totalB (all
// earlier B-suffixes' contributions) to the answer, and vice versa; then we
// push a new pending group (height = +infinity, since its real capping
// height isn't known until a smaller height arrives later).
//
// The one subtlety that isn't obvious from a quick reading of the standard
// technique: a freshly pushed pending group (height = infinity) has *not*
// had any contribution added to totalA/totalB yet (nothing has capped it),
// so when it's later popped (which happens on the very next iteration,
// since infinity exceeds any real height), you must not recompute its
// contribution from its stored height -- that would use "infinity - K + 1"
// and corrupt the running totals with a huge bogus value. Instead each
// group stores addedC[], the contribution value actually folded into
// totalA/totalB at creation/merge time (0 for a still-pending group), and
// popping subtracts exactly that stored value.
//
// The answer can exceed 2^31 (e.g. two 1e5-length strings of a single
// repeated letter with K=1 gives ~3.3e14), so everything is long long.
// POJ's judge runs on Windows and printf("%lld") is unreliable there
// (some compiler/language combinations there want "%I64d" instead), so the
// answer is printed digit by digit rather than through printf.
//
// Nothing in the statement was ambiguous once "triples" was read as
// (start-in-A, start-in-B, length) rather than distinct substring values.
#include <cstdio>
#include <cstring>

const int MAXN = 100005;
const int MAXL = 2 * MAXN + 10;
const int MAXA = 60;

char A[MAXN], B[MAXN];
int s[MAXL];
int type_[MAXL]; // 0 = belongs to A, 1 = belongs to B, -1 = separator/sentinel

int sa[MAXL], rnk_[MAXL], height_[MAXL];
int wa_[MAXL], wb_[MAXL], wv_[MAXL], ws_[MAXL];

int la, lb, len;
long long K;

static inline int cmpk(int *r, int a, int b, int l) {
    return r[a] == r[b] && r[a + l] == r[b + l];
}

// Classic O(n log n) doubling suffix array (radix-sort based).
void da(int *r, int *sa, int n, int m) {
    int i, j, p, *x = wa_, *y = wb_, *t;
    for (i = 0; i < m; i++) ws_[i] = 0;
    for (i = 0; i < n; i++) ws_[x[i] = r[i]]++;
    for (i = 1; i < m; i++) ws_[i] += ws_[i - 1];
    for (i = n - 1; i >= 0; i--) sa[--ws_[x[i]]] = i;
    for (j = 1, p = 1; p < n; j *= 2, m = p) {
        for (p = 0, i = n - j; i < n; i++) y[p++] = i;
        for (i = 0; i < n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
        for (i = 0; i < n; i++) wv_[i] = x[y[i]];
        for (i = 0; i < m; i++) ws_[i] = 0;
        for (i = 0; i < n; i++) ws_[wv_[i]]++;
        for (i = 1; i < m; i++) ws_[i] += ws_[i - 1];
        for (i = n - 1; i >= 0; i--) sa[--ws_[wv_[i]]] = y[i];
        for (t = x, x = y, y = t, p = 1, x[sa[0]] = 0, i = 1; i < n; i++)
            x[sa[i]] = cmpk(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
    }
}

// Kasai. n = text length excluding the appended sentinel;
// height_[i] = lcp(sa[i-1], sa[i]) for i = 1..n.
void calheight(int *r, int *sa, int n) {
    int i, j, k = 0;
    for (i = 1; i <= n; i++) rnk_[sa[i]] = i;
    for (i = 0; i < n; i++) {
        if (k) k--;
        j = sa[rnk_[i] - 1];
        while (r[i + k] == r[j + k]) k++;
        height_[rnk_[i]] = k;
    }
}

// POJ's judge runs on Windows; printf("%lld") is unreliable there (needs
// %I64d with some compiler/language combos). Print digit by digit instead.
static void printll(long long x) {
    char buf[24];
    int len = 0;
    if (x == 0) { putchar('0'); putchar('\n'); return; }
    while (x > 0) { buf[len++] = (char)('0' + (int)(x % 10)); x /= 10; }
    while (len > 0) putchar(buf[--len]);
    putchar('\n');
}

static inline int code(char c) {
    if (c >= 'a' && c <= 'z') return 2 + (c - 'a');
    return 28 + (c - 'A'); // 'A'..'Z'
}

// Stack of groups (h, cntA, cntB): a run of previously-seen suffixes whose
// LCP with the suffix about to be compared next is h, cntA/cntB of them
// belonging to A/B respectively. addedC[top] is the per-member contribution
// (max(0,h-K+1)) that was actually folded into totalA/totalB for this group;
// a freshly-pushed pending group (h = INF, not yet capped by anything) has
// addedC = 0 since nothing has been added for it yet. Popping must subtract
// exactly addedC, never recompute from h (h can be INF, which would
// otherwise blow totalA/totalB to garbage).
int sh[MAXL];
long long sca[MAXL], scb[MAXL], addedC[MAXL];
int top_;

int main() {
    int kin;
    while (scanf("%d", &kin) == 1 && kin != 0) {
        K = kin;
        scanf("%s", A);
        scanf("%s", B);
        la = (int)strlen(A);
        lb = (int)strlen(B);
        len = la + 1 + lb;

        for (int i = 0; i < la; i++) { s[i] = code(A[i]); type_[i] = 0; }
        s[la] = 1; type_[la] = -1; // unique separator, below every letter code
        for (int j = 0; j < lb; j++) { s[la + 1 + j] = code(B[j]); type_[la + 1 + j] = 1; }
        s[len] = 0; // sentinel for suffix array construction

        da(s, sa, len + 1, MAXA);
        calheight(s, sa, len);

        long long answer = 0;
        long long totalA = 0, totalB = 0;
        top_ = -1;
        const int INF = 1 << 30;

        for (int i = 1; i <= len; i++) {
            int hh = height_[i];
            long long accA = 0, accB = 0;
            while (top_ >= 0 && sh[top_] > hh) {
                accA += sca[top_];
                accB += scb[top_];
                totalA -= sca[top_] * addedC[top_];
                totalB -= scb[top_] * addedC[top_];
                top_--;
            }
            if (accA > 0 || accB > 0) {
                long long contrib = (long long)hh - K + 1;
                if (contrib < 0) contrib = 0;
                if (top_ >= 0 && sh[top_] == hh) {
                    sca[top_] += accA;
                    scb[top_] += accB;
                    addedC[top_] = contrib;
                } else {
                    top_++;
                    sh[top_] = hh;
                    sca[top_] = accA;
                    scb[top_] = accB;
                    addedC[top_] = contrib;
                }
                totalA += accA * contrib;
                totalB += accB * contrib;
            }

            int pos = sa[i];
            int t = type_[pos];
            if (t == 0) answer += totalB;
            else if (t == 1) answer += totalA;

            top_++;
            sh[top_] = INF;
            sca[top_] = (t == 0) ? 1 : 0;
            scb[top_] = (t == 1) ? 1 : 0;
            addedC[top_] = 0;
        }

        printll(answer);
    }
    return 0;
}
