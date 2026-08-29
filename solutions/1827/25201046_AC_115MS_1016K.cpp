// POJ 1827 - A Bunch Of Monsters
// Model: claude-sonnet-5
//
// Each monster i has a card value d[i]: it may only receive a treasure box
// whose number is <= d[i]. Box numbers are distinct positive integers, so
// this is exactly weighted job scheduling with unit-time jobs and deadlines
// d[i], profit s[i]: maximize the total strength of monsters that CAN be
// simultaneously assigned distinct boxes <= their deadlines, then damage =
// (sum of all strengths) - (that maximum).
//
// Classic min-heap greedy: sort monsters by deadline ascending. Scan in that
// order, push each strength into a min-heap and add it to a running
// "selected" sum. Whenever the heap's size exceeds the current deadline d,
// there cannot be enough distinct box numbers <= d for all heap members (by
// a Hall's-theorem argument on this staircase bipartite structure), so evict
// the globally smallest strength currently held. Because deadlines are
// processed in non-decreasing order, maintaining the invariant
// heap.size() <= current d after each step keeps it valid for every earlier,
// smaller deadline too. Verified against the sample: case 2 selects
// {70,60,50,40,10} summing to 230 out of total 280, giving damage 50, which
// matches the expected output.
//
// No statement ambiguity found; the discuss board confirms sort+greedy+heap
// as the standard accepted approach (time limit is tight for slower
// approaches like segment tree / DSU variants, but O(N log N) with a heap is
// fine).
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

struct Monster {
    int d;
    int s;
};

bool cmpByDeadline(const Monster &a, const Monster &b) {
    return a.d < b.d;
}

static Monster mo[50005];

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 0; i < n; i++) scanf("%d", &mo[i].d);
        for (int i = 0; i < n; i++) scanf("%d", &mo[i].s);
        sort(mo, mo + n, cmpByDeadline);

        priority_queue<int, vector<int>, greater<int> > heap;
        long long totalSum = 0, heapSum = 0;
        for (int i = 0; i < n; i++) {
            totalSum += mo[i].s;
            heap.push(mo[i].s);
            heapSum += mo[i].s;
            if ((int)heap.size() > mo[i].d) {
                heapSum -= heap.top();
                heap.pop();
            }
        }
        printf("%lld\n", totalSum - heapSum);
    }
    return 0;
}
