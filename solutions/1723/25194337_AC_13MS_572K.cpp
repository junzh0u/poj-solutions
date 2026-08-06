// POJ 1723 - SOLDIERS
//
// N soldiers must end up on consecutive integer x-coordinates at some common
// y, minimizing total Manhattan distance moved. The two coordinates are
// independent, and it's always possible to schedule the moves so soldiers
// never collide mid-flight, so the answer is just the sum of the two
// coordinates' optimal costs.
//
// y: every soldier ends at the same y, so cost is minimized by the plain
// median of the y-coordinates.
//
// x: soldiers must land on N *consecutive* distinct x values x0..x0+N-1, but
// which soldier gets which slot is free. Sort the x's ascending and subtract
// each one's 0-indexed rank i: b_i = x_i - i. Because the sorted order is
// preserved by any optimal assignment (swapping two soldiers' target slots
// never helps once sorted), matching sorted x_i to slot x0+i turns the
// "consecutive distinct integers" constraint into an ordinary "same value"
// problem in b-space: minimize sum |b_i - x0| over x0, solved by the median
// of the b_i. Total answer = median cost of y's + median cost of b's.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

long long xs[10005], ys[10005], bs[10005];

int cmp_ll(const void *a, const void *b) {
    long long va = *(const long long*)a, vb = *(const long long*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

void printLL(long long v) {
    if (v == 0) { printf("0"); return; }
    char buf[32];
    int n = 0;
    while (v > 0) { buf[n++] = '0' + (int)(v % 10); v /= 10; }
    for (int i = n - 1; i >= 0; --i) putchar(buf[i]);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld %lld", &xs[i], &ys[i]);
    }
    // y: plain median
    for (int i = 0; i < n; ++i) ys[i] = ys[i];
    qsort(ys, n, sizeof(long long), cmp_ll);
    long long ymed = ys[n / 2];
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long d = ys[i] - ymed;
        if (d < 0) d = -d;
        total += d;
    }
    // x: sort, subtract rank, then median
    qsort(xs, n, sizeof(long long), cmp_ll);
    for (int i = 0; i < n; ++i) bs[i] = xs[i] - i;
    qsort(bs, n, sizeof(long long), cmp_ll);
    long long xmed = bs[n / 2];
    for (int i = 0; i < n; ++i) {
        long long d = bs[i] - xmed;
        if (d < 0) d = -d;
        total += d;
    }
    printLL(total);
    printf("\n");
    return 0;
}
