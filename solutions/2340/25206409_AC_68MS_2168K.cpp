// POJ 2340 - Memory management
// Model: claude-sonnet-5
//
// Simulate an LRU-style memory allocator over N=30000 blocks with a
// T=600 second expiration window. Each block, once allocated or
// accessed, stays allocated until 600 seconds pass with no further
// request to it; the expiration is exact (a block is already free at
// time == last_touch + T, confirmed by the sample: block 1 touched at
// t=1 fails an access at t=601).
//
// Maintain, per block, the time at which it will expire (or -1 if the
// block is currently free/untouched). A min-heap of (expire_time,
// block) records scheduled expirations; before processing each input
// event at time t, pop and apply every heap entry whose expire_time
// <= t, but only if it still matches the block's current expire_time
// (a later refresh may have made the entry stale, so it is discarded
// instead of freeing the block). Freed blocks are moved into a
// std::set<int> that always holds every currently free block, so an
// allocation request just takes *set.begin().
//
// This keeps every operation at O(log N), well inside the 2s limit
// regardless of how many lines share the same timestamp (the board's
// discuss thread flags a stress case with many "1 +" lines in a row;
// a std::set/priority_queue pair handles that without degrading).
//
// Statement ambiguity: none once the discuss board confirmed input
// times are strictly non-decreasing and that whitespace surrounds the
// '+' / '.' tokens as shown in the sample; scanf("%d %c", ...) handles
// that spacing directly.

#include <cstdio>
#include <set>
#include <queue>
#include <vector>
#include <utility>

using namespace std;

static const int N = 30000;
static const int T = 600;

int main() {
    vector<int> expireTime(N + 1, -1); // -1 means currently free
    set<int> freeBlocks;
    for (int i = 1; i <= N; ++i) freeBlocks.insert(i);

    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > heap;

    int t;
    char op;
    while (scanf("%d %c", &t, &op) == 2) {
        // Apply all scheduled expirations that have occurred by time t.
        while (!heap.empty() && heap.top().first <= t) {
            pair<int, int> ev = heap.top();
            heap.pop();
            int b = ev.second;
            if (expireTime[b] == ev.first) {
                // Still valid (not refreshed since this event was scheduled).
                expireTime[b] = -1;
                freeBlocks.insert(b);
            }
        }

        if (op == '+') {
            int b = *freeBlocks.begin();
            freeBlocks.erase(freeBlocks.begin());
            int ne = t + T;
            expireTime[b] = ne;
            heap.push(make_pair(ne, b));
            printf("%d\n", b);
        } else { // '.'
            int b;
            scanf("%d", &b);
            if (expireTime[b] != -1) {
                int ne = t + T;
                expireTime[b] = ne;
                heap.push(make_pair(ne, b));
                printf("+\n");
            } else {
                printf("-\n");
            }
        }
    }

    return 0;
}
