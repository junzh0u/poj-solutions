// POJ 2442 - Sequence
// Model: claude-sonnet-5
// Approach: Reduce m sequences to the answer by repeated pairwise merges.
// Maintaining a running "current smallest-n-sums" array (initially the first
// sequence sorted), merge it against each subsequent sequence in turn using
// the classic "smallest n sums from two sorted arrays" technique: view row i
// (current[i] + nextSeq[j] for j=0..n-1) as a sorted list (since nextSeq is
// sorted ascending), and merge these n sorted lists with a size-n min-heap,
// seeded with (current[i]+nextSeq[0], i, 0) for every i, popping the min n
// times and each time advancing j for that same i. This is O(n log n) per
// merge and O(m n log n) overall, well inside the 6s limit for m<=100,
// n<=2000. No statement ambiguity beyond the usual: sums may repeat (as in
// the sample, "3 3 4"), and ties in the heap can be broken arbitrarily.
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

struct Node {
    int sum, i, j;
};
struct Cmp {
    bool operator()(const Node &a, const Node &b) const {
        return a.sum > b.sum;
    }
};

static int res[2005];
static int row[2005];
static int nres[2005];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int m, n;
        scanf("%d %d", &m, &n);
        for (int j = 0; j < n; ++j) scanf("%d", &res[j]);
        sort(res, res + n);
        for (int i = 1; i < m; ++i) {
            for (int j = 0; j < n; ++j) scanf("%d", &row[j]);
            sort(row, row + n);

            priority_queue<Node, vector<Node>, Cmp> pq;
            for (int k = 0; k < n; ++k) {
                Node nd;
                nd.sum = res[k] + row[0];
                nd.i = k;
                nd.j = 0;
                pq.push(nd);
            }
            for (int k = 0; k < n; ++k) {
                Node nd = pq.top();
                pq.pop();
                nres[k] = nd.sum;
                if (nd.j + 1 < n) {
                    Node nxt;
                    nxt.sum = res[nd.i] + row[nd.j + 1];
                    nxt.i = nd.i;
                    nxt.j = nd.j + 1;
                    pq.push(nxt);
                }
            }
            for (int k = 0; k < n; ++k) res[k] = nres[k];
        }
        for (int j = 0; j < n; ++j) {
            printf("%d%c", res[j], j + 1 < n ? ' ' : '\n');
        }
    }
    return 0;
}
