// POJ 3579 - Median
//
// Given N numbers, consider the C(N,2) pairwise absolute differences and
// report their median.
//
// Approach: binary search the answer.  Sort the array; then for a candidate
// distance d, the number of pairs with difference <= d is computed by a single
// two-pointer sweep (for each j, advance i while a[j]-a[i] > d and add j-i),
// which is O(n) per candidate and O(n log C) overall for value range C.  The
// smallest d whose count reaches k is the k-th smallest difference, i.e. the
// median.  The pair count reaches ~5*10^9 for N = 100000, so it is kept in
// long long.
//
// Statement ambiguity: the median is only defined for even m ("the (m/2)-th
// smallest"), and the samples settle the odd case.  Sample 2 has m = 3 and
// wants the 2nd smallest (8 out of 1,8,9), so the rank is k = ceil(m/2) =
// (m+1)/2, which also reproduces the even case (m = 6 -> 3rd smallest).
//
// Xi has only an upper bound (10^9) in the statement, no lower bound, so the
// search range and the differences are computed in long long in case values
// are negative and a span exceeds 2^31.  The answer is printed through
// std::cout to avoid POJ's old printf("%lld") quirks.
//
// Input runs to several test cases until EOF, up to ~10^6 numbers, so it is
// read through an fread buffer.

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

static int a[100005];

/* buffered reader: POJ's judge is slow and the input can hold ~10^6 numbers */
static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

static int readInt(int &out) {
    int c = gc();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c < 0) return 0;
        c = gc();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return 1;
}

/* number of pairs (i<j) with a[j]-a[i] <= d, array sorted ascending */
static long long countLE(int n, long long d) {
    long long cnt = 0;
    int i = 0;
    for (int j = 0; j < n; ++j) {
        while ((long long)a[j] - a[i] > d) ++i;
        cnt += (long long)(j - i);
    }
    return cnt;
}

int main() {
    int n;
    while (readInt(n)) {
        for (int i = 0; i < n; ++i) readInt(a[i]);
        std::sort(a, a + n);
        long long m = (long long)n * (n - 1) / 2;
        long long k = (m + 1) / 2;          /* ceil(m/2) */
        long long lo = 0, hi = (long long)a[n - 1] - a[0];
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (countLE(n, mid) >= k) hi = mid; else lo = mid + 1;
        }
        std::cout << lo << "\n";
    }
    return 0;
}
