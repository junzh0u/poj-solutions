// POJ 3190 - Stall Reservations
//
// N cows each need a private stall for the closed interval [A, B]; find the
// minimum number of stalls and print, for every cow, the stall it gets.
//
// Approach: sort the cows by start time and sweep, keeping a min-heap of
// (end time, stall id) over the stalls currently in use. For the next cow,
// look at the stall whose occupant finishes earliest: if that end time is
// strictly less than the cow's start, the stall is free and is reused with
// its end time updated; otherwise every stall is still busy and a new one is
// opened. The number of stalls opened equals the maximum number of intervals
// covering any single point, which is the lower bound, so it is optimal.
// O(n log n) time, O(n) memory.
//
// Two details the statement makes easy to get wrong:
//  - The intervals are closed, so cows [1,2] and [2,3] conflict; a stall is
//    reusable only when the new cow starts strictly after the previous one
//    ends (the sample reuses stall 2 for 2..4 then 5..8).
//  - The output is per cow in *input* order, so the original index has to be
//    carried through the sort and the answer written back through it.
//
// Special Judge: any assignment achieving the minimum stall count is accepted.

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

struct Cow {
    int a, b, idx;
};

static bool byStart(const Cow &x, const Cow &y) {
    if (x.a != y.a) return x.a < y.a;
    return x.b < y.b;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<Cow> cows(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &cows[i].a, &cows[i].b);
        cows[i].idx = i;
    }
    sort(cows.begin(), cows.end(), byStart);

    vector<int> ans(n, 0);
    /* min-heap of (end time, stall id) */
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    int stalls = 0;
    for (int i = 0; i < n; ++i) {
        if (!pq.empty() && pq.top().first < cows[i].a) {
            pair<int, int> t = pq.top();
            pq.pop();
            ans[cows[i].idx] = t.second;
            pq.push(make_pair(cows[i].b, t.second));
        } else {
            ++stalls;
            ans[cows[i].idx] = stalls;
            pq.push(make_pair(cows[i].b, stalls));
        }
    }

    printf("%d\n", stalls);
    for (int i = 0; i < n; ++i) printf("%d\n", ans[i]);
    return 0;
}
