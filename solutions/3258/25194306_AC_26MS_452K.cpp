// POJ 3258 - River Hopscotch
//
// Binary search on the answer (the maximum achievable "shortest jump").
// For a candidate distance d, greedily sweep the rocks left to right and
// remove any rock that is closer than d to the last rock kept; if the
// number removed exceeds M, d is infeasible. This greedy check is a
// standard result: keeping the earliest possible rock at each step never
// does worse than any other choice, since it only gives more room to the
// rocks that follow. Binary search d over [1, L] using this feasibility
// test to find the largest working distance.
//
// The start (0) and end (L) banks are rocks that can never be removed, so
// they are inserted into the position array like ordinary rocks and simply
// excluded from candidacy in the greedy sweep (the loop only ever removes
// interior rocks, since index 0 is the fixed starting point and the last
// element is the fixed end bank whose own gap to its predecessor is
// checked but which is never itself discarded).
#include <cstdio>
#include <algorithm>
using namespace std;

int L, N, M;
int d[50005]; // positions of rocks including 0 and L, sorted

bool feasible(int mid, int n) {
    // n = N+2 total rocks (including 0 and L), d[0..n-1] sorted
    int removed = 0;
    int last = d[0];
    for (int i = 1; i < n; i++) {
        if (d[i] - last < mid) {
            removed++;
            if (removed > M) return false;
        } else {
            last = d[i];
        }
    }
    return removed <= M;
}

int main() {
    scanf("%d %d %d", &L, &N, &M);
    for (int i = 0; i < N; i++) scanf("%d", &d[i+1]);
    d[0] = 0;
    d[N+1] = L;
    int n = N + 2;
    sort(d, d + n);

    int lo = 1, hi = L, ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(mid, n)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    printf("%d\n", ans);
    return 0;
}
