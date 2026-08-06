// POJ 1456 - Supermarket
//
// Each product x has a profit p and a deadline d (sell it by the end of
// time-slot d, one product per unit of time, starting at time 0). Choose a
// subset and an order maximizing total profit subject to every chosen
// product finishing at or before its deadline.
//
// Classic job-sequencing-with-deadlines greedy: sort products by deadline
// ascending and sweep them in that order, keeping a min-heap of the profits
// of products tentatively accepted so far. After adding product i's profit
// to the heap, if the heap holds more items than the deadline allows (i.e.
// more than d slots exist among products seen so far with deadline <= d),
// evict the smallest profit in the heap. Because every deadline seen later
// is >= d, the evicted item could never have been scheduled at any earlier
// slot anyway, so this never discards a profit that could otherwise be
// kept alongside everything remaining. At the end the heap holds exactly
// the accepted set and its sum is the optimal profit.
//
// Ambiguity: the sample input's second block ("7 products") renders on the
// judge's page across what looks like two separate lines with a blank line
// in between; it is actually one whitespace-separated stream of 7 (profit,
// deadline) pairs -- the problem statement explicitly allows whitespace to
// occur freely, so scanf handles it uniformly.
#include <cstdio>
#include <algorithm>
#include <queue>
#include <functional>
#include <vector>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        vector<pair<int,int> > items(n); // (deadline, profit)
        for (int i = 0; i < n; ++i) {
            int p, d;
            scanf("%d %d", &p, &d);
            items[i] = make_pair(d, p);
        }
        sort(items.begin(), items.end());
        priority_queue<int, vector<int>, greater<int> > heap; // min-heap of profits
        for (int i = 0; i < n; ++i) {
            int d = items[i].first;
            int p = items[i].second;
            heap.push(p);
            if ((int)heap.size() > d) {
                heap.pop();
            }
        }
        int sum = 0;
        while (!heap.empty()) {
            sum += heap.top();
            heap.pop();
        }
        printf("%d\n", sum);
    }
    return 0;
}
