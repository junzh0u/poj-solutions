// POJ 3378 - Crazy Thairs
// Model: claude-opus-5
//
// Count 5-tuples i<j<k<l<m with A_i<A_j<A_k<A_l<A_m, N <= 50000, A <= 1e9.
//
// Approach: coordinate-compress the values, then run four Fenwick trees, one
// per subsequence length 1..4.  Sweeping left to right, for the current
// element of rank r let d[t] be the number of strictly increasing
// subsequences of length t ending here; d[1] = 1 and d[t] = sum of d[t-1]
// over all earlier elements of rank < r, which is a prefix query on tree
// t-1.  d[5] is accumulated into the answer; d[1..4] are pushed into their
// trees.  O(N log N) per test case.
//
// Precision: every intermediate fits in a signed 64-bit integer -- the
// largest is the total number of length-4 subsequences, C(50000,4) ~ 2.6e17,
// and a single d[5] is at most C(49999,4) ~ 2.6e17.  Only the final sum
// overflows: C(50000,5) ~ 2.6e21 > 2^64.  So the answer alone is kept in a
// base-1e9 big-integer accumulator, added to once per element.  Printing
// uses %u / %09u so no 64-bit printf format specifier is needed (POJ's C++
// compiler is old and %lld is unreliable there).
//
// Input framing: the statement says "several test cases" without giving a
// count, so read to EOF.  Values are compared strictly, so equal values must
// share a rank and the prefix query must stop at rank-1.

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

int n;
int a[MAXN], srt[MAXN];
long long tre[4][MAXN];
int m;

int lowbit(int x) { return x & (-x); }

void upd(int lv, int p, long long v) {
    for (; p <= m; p += lowbit(p)) tre[lv][p] += v;
}

long long qry(int lv, int p) {
    long long s = 0;
    for (; p > 0; p -= lowbit(p)) s += tre[lv][p];
    return s;
}

/* base-1e9 accumulator for the final answer */
const int LIMBS = 8;
unsigned int acc[LIMBS];

void accAdd(long long v) {
    unsigned int part[3];
    part[0] = (unsigned int)(v % 1000000000LL);
    part[1] = (unsigned int)((v / 1000000000LL) % 1000000000LL);
    part[2] = (unsigned int)(v / 1000000000000000000LL);
    unsigned long long carry = 0;
    for (int i = 0; i < LIMBS; ++i) {
        unsigned long long cur = (unsigned long long)acc[i] + carry;
        if (i < 3) cur += part[i];
        acc[i] = (unsigned int)(cur % 1000000000ULL);
        carry = cur / 1000000000ULL;
    }
}

void accPrint() {
    int hi = LIMBS - 1;
    while (hi > 0 && acc[hi] == 0) --hi;
    printf("%u", acc[hi]);
    for (int i = hi - 1; i >= 0; --i) printf("%09u", acc[i]);
    printf("\n");
}

int readInt(int *out) {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return 0;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    *out = v * sign;
    return 1;
}

int main() {
    while (readInt(&n)) {
        if (n < 0) n = 0;
        if (n > MAXN - 5) n = MAXN - 5;
        for (int i = 0; i < n; ++i) { readInt(&a[i]); srt[i] = a[i]; }
        sort(srt, srt + n);
        m = (int)(unique(srt, srt + n) - srt);
        for (int lv = 0; lv < 4; ++lv)
            for (int i = 0; i <= m; ++i) tre[lv][i] = 0;
        memset(acc, 0, sizeof(acc));
        for (int i = 0; i < n; ++i) {
            int r = (int)(lower_bound(srt, srt + m, a[i]) - srt) + 1;
            long long d[6];
            d[1] = 1;
            for (int lv = 2; lv <= 5; ++lv) d[lv] = qry(lv - 2, r - 1);
            for (int lv = 1; lv <= 4; ++lv) if (d[lv]) upd(lv - 1, r, d[lv]);
            if (d[5]) accAdd(d[5]);
        }
        accPrint();
    }
    return 0;
}
