// POJ 1968 - Booklets
// Model: claude-sonnet-5
//
// The rule "any booklet A given to S_i has pages(A) <= pages(B) for any
// booklet B given to S_j when i<j" forces the schools' assigned booklets,
// in visiting order (the r = M mod N schools that get UIP = ceil(M/N)
// booklets first, then the N-r schools that get LIP = floor(M/N)), to be
// exactly the contiguous blocks of the page-sorted list of booklets.
//
// The catch (missed on the first submission, which produced Wrong
// Answer): within a school Bob delivers its assigned booklets in the
// SAME RELATIVE ORDER as on his original list, not in page-sorted order.
// So the "first booklet distributed" to a school is the one with the
// smallest ORIGINAL index among that school's block -- not the smallest
// page value in the block. These coincide only when the block happens to
// already be increasing in original index. The official sample (N=3,
// k=2, M=7) doesn't discriminate this: school 2's block is {idx4:11,
// idx5:14}, already index-increasing, so both readings give 11. Hand
// tracing school 0's block {idx3:1, idx6:2, idx0:3} exposes the gap: the
// smallest-page reading gives 1, but the smallest-original-index
// (correct) reading gives 3 (idx0 comes first on the list).
//
// So: stable-sort (page, original index) pairs by page ascending (ties
// keep list order, the natural resolution when boundary values tie),
// slice into the school blocks, then within the queried school's block
// report the page of the element with the minimum original index.
#include <cstdio>
#include <algorithm>
using namespace std;

struct Booklet {
    int page;
    int idx;
};

bool byPage(const Booklet &a, const Booklet &b) {
    return a.page < b.page;
}

int main() {
    int n, k, m;
    while (scanf("%d %d %d", &n, &k, &m) == 3) {
        static Booklet b[3000];
        for (int i = 0; i < m; ++i) {
            scanf("%d", &b[i].page);
            b[i].idx = i;
        }
        stable_sort(b, b + m, byPage);

        int lip = m / n;
        int r = m % n;      // number of schools receiving UIP = lip+1
        int uip = lip + (r > 0 ? 1 : 0);

        int start;
        int cnt;
        if (k < r) { start = k * uip; cnt = uip; }
        else { start = r * uip + (k - r) * lip; cnt = lip; }

        int bestPos = start;
        for (int i = start + 1; i < start + cnt; ++i) {
            if (b[i].idx < b[bestPos].idx) bestPos = i;
        }

        printf("%d\n", b[bestPos].page);
    }
    return 0;
}
