// POJ 3270 - Cow Sorting
//
// Sort N (<= 10000) distinct grumpiness values 1..100000 by swapping any two
// cows, a swap of values X and Y costing X+Y; minimise the total cost.
//
// The permutation taking the array to its sorted order splits into cycles, and
// no optimal solution ever swaps across cycles, so each cycle is paid for
// independently. For a cycle of length L, element sum S and smallest element m,
// two strategies compete:
//
//   1. Rotate in place using the cycle's own minimum as the pivot: L-1 swaps,
//      each involving m, costing S + (L-2)*m.
//   2. Borrow the global minimum g of the whole array: swap g into the cycle
//      (cost g+m), rotate the now-(L+1)-element cycle around g, then swap g
//      back out — total S + m + (L+1)*g.
//
// Take min(...) per cycle and sum. Strategy 2 wins when a cycle's own minimum
// is expensive but a much cheaper element exists elsewhere; using only
// strategy 1 is the classic wrong answer here. When g lies inside the cycle
// (m == g) strategy 2 is never cheaper, so no special case is needed.
//
// Cycles of length 1 are free. Grumpiness is stated to be unique, so ranks via
// lower_bound on the sorted copy give an unambiguous permutation.
//
// The answer reaches ~1.85e9 at the maxima (the 10000 largest values in one
// cycle), so it is accumulated in long long and printed digit by digit rather
// than with %lld, which is unreliable on POJ's older compilers.
#include <cstdio>
#include <algorithm>
using namespace std;

static const int MAXN = 10005;

static int a[MAXN];
static int b[MAXN];
static int nxt[MAXN];
static bool vis[MAXN];

static void printLL(long long v) {
    char buf[32];
    int k = 0;
    if (v == 0) { putchar('0'); putchar('\n'); return; }
    if (v < 0) { putchar('-'); v = -v; }
    while (v > 0) { buf[k++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (k > 0) putchar(buf[--k]);
    putchar('\n');
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int i;
    for (i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) a[i] = 0;
        b[i] = a[i];
    }
    if (n <= 0) { printLL(0); return 0; }
    sort(b, b + n);
    int gmin = b[0];
    for (i = 0; i < n; ++i)
        nxt[i] = (int)(lower_bound(b, b + n, a[i]) - b);
    for (i = 0; i < n; ++i) vis[i] = false;

    long long total = 0;
    for (i = 0; i < n; ++i) {
        if (vis[i]) continue;
        int j = i;
        long long s = 0;
        int len = 0;
        int m = 2147483647;
        while (!vis[j]) {
            vis[j] = true;
            s += a[j];
            if (a[j] < m) m = a[j];
            ++len;
            j = nxt[j];
        }
        if (len <= 1) continue;
        long long c1 = s + (long long)(len - 2) * m;
        long long c2 = s + m + (long long)(len + 1) * gmin;
        total += (c1 < c2 ? c1 : c2);
    }
    printLL(total);
    return 0;
}
