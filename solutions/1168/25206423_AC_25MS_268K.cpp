// POJ 1168 - The Circle
// Model: claude-sonnet-5
//
// Approach: n<=6 sectors arranged in a circle, each holding an integer >=k.
// We want the maximum i such that all of m..i are expressible as sums of
// contiguous (circular) runs of sectors, and to list every circular
// arrangement achieving that maximum, each printed starting from a sector
// holding the minimum value in that arrangement (both traversal directions
// count as separate arrangements, since reversing the circle is a distinct
// printable sequence even though it reproduces the same set of arc sums).
//
// Key bound: a circle of n sectors has exactly n*n (start,length) arcs, but
// the n length-n arcs (the whole circle) all share one value, so at most
// n*(n-1)+1 distinct sums are achievable. Hence i <= m + n*(n-1) - 1 + 1
// = m + n*(n-1), and every needed value lies in [m, m+n*(n-1)+1] (one past
// the true bound, chosen so a witness of "next still-missing value" always
// exists within range by pigeonhole -- this matches a posted accepted
// solution on the problem's discuss board, whose bound we reuse).
//
// Search: physical circle position 0 always receives the globally smallest
// value used (this is WLOG since we explore, over all permutations of the
// remaining n-1 positions, every way of placing subsequently-larger values
// into the other slots -- so every rotation/reflection that starts at a
// minimum-valued sector is discovered as its own array). Within one such
// permutation we assign values to slots in non-decreasing order; this is
// provably safe pruning: when placing the (r+1)-th smallest value v, every
// arc fully composed of the already-placed (smaller-or-equal) sectors is
// already known, and any arc touching a not-yet-placed sector has sum >= v.
// So if v exceeds the smallest still-missing target number ("nextneed"),
// that missing number could never be produced by any completion -- hence
// v <= nextneed is a necessary bound, not merely a heuristic, and pruning
// on it loses no valid arrangement.
//
// All arrangements attaining the optimum are collected, then sorted and
// deduplicated lexicographically for output.

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

int n, m, k, HB;
int a[6];
// Tracks EVERY confirmed arc sum, not just those in [m,HB]: a value below m
// or above HB still uses up one of the circle's limited n*(n-1)+1 distinct
// sums, so counting it is what makes the capacity bound below valid. Every
// sector value is capped at HB, so the largest possible arc sum is n*HB;
// size the array generously past that.
bool achieved[2000];
int distinctCount; // number of true entries in achieved[]
int maxDistinctTotal; // n*(n-1)+1, the hard cap on distinct arc sums
int bestI;
vector< vector<int> > sols;

int nextMissing() {
    for (int t = m; t <= HB; t++)
        if (!achieved[t]) return t;
    return HB + 1; // shouldn't happen given HB's pigeonhole margin
}

// Recompute which of the n*n arcs are now fully decided and mark their sums,
// recording the newly-set indices into the caller's stack buffer (no heap
// allocation per call -- this runs at every search node, and each recursion
// level uses its own local array so nested calls can't clobber each other).
int markArcs(int newly[]) {
    int cnt = 0;
    for (int s = 0; s < n; s++) {
        int sum = 0;
        for (int len = 1; len <= n; len++) {
            int idx = (s + len - 1) % n;
            if (a[idx] == -1) break;
            sum += a[idx];
            if (!achieved[sum]) {
                achieved[sum] = true;
                distinctCount++;
                newly[cnt++] = sum;
            }
        }
    }
    return cnt;
}

// Rather than fixing the decision order up front (as one permutation among
// (n-1)! choices) and re-running a whole independent search per permutation,
// fold the choice of "which still-undecided physical position gets the next
// value" into the DFS itself. Every (position order, value assignment)
// combination explored is identical to the permutation-based formulation --
// this only merges shared prefixes so they are computed once instead of
// once per permutation that happens to start the same way.
void dfs(int rank, int lastVal, int sumSoFar, int usedMask) {
    if (rank == n) {
        int run = nextMissing() - 1;
        if (run < m - 1) return; // nothing achieved at all (degenerate)
        if (run > bestI) {
            bestI = run;
            sols.clear();
        }
        if (run == bestI) {
            vector<int> cur(a, a + n);
            sols.push_back(cur);
        }
        return;
    }
    // The full-circle sum T is the unique maximum arc sum, so the final
    // answer i can never exceed T. Bound T by assuming every still-undecided
    // sector takes the global max value HB (nextneed can only grow as more
    // sectors are placed, so bounding by the *current* nextneed instead
    // would be unsound -- it can rise again once a large value completes
    // more arcs, exactly as happens in the sample).
    if (sumSoFar + (n - rank) * HB < bestI) return;
    int nn = nextMissing();
    // Every currently-missing target number in [nn, X] needs its own new
    // distinct arc sum to be covered, and the circle can never hold more
    // than maxDistinctTotal distinct sums in total (already-used ones
    // included, whatever their magnitude, since those arcs are shared with
    // whatever gets built later). Spend the remaining "new distinct sum"
    // budget scanning upward from nn, covering already-achieved values for
    // free: the point it runs out bounds the best possible final run --
    // tighter early on than the total-sum bound above, since HB alone is a
    // loose ceiling. (A naive "budget = X - nn + 1" version is unsound: it
    // ignores values past nn that may already be achieved out of order.)
    {
        int budget = maxDistinctTotal - distinctCount;
        int t = nn;
        while (t <= HB && (achieved[t] || budget > 0)) {
            if (!achieved[t]) budget--;
            t++;
        }
        if (t - 1 < bestI) return;
    }
    int hi = min(HB, nn);
    for (int p = 0; p < n; p++) {
        if (usedMask & (1 << p)) continue;
        for (int v = hi; v >= lastVal; v--) {
            a[p] = v;
            int newly[40];
            int cnt = markArcs(newly);
            dfs(rank + 1, v, sumSoFar + v, usedMask | (1 << p));
            for (int idx = 0; idx < cnt; idx++) { achieved[newly[idx]] = false; distinctCount--; }
            a[p] = -1;
        }
    }
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    HB = m + n * (n - 1) + 1;
    if (HB < m) HB = m; // safety, shouldn't trigger
    maxDistinctTotal = n * (n - 1) + 1;
    memset(achieved, 0, sizeof(achieved));
    distinctCount = 0;
    for (int i = 0; i < n; i++) a[i] = -1;
    bestI = m - 1;

    // Position 0 always receives the globally smallest value (WLOG, since
    // every rotation/reflection starting at a minimum-valued sector is
    // discovered as its own array via the position choice inside dfs()).
    // Try small minimum values first: they are the ones real optimal circles
    // tend to use (a small anchor leaves more of the n*(n-1)+1 sum budget
    // available for combinations), so this finds a strong bestI early and
    // lets the pruning above engage sooner for the rest of the search.
    int nn0 = nextMissing();
    int hi0 = min(HB, nn0);
    for (int v = k; v <= hi0; v++) {
        a[0] = v;
        int newly[40];
        int cnt = markArcs(newly);
        dfs(1, v, v, 1);
        for (int idx = 0; idx < cnt; idx++) { achieved[newly[idx]] = false; distinctCount--; }
        a[0] = -1;
    }

    sort(sols.begin(), sols.end());
    sols.erase(unique(sols.begin(), sols.end()), sols.end());

    printf("%d\n", bestI);
    for (size_t s = 0; s < sols.size(); s++) {
        for (int i = 0; i < n; i++) {
            if (i) printf(" ");
            printf("%d", sols[s][i]);
        }
        printf("\n");
    }
    return 0;
}
