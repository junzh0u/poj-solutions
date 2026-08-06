// POJ 2010 - Moo University - Financial Aid
//
// Choose N (odd) calves out of C so the median CSAT score is maximized
// while total financial aid stays within budget F.
//
// Key idea: sort calves by score. If calf i (0-indexed in this sorted
// order) is the median, we need exactly k = (N-1)/2 other admitted calves
// with index < i and k with index > i. Any k calves chosen from the
// left side have score <= calves[i].score, and any k from the right side
// have score >= calves[i].score, so no matter which ones are picked,
// calf i still ends up exactly in the middle once everything is sorted
// together - ties are handled automatically by this positional argument.
// So for each i, the cheapest way to supply the k needed on each side is
// simply the k cheapest costs available on that side.
//
// prefix[i] = min total cost of the k cheapest calves among indices
// [0, i), computed with a running max-heap of size k while sweeping
// left to right (swap the heap's max out whenever a cheaper cost shows
// up), and -1 if fewer than k calves are available yet.
// suffix[i] is the mirror computation over indices (i, C) sweeping
// right to left.
//
// For every i with both prefix[i] and suffix[i] defined, the cheapest
// total cost to make calf i the median is prefix[i] + cost[i] +
// suffix[i]; if that fits in F, calves[i].score is an achievable
// median. The answer is the max such score, or -1 if none fit
// (explicitly required by the statement for insufficient funds).
// Overall O(C log C) for the sort plus the two heap sweeps.
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

struct Calf {
    int score;
    int cost;
};

bool cmpScore(const Calf &a, const Calf &b) {
    return a.score < b.score;
}

int main() {
    int N, C;
    long long F;
    scanf("%d %d %lld", &N, &C, &F);
    vector<Calf> calves(C);
    for (int i = 0; i < C; i++) {
        scanf("%d %d", &calves[i].score, &calves[i].cost);
    }
    sort(calves.begin(), calves.end(), cmpScore);

    int k = (N - 1) / 2;
    vector<long long> prefix(C, -1);
    vector<long long> suffix(C, -1);

    {
        priority_queue<int> heap; // max-heap holding the k cheapest costs seen so far
        long long sum = 0;
        for (int i = 0; i < C; i++) {
            if ((int)heap.size() == k) prefix[i] = sum;
            int c = calves[i].cost;
            if ((int)heap.size() < k) {
                heap.push(c);
                sum += c;
            } else if (!heap.empty() && c < heap.top()) {
                sum -= heap.top();
                heap.pop();
                heap.push(c);
                sum += c;
            }
        }
    }
    {
        priority_queue<int> heap;
        long long sum = 0;
        for (int i = C - 1; i >= 0; i--) {
            if ((int)heap.size() == k) suffix[i] = sum;
            int c = calves[i].cost;
            if ((int)heap.size() < k) {
                heap.push(c);
                sum += c;
            } else if (!heap.empty() && c < heap.top()) {
                sum -= heap.top();
                heap.pop();
                heap.push(c);
                sum += c;
            }
        }
    }

    int ans = -1;
    for (int i = 0; i < C; i++) {
        if (prefix[i] < 0 || suffix[i] < 0) continue;
        long long total = prefix[i] + suffix[i] + calves[i].cost;
        if (total <= F && calves[i].score > ans) {
            ans = calves[i].score;
        }
    }
    printf("%d\n", ans);
    return 0;
}
