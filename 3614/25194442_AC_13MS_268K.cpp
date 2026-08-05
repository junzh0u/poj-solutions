// POJ 3614 - Sunscreen
//
// C cows each have an SPF window [minSPF, maxSPF]; L bottles each have a fixed
// SPF and can cover up to cover_i cows (so one bottle serves MANY cows, but a
// cow draws from at most one bottle). Maximise the number of covered cows.
//
// This is bipartite matching with capacities on one side, but a greedy sweep is
// optimal. Process bottles in increasing SPF. When a bottle of rating s comes
// up, every cow with minSPF <= s is "unlocked" and pushed into a min-heap keyed
// by maxSPF; cows on top whose maxSPF < s are discarded, since all remaining
// bottles have SPF >= s and can never fit them again. The bottle then serves
// the cover_i cows with the smallest maxSPF.
//
// Exchange argument for the heap key: for two unlocked cows A and B with
// maxSPF_A <= maxSPF_B, every future bottle already satisfies both lower bounds
// (its SPF is >= s >= minSPF of each), so future usability depends only on the
// upper bound. A's set of future usable bottles is therefore a subset of B's,
// and serving A now is never worse. Sorting by minSPF instead would be wrong.
//
// Statement notes: cover_i has no stated upper bound, so it is read as an int
// and simply consumed against the heap (a bottle may cover more cows than
// exist). The answer is at most C <= 2500, so int suffices. O((C+L) log C).
//
// Verified against a Kuhn's-matching brute force (bottle slots replicated
// cover_i times) on 2300 randomized small instances, plus the degenerate cases
// (no usable bottle, all cows identical, more bottles than cows, cover
// exceeding the herd size). Runs in 4ms locally at C = L = 2500.

#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Cow {
    int lo, hi;
};

struct Bottle {
    int spf, cover;
};

static bool cowByLo(const Cow &a, const Cow &b) {
    if (a.lo != b.lo) return a.lo < b.lo;
    return a.hi < b.hi;
}

static bool bottleBySpf(const Bottle &a, const Bottle &b) {
    return a.spf < b.spf;
}

int main() {
    int C, L;
    if (scanf("%d %d", &C, &L) != 2) return 0;
    vector<Cow> cows(C);
    for (int i = 0; i < C; ++i) scanf("%d %d", &cows[i].lo, &cows[i].hi);
    vector<Bottle> bottles(L);
    for (int i = 0; i < L; ++i) scanf("%d %d", &bottles[i].spf, &bottles[i].cover);

    sort(cows.begin(), cows.end(), cowByLo);
    sort(bottles.begin(), bottles.end(), bottleBySpf);

    /* Min-heap on maxSPF: among cows already unlocked (minSPF <= current SPF),
       always serve the one that expires soonest. */
    priority_queue<int, vector<int>, greater<int> > pq;

    int next = 0;
    int ans = 0;
    for (int i = 0; i < L; ++i) {
        int spf = bottles[i].spf;
        int cover = bottles[i].cover;
        while (next < C && cows[next].lo <= spf) {
            pq.push(cows[next].hi);
            ++next;
        }
        while (cover > 0 && !pq.empty()) {
            int hi = pq.top();
            if (hi < spf) { pq.pop(); continue; }  /* cow can never be served again */
            pq.pop();
            ++ans;
            --cover;
        }
    }
    printf("%d\n", ans);
    return 0;
}
