// POJ 2374 - Fence Obstacle Course
// Model: claude-sonnet-5
// Approach: process fences from N down to 1 (per the statement: "fences will
// be traversed in reverse order of the input sequence", i.e. start atop fence
// N at x=S and walk down toward fence 1 then the barn door at x=0). Maintain
// a sparse map from reachable x-coordinate -> minimum accumulated horizontal
// walking cost to be at that x, "about to interact with" the current fence
// level. For fence i's segment [A_i,B_i]: any state whose x lies STRICTLY
// inside (A_i,B_i) is blocked by that fence (this includes the very first
// fence N, since starting atop it counts as standing on it) and must be
// walked to one end or the other, cost += distance; states with x <= A_i or
// x >= B_i pass through unobstructed (landing exactly on an endpoint does not
// block -- confirmed against the sample and a board-reported edge case).
// Collapsed-to-endpoint states are merged (min-cost) into just the two
// endpoint keys, so the map holds O(N) keys total across the whole run and
// every entry is erased at most once, giving O(N log N) overall. After all N
// fences, the answer is min over remaining states of cost + |x| (final walk
// to the door at the origin).
//
// Ambiguity resolved from the discuss board (message 341392 "does landing on
// the endpoint count as hitting [the fence]?"): endpoints A_i, B_i are NOT
// blocking, matching the walkthrough in the statement's own worked example
// and independently confirmed by reproducing message 352263's test case
// (N=4 S=-7, segments (-40,53)(6,58)(56,95)(42,88), whose starting point
// violates the stated A_N<=S<=B_N guarantee) which this algorithm evaluates
// to 73, the value the board consensus expected.
#include <cstdio>
#include <map>
using namespace std;

int main(){
    int n, s;
    scanf("%d %d", &n, &s);
    static int A[50005], B[50005];
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &A[i], &B[i]);
    }
    map<int, long long> M;
    M[s] = 0;
    for (int i = n; i >= 1; i--) {
        int a = A[i], b = B[i];
        map<int, long long>::iterator lo = M.upper_bound(a);
        map<int, long long>::iterator hi = M.lower_bound(b);
        if (lo == hi) continue;
        long long bestA = 0, bestB = 0;
        bool haveA = false, haveB = false;
        for (map<int, long long>::iterator it = lo; it != hi; ++it) {
            long long costA = it->second + (long long)(it->first - a);
            long long costB = it->second + (long long)(b - it->first);
            if (!haveA || costA < bestA) { bestA = costA; haveA = true; }
            if (!haveB || costB < bestB) { bestB = costB; haveB = true; }
        }
        M.erase(lo, hi);
        if (haveA) {
            map<int, long long>::iterator it = M.find(a);
            if (it == M.end()) M[a] = bestA;
            else if (bestA < it->second) it->second = bestA;
        }
        if (haveB) {
            map<int, long long>::iterator it = M.find(b);
            if (it == M.end()) M[b] = bestB;
            else if (bestB < it->second) it->second = bestB;
        }
    }
    long long ans = -1;
    for (map<int, long long>::iterator it = M.begin(); it != M.end(); ++it) {
        long long x = it->first;
        long long cost = it->second + (x >= 0 ? x : -x);
        if (ans < 0 || cost < ans) ans = cost;
    }
    printf("%lld\n", ans);
    return 0;
}
