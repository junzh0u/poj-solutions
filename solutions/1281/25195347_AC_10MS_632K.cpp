// POJ 1281 - MANAGER
// Model: claude-opus-5
//
// Simulate a priority queue of process costs under two policies (1 = pop
// minimum cost, 2 = pop maximum cost).  Costs live in 1..10000 and several
// processes may share a cost, so the queue is a multiset kept as a count
// array indexed by cost, wrapped in a Fenwick tree so that "pop min" and
// "pop max" are O(log MAXC) instead of a linear scan over the cost range.
// find_kth() walks the tree by binary lifting: k=1 gives the cheapest live
// process, k=total the most expensive.
//
// Input framing (not spelled out in the statement, confirmed on the discuss
// board): the file holds several data sets, blank-line separated, and must be
// read to EOF.  Every data set is  maxcost / listlen / listlen ordinals /
// requests ... / e.  Token-based extraction makes the blank lines a non-issue.
//
// Statement ambiguity, and the reason this is the version submitted: the
// Description says the manager prints when "the ordinal number of the removed
// process" is listed, while the Output section says "the ordinal number of the
// remove request" -- these differ exactly when an 'r' arrives at an empty
// queue.  The discuss board has this reported as self-contradictory, and both
// accepted sources posted there resolve it the same way, which is what is done
// here: an 'r' on an empty queue prints -1 unconditionally (whether or not any
// ordinal matches) and does NOT advance the removal counter; only a removal
// that actually happens takes the next ordinal number.
//
// Output: the cost of each removed process whose ordinal is listed, one per
// line, and a blank line after every data set (including the last one, as in
// both accepted sources on the board).
//
// C++98 only: no auto, no range-for, no <cstdint>.

#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

// Costs are stated as 1..10000; the tree is sized well past that so that a
// stray larger cost in the data cannot corrupt memory.
static const int MAXC = 100000;

static int bit[MAXC + 1];
static int logMAXC;

static void bit_clear() {
    memset(bit, 0, sizeof(bit));
}

static void bit_add(int i, int v) {
    for (; i <= MAXC; i += i & (-i)) bit[i] += v;
}

// Smallest index whose prefix sum is >= k (k >= 1 and k <= total).
static int bit_kth(int k) {
    int pos = 0;
    for (int pw = logMAXC; pw > 0; pw >>= 1) {
        if (pos + pw <= MAXC && bit[pos + pw] < k) {
            pos += pw;
            k -= bit[pos];
        }
    }
    return pos + 1;
}

int main() {
    logMAXC = 1;
    while ((logMAXC << 1) <= MAXC) logMAXC <<= 1;

    int maxcost;
    while (scanf("%d", &maxcost) == 1) {
        int listlen;
        if (scanf("%d", &listlen) != 1) break;

        std::vector<int> want;
        for (int i = 0; i < listlen; ++i) {
            int t;
            if (scanf("%d", &t) != 1) break;
            want.push_back(t);
        }
        std::sort(want.begin(), want.end());

        bit_clear();
        int total = 0;      // processes currently queued
        int removed = 0;    // ordinal of the last process actually removed
        int policy = 1;

        char req[16];
        while (scanf("%15s", req) == 1) {
            if (req[0] == 'e') break;
            if (req[0] == 'a') {
                int x;
                if (scanf("%d", &x) != 1) break;
                if (x >= 1 && x <= MAXC) {
                    bit_add(x, 1);
                    ++total;
                }
            } else if (req[0] == 'p') {
                int p;
                if (scanf("%d", &p) != 1) break;
                policy = p;
            } else if (req[0] == 'r') {
                if (total == 0) {
                    printf("-1\n");
                    continue;
                }
                int cost = bit_kth(policy == 2 ? total : 1);
                bit_add(cost, -1);
                --total;
                ++removed;
                if (std::binary_search(want.begin(), want.end(), removed))
                    printf("%d\n", cost);
            }
        }
        printf("\n");
    }
    (void)maxcost;
    return 0;
}
