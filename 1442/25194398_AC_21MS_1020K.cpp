// POJ 1442 - Black Box
//
// A "Black Box" holds a multiset of integers and answers a sequence of GET
// queries: the p-th GET must report the p-th smallest of the first u(p)
// inserted values. The u sequence is non-descending with p <= u(p) <= M, so
// the requested order statistic only ever grows and the prefix of the array
// only ever extends -- which is exactly what makes an incremental structure
// work instead of re-sorting at every query.
//
// Two heaps partition the elements seen so far:
//   low  -- a max-heap holding the p smallest elements, p = GETs answered
//   high -- a min-heap holding everything else
// Invariant: every element of low <= every element of high, and |low| equals
// the number of GETs already answered.
//
// Insert x: if low is non-empty and x < low.top(), x belongs in the bottom
// set, so push x into low and move the displaced low.top() into high;
// otherwise push x into high. Either way |low| is unchanged.
//
// GET p: at that moment |low| == p-1, so low holds exactly the p-1 smallest
// elements and the p-th smallest is high.top(). Report it, then move it from
// high into low, restoring the invariant for the next query. Each transaction
// costs O(log M), so the whole run is O((M+N) log M).
//
// Notes on the statement:
//  - Values may repeat (the sample adds 2 twice); the structure is a multiset,
//    so nothing special is needed -- ties simply occupy adjacent ranks.
//  - The input is a flat whitespace-separated stream (M, N, then the M values,
//    then the N query counts), not an interleaved transaction log; the
//    ADD/GET narrative in the statement is only how the two arrays are meant
//    to be interpreted.
//  - |A(i)| <= 2 000 000 000 fits in a 32-bit int, so the hand-rolled reader
//    accumulates into an unsigned int and negates afterwards -- avoiding
//    long long, which is not C++98.
//  - u(p) >= p guarantees high is never empty at a GET.
//  - The main loop reads test cases until EOF, so a file holding several
//    concatenated cases is handled as well.
#include <cstdio>
#include <queue>
#include <vector>
#include <functional>

static const int MAXM = 30005;

static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return buf[bufPos++];
}

/* reads a signed integer; returns false at EOF */
static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    unsigned int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10u + (unsigned int)(c - '0'); c = gc(); }
    out = neg ? -(int)v : (int)v;
    return true;
}

int main() {
    int m, n;
    static int a[MAXM];
    std::vector<int> lowBuf, highBuf;
    lowBuf.reserve(MAXM);
    highBuf.reserve(MAXM);

    while (readInt(m)) {
        if (!readInt(n)) break;
        int i;
        for (i = 0; i < m; ++i) readInt(a[i]);

        /* low: max-heap holding the p smallest elements seen so far,
           where p = number of GETs already answered.
           high: min-heap holding everything else.
           Invariant: every element of low <= every element of high. */
        std::priority_queue<int> low;
        std::priority_queue<int, std::vector<int>, std::greater<int> > high;

        int pos = 0; /* how many of a[] have been inserted */
        for (int p = 1; p <= n; ++p) {
            int u;
            if (!readInt(u)) u = pos;
            while (pos < u) {
                int x = a[pos++];
                if (!low.empty() && x < low.top()) {
                    int t = low.top();
                    low.pop();
                    low.push(x);
                    high.push(t);
                } else {
                    high.push(x);
                }
            }
            /* p-th smallest = min of high, since low holds the p-1 smallest */
            int ans = high.top();
            high.pop();
            low.push(ans);
            printf("%d\n", ans);
        }
    }
    return 0;
}
