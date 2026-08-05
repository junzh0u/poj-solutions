// POJ 3294 - Life Forms
//
// Several test cases; each gives n <= 100 DNA strings of lower-case letters,
// each 1..1000 long.  Print every longest substring shared by *more than half*
// of them, in alphabetical order, or "?" if no such substring of length >= 1
// exists.  Blank line between test cases.
//
// Approach: concatenate the n strings into one text, separating them with a
// distinct sentinel character per string (values 1..n, all below the letters,
// which are mapped to 101..126) plus a unique smallest 0 at the very end.
// Because every separator occurs exactly once, no common prefix of two
// different suffixes can ever span one, so an LCP of L is automatically L real
// letters inside a single input string.  Build the suffix array (O(len log len)
// radix-sort doubling) and the LCP array (Kasai) over that text, then binary
// search the answer length L: for a candidate L, split the suffix array into
// maximal runs of adjacent suffixes with pairwise LCP >= L and count, per run,
// how many *distinct* source strings its suffixes came from (a stamped
// membership array, so the whole sweep is O(len)).  L is feasible iff some run
// reaches n/2 + 1 distinct sources.  The runs are maximal, so two different
// runs necessarily have different length-L prefixes and each qualifying run
// contributes exactly one answer string; visiting them in suffix-array order
// emits those strings already sorted, so the final sweep at L = best prints the
// answer directly with no dedup or sort.
//
// Two things the statement leaves to be read carefully.  "More than half" is
// strict, so the threshold is n/2 + 1 for both parities (n = 4 needs 3, not 2).
// And that makes n = 1 degenerate: one string out of one is more than half, so
// the answer is the entire string -- handled separately, since the general
// sweep only ever considers runs of >= 2 suffixes (for n >= 2 a run of one
// suffix can never reach the threshold, which is also what lets the sweep skip
// suffixes that start on a separator).
//
// len <= ~100k, the binary search costs ~10 O(len) sweeps, and a maximum-size
// test case runs in about 8ms locally -- comfortable against the 5s limit even
// with many test cases in one input.
#include <cstdio>
#include <cstring>

const int MAXL = 101 * 1010;   // 100 strings * 1000 chars + 100 separators + slack
const int MAXA = 200;          // alphabet bound

int s[MAXL];                   // encoded text (sentinel 0 appended)
char raw[MAXL];                // original letters ('\0' at separator positions)
int belong[MAXL];              // which input string a position came from, -1 for separator

int sa[MAXL], rnk_[MAXL], height_[MAXL];
int wa_[MAXL], wb_[MAXL], wv_[MAXL], ws_[MAXL > MAXA ? MAXL : MAXA];

int n, len;
int mark_[128], stampv;
int need;

static inline int cmpk(int *r, int a, int b, int l) {
    return r[a] == r[b] && r[a + l] == r[b + l];
}

// Classic O(n log n) doubling suffix array.  r[0..n-1] with r[n-1] the unique
// smallest value (0), values in [0, m).
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

// Kasai.  n = text length excluding the sentinel; height_[i] = lcp(sa[i-1], sa[i]).
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

// Scan maximal runs of suffixes whose pairwise lcp is >= L; a run qualifies when
// the suffixes in it come from more than half of the input strings.  When `out`
// is non-null the starting position of each qualifying run is appended (runs are
// visited in suffix-array order, i.e. the substrings come out sorted).
bool check(int L, int *out, int *outn) {
    bool found = false;
    if (outn) *outn = 0;
    int i = 1;
    while (i <= len) {
        int j = i;
        while (j + 1 <= len && height_[j + 1] >= L) j++;
        if (j > i) {                 // need >= 2 sources, so singleton runs can't qualify
            stampv++;
            int c = 0;
            for (int k = i; k <= j; k++) {
                int b = belong[sa[k]];
                if (b >= 0 && mark_[b] != stampv) { mark_[b] = stampv; c++; }
            }
            if (c >= need) {
                found = true;
                if (out) out[(*outn)++] = sa[i];
            }
        }
        i = j + 1;
    }
    return found;
}

char buf[1024];
int outpos[MAXL];

int main() {
    bool first = true;
    while (scanf("%d", &n) == 1 && n != 0) {
        if (!first) putchar('\n');
        first = false;

        len = 0;
        int maxlen = 0;
        for (int i = 0; i < n; i++) {
            scanf("%s", buf);
            int l = (int)strlen(buf);
            if (l > maxlen) maxlen = l;
            for (int k = 0; k < l; k++) {
                raw[len] = buf[k];
                belong[len] = i;
                s[len] = 101 + (buf[k] - 'a');   // letters live above every separator
                len++;
            }
            raw[len] = 0;
            belong[len] = -1;
            s[len] = 1 + i;                      // separator, unique per string
            len++;
        }
        s[len] = 0;                              // sentinel, unique smallest

        // n == 1 is the degenerate majority case: one string out of one is
        // "more than half", so the whole string is the answer.
        if (n == 1) {
            for (int k = 0; k < len - 1; k++) putchar(raw[k]);
            putchar('\n');
            continue;
        }

        need = n / 2 + 1;                        // strictly more than half

        da(s, sa, len + 1, MAXA);
        calheight(s, sa, len);

        int lo = 1, hi = maxlen, best = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (check(mid, 0, 0)) { best = mid; lo = mid + 1; }
            else hi = mid - 1;
        }

        if (best == 0) {
            printf("?\n");
        } else {
            int cnt = 0;
            check(best, outpos, &cnt);
            for (int i = 0; i < cnt; i++) {
                for (int k = 0; k < best; k++) putchar(raw[outpos[i] + k]);
                putchar('\n');
            }
        }
    }
    return 0;
}
