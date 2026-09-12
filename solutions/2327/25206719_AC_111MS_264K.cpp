// POJ 2327 - Dumb Bones
// Model: claude-sonnet-5
//
// Approach: DP over the length of an "isolated" segment of empty positions
// (bounded on both sides by true ends -- edges that can never be knocked
// down). Let E[m] be the expected number of placements needed to fill such
// a segment of length m from scratch, under an optimal strategy.
//
// From the problem statement (confirmed via the discuss board, message
// 113069): if you place a domino between two already-standing groups A and
// B, then with probability Pl the domino falls left and the *entire* group
// A is knocked down, with probability Pr the *entire* group B is knocked
// down, and otherwise (prob 1-Pl-Pr) the domino stands together with A and
// B, merging them into one group. Even with no adjacent group, a placement
// still "fails" with probability Pl+Pr and must be retried (it just topples
// itself with no collateral damage), which is why a single isolated domino
// needs an expected 1/(1-Pl-Pr) attempts rather than exactly 1.
//
// The optimal strategy for filling a fresh isolated gap of length m is:
// choose a split point, building the left part (length l) and right part
// (length r = m-1-l) completely and independently first (both are
// themselves isolated sub-problems, since nothing outside them has been
// touched yet), then attempt to place the single connecting domino between
// the two finished blocks. If that connecting attempt knocks down one
// side, that whole side must be rebuilt from scratch (cost E[l] or E[r]
// again) and the connecting attempt retried. Solving the resulting renewal
// equation for the expected cost M(l,r) of "merge two finished blocks of
// size l, r" gives:
//   M(l,r) = (1 + Pl*E[l] + Pr*E[r]) / (1 - Pl - Pr)
// and then
//   E[m] = min over l=0..m-1 of E[l] + E[r] + M(l,r), r = m-1-l.
// E[0] = 0. This closed form was checked against all three sample cases
// (46.25, 37.28, 20.00) before submission.
//
// Complexity: O(n^2) per test case, n <= 1000, <= 100 cases -- well within
// the 2s limit.
#include <cstdio>

static double E[1001];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        double Pl, Pr;
        scanf("%lf %lf", &Pl, &Pr);
        double q = Pl + Pr;
        double inv = 1.0 / (1.0 - q);
        E[0] = 0.0;
        for (int m = 1; m <= n; ++m) {
            double best = -1.0;
            for (int l = 0; l < m; ++l) {
                int r = m - 1 - l;
                double M = (1.0 + Pl * E[l] + Pr * E[r]) * inv;
                double val = E[l] + E[r] + M;
                if (best < 0 || val < best) best = val;
            }
            E[m] = best;
        }
        printf("%.2f\n", E[n]);
    }
    return 0;
}
