// POJ 3443 - Team Work
// Model: claude-sonnet-5
//
// Approach: choose three disjoint subsets of the given pieces (some pieces
// may be left unused) that all sum to the same value T, each subset having
// at least 2 pieces (a stick must actually be glued together from more than
// one piece -- sample 4, ten pieces of length 25, answers 0, which rules out
// treating an un-glued single piece as one of the three sticks). Among all
// such partitions we additionally require that we can *order* the pieces
// within each of the three sticks so that no interior glue point (a partial
// sum strictly between 0 and T) repeats across two different sticks -- the
// literal reading of "none of the connection points coincide". We maximize T.
//
// Phase A is a standard equal-sum backtracking split of the (descending
// sorted) pieces into 3 buckets + unused, with the classic symmetry pruning
// (never try the same running bucket sum twice at one recursion level).
// Whenever a complete valid partition (equal sums, each bucket size >= 2)
// beats the current best, phase B checks whether *some* ordering of each
// bucket avoids interior-point collisions: it assigns an order to bucket 0,
// then (given the glue points that fixes) to bucket 1, then bucket 2, via a
// backtracking search over which remaining value to place next (deduplicated
// by value, since identical-length pieces are interchangeable), marking the
// value *before* the piece being appended (the true position of the new glue
// joint) as used -- the final length T itself is never marked, since it is
// the stick's endpoint, not a connection point.
//
// This was validated against a true brute force (which tries every
// assignment of pieces to {unused,1,2,3} and then every permutation of each
// bucket) on hundreds of small random cases with no disagreement, reproduces
// all four sample outputs exactly, and a mutation that checked the point
// *after* adding a piece instead of before (over-restrictive) breaks samples
// 1-3, while a mutation dropping the ">=2 pieces per stick" rule breaks only
// sample 4 -- confirming both rules are load-bearing and sample-checked.
//
// N <= 13, piece length <= 25, so T <= floor(13*25/3) = 108; well within
// int range, no overflow concerns.

#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;

int n;
int a[15];
int total;
int bestT;

int grp[3][15], grpSize[3];
int sums[3];

bool used[512];
int gcnt[3][26];

bool placeGroup(int gi, int remaining, int partial) {
    if (remaining == 0) {
        if (gi == 2) return true;
        return placeGroup(gi + 1, grpSize[gi + 1], 0);
    }
    for (int v = 1; v <= 25; v++) {
        if (gcnt[gi][v] == 0) continue;
        int newPartial = partial + v;
        bool conflict = false, marked = false;
        if (partial > 0) {
            if (used[partial]) {
                conflict = true;
            } else {
                used[partial] = true;
                marked = true;
            }
        }
        if (!conflict) {
            gcnt[gi][v]--;
            bool ok = placeGroup(gi, remaining - 1, newPartial);
            gcnt[gi][v]++;
            if (ok) return true;
        }
        if (marked) used[partial] = false;
    }
    return false;
}

bool orderFeasible() {
    memset(used, 0, sizeof(used));
    memset(gcnt, 0, sizeof(gcnt));
    for (int g = 0; g < 3; g++)
        for (int i = 0; i < grpSize[g]; i++)
            gcnt[g][grp[g][i]]++;
    return placeGroup(0, grpSize[0], 0);
}

void rec(int idx, int cnt0, int cnt1, int cnt2) {
    if (idx == n) {
        if (cnt0 >= 2 && cnt1 >= 2 && cnt2 >= 2 &&
            sums[0] == sums[1] && sums[1] == sums[2] && sums[0] > bestT) {
            if (orderFeasible()) bestT = sums[0];
        }
        return;
    }
    int cap = total / 3;
    int tried[3];
    int ntried = 0;
    int cnts[3] = {cnt0, cnt1, cnt2};
    for (int k = 0; k < 3; k++) {
        bool dup = false;
        for (int j = 0; j < ntried; j++)
            if (tried[j] == sums[k]) { dup = true; break; }
        if (dup) continue;
        tried[ntried++] = sums[k];
        if (sums[k] + a[idx] > cap) continue;
        grp[k][grpSize[k]++] = a[idx];
        sums[k] += a[idx];
        cnts[k]++;
        rec(idx + 1, cnts[0], cnts[1], cnts[2]);
        cnts[k]--;
        sums[k] -= a[idx];
        grpSize[k]--;
    }
    rec(idx + 1, cnt0, cnt1, cnt2);
}

int main() {
    int tc = 0;
    while (scanf("%d", &n) == 1 && n > 0) {
        tc++;
        total = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            total += a[i];
        }
        sort(a, a + n, greater<int>());
        bestT = 0;
        sums[0] = sums[1] = sums[2] = 0;
        grpSize[0] = grpSize[1] = grpSize[2] = 0;
        rec(0, 0, 0, 0);
        printf("Case %d: %d\n", tc, bestT);
    }
    return 0;
}
