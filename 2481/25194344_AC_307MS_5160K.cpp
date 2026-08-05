// POJ 2481 - Cows
//
// Each cow has a closed range [S,E]. Cow i is "stronger" than cow j iff
// Si<=Sj, Ej<=Ei, and Ei-Si > Ej-Sj (strict length inequality). The strict
// length comparison means an exact-duplicate range never counts as stronger
// than another copy of itself, even though the endpoint comparisons alone
// are non-strict; duplicates must therefore all receive the same answer.
//
// Approach: sort cows by S ascending, then E descending. Process cows as
// runs of cows sharing the exact same (S,E) pair (these are true duplicates
// and must be handled together): for the whole run, query a Fenwick tree
// for how many previously-inserted cows have E' >= E (S'<=S already holds
// because of the sort order), assign that single count to every cow in the
// run, then insert the whole run into the tree. Querying before inserting
// the run is what keeps duplicates from counting each other; processing
// groups in decreasing-E order within the same S lets a longer cow with
// equal S still count as stronger for a shorter one.
//
// The Fenwick tree stores point counts indexed by E, but the query needed
// is a suffix sum (E' >= E). This is done with the standard trick of
// mirroring the index: idx(E) = MAXV - E + 1, turning "suffix sum from E"
// into a plain prefix sum on the mirrored index.
//
// O(n log n) per test case; N <= 1e5, multiple test cases until a line "0".
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXV = 100000;
const int TREEN = MAXV + 2;
static int bit_[TREEN + 2];

void update(int idx, int val) {
    for (; idx <= TREEN; idx += idx & (-idx))
        bit_[idx] += val;
}
int query(int idx) {
    int s = 0;
    for (; idx > 0; idx -= idx & (-idx))
        s += bit_[idx];
    return s;
}

struct Cow {
    int s, e, idx;
};

bool cmp(const Cow &a, const Cow &b) {
    if (a.s != b.s) return a.s < b.s;
    return a.e > b.e;
}

static Cow cows[100005];
static int ans[100005];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &cows[i].s, &cows[i].e);
            cows[i].idx = i;
        }
        sort(cows, cows + n, cmp);
        memset(bit_, 0, sizeof(bit_));

        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && cows[j].s == cows[i].s && cows[j].e == cows[i].e) ++j;
            int tIdx = MAXV - cows[i].e + 1;
            int cnt = query(tIdx);
            for (int k = i; k < j; ++k) ans[cows[k].idx] = cnt;
            update(tIdx, j - i);
            i = j;
        }

        for (int k = 0; k < n; ++k) {
            printf("%d%c", ans[k], (k + 1 < n) ? ' ' : '\n');
        }
    }
    return 0;
}
