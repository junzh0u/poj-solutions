// POJ 1277 - Beer Land
// Model: claude-sonnet-5
//
// The King must build the minimum number of two-way straight-line roads
// connecting n towns so the road network survives the closure of any single
// road (i.e. no bridge edges) -- for n>=3 the minimum is a single Hamiltonian
// cycle of n roads (n edges is the minimum possible for a 2-edge-connected
// spanning graph, and it is achievable). Towns and the cyclic visiting order
// can be placed freely in the plane, so the n segments of this closed
// polygonal chain may be drawn self-intersecting. A beer stand is built at
// every point where roads cross, counted once even if several roads meet
// there -- so to maximize stands we want every crossing pair of non-adjacent
// segments to meet at its own distinct point (generic position), never
// coinciding with another crossing.
//
// This reduces to a classical result: the maximum number of self
// intersection points of a closed n-gon (n points connected in some cyclic
// order by straight segments, chosen to maximize the count of distinct
// crossing points among non-adjacent edges) is
//   n*(n-3)/2         for odd n
//   n*(n-4)/2 + 1      for even n
// verified against the given sample: n=3 -> 0, n=4 -> 1, n=5 -> 5.
//
// n=1 and n=2 are degenerate: with one town no roads are needed, and with
// two towns a single straight line between them cannot be duplicated (two
// distinct straight roads between the same two points is not geometrically
// realizable), so no valid 2-edge-connected road network with crossroads
// exists; the answer is 0 for both (the board's discuss thread flags exactly
// this as the case people forget).
//
// All values fit well within a 32-bit signed range even at n=32767
// (~5.4*10^8), so plain long/long long both work; no overflow concerns.

#include <cstdio>

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        long n;
        scanf("%ld", &n);
        long ans;
        if (n <= 2) {
            ans = 0;
        } else if (n % 2 == 1) {
            ans = n * (n - 3) / 2;
        } else {
            ans = n * (n - 4) / 2 + 1;
        }
        printf("%ld\n", ans);
    }
    return 0;
}
