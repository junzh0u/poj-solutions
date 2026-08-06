// POJ 2796 - Feel Good
//
// Maximise (sum of a subarray) * (minimum of that subarray), and report the
// subarray itself with 1-based inclusive endpoints.
//
// Every candidate subarray can be charged to the element that is its minimum,
// and for a fixed minimum a[i] the best subarray is the widest one over which
// a[i] is still the minimum -- widening only adds non-negative terms to the sum
// (values are >= 0) while the minimum is unchanged. So it suffices to consider,
// for each i, the maximal span (L[i]+1 .. R[i]-1) where L[i] is the nearest
// index to the left holding a strictly smaller value and R[i] the nearest such
// index to the right. Two monotonic-stack passes (pop while the stacked value
// is >= a[i]) give both arrays in O(n), and a prefix-sum array turns each span
// into a[i] * (pre[R[i]-1] - pre[L[i]]) in O(1). Total O(n) time, O(n) memory.
//
// Using "strictly smaller" on both sides means a run of equal values assigns
// the same full span to each of its members -- harmless, since the minimum over
// that span really is that value.
//
// Notes on the statement:
//   - Values are 0..1e6 and n <= 1e5, so the answer reaches 1e5 * 1e6 * 1e6 =
//     1e17: it must be accumulated and printed as a 64-bit integer. Output is
//     written digit by digit rather than through printf's %lld.
//   - Zeros are allowed, so the answer can legitimately be 0; n >= 1 guarantees
//     some interval always exists, and a single element is a valid answer.
//   - "If there are multiple periods with the greatest possible value, then
//     print any one of them" -- the problem is special-judged, so any span
//     achieving the maximum is accepted.
#include <cstdio>

static const int MAXN = 100005;

static int a[MAXN];
static long long pre[MAXN];
static int L[MAXN], R[MAXN];
static int stk[MAXN];

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return -1;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sign;
}

static void writeLL(long long x) {
    char buf[24];
    int n = 0;
    if (x == 0) { putchar('0'); return; }
    if (x < 0) { putchar('-'); x = -x; }
    while (x > 0) { buf[n++] = (char)('0' + (int)(x % 10)); x /= 10; }
    while (n > 0) putchar(buf[--n]);
}

int main() {
    int n = readInt();
    if (n <= 0) { putchar('0'); putchar('\n'); return 0; }
    int i;
    for (i = 1; i <= n; ++i) a[i] = readInt();
    pre[0] = 0;
    for (i = 1; i <= n; ++i) pre[i] = pre[i - 1] + a[i];

    /* L[i] = index of nearest element to the left that is strictly smaller */
    int top = 0;
    for (i = 1; i <= n; ++i) {
        while (top > 0 && a[stk[top]] >= a[i]) --top;
        L[i] = (top > 0) ? stk[top] : 0;
        stk[++top] = i;
    }
    /* R[i] = index of nearest element to the right that is strictly smaller */
    top = 0;
    for (i = n; i >= 1; --i) {
        while (top > 0 && a[stk[top]] >= a[i]) --top;
        R[i] = (top > 0) ? stk[top] : n + 1;
        stk[++top] = i;
    }

    long long best = -1;
    int bl = 1, br = 1;
    for (i = 1; i <= n; ++i) {
        int lo = L[i] + 1, hi = R[i] - 1;
        long long v = (long long)a[i] * (pre[hi] - pre[lo - 1]);
        if (v > best) { best = v; bl = lo; br = hi; }
    }

    writeLL(best);
    putchar('\n');
    writeLL(bl);
    putchar(' ');
    writeLL(br);
    putchar('\n');
    return 0;
}
