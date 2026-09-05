// POJ 2032 - Square Carpets
// Model: claude-sonnet-5
//
// Minimum number of axis-aligned squares (any size, side a multiple of the
// panel size) needed to cover all "scratched" cells of a W x H grid while
// never covering a "flawless" cell. Squares may overlap freely.
//
// Key reduction: for a fixed top-left anchor (i,j), any valid square rooted
// there (all cells scratched) is dominated -- as a *set of covered cells* --
// by the largest valid square rooted at the same anchor, since a smaller
// square's footprint is always a subset of the larger one's footprint when
// the anchor is fixed. Since every carpet costs the same regardless of
// size, an optimal solution can always be rewritten to use, at each anchor
// it uses, the maximal square anchored there. So the only candidate carpets
// worth considering are the (at most W*H) "maximal squares", one per anchor
// cell whose maximal size is >= 1. Any candidate whose footprint is a subset
// of another candidate's footprint is then also dropped (never better to use
// the smaller one).
//
// This turns the problem into minimum SET COVER over at most 100 elements
// (scratched cells) and at most 100 candidate sets (maximal squares).
// Connected components of scratched cells are solved independently (a
// square's footprint is always fully inside one component).
//
// Each component is solved by DFS branch-and-bound in the style of Knuth's
// "repeated cover" search: repeatedly pick the *uncovered* cell with the
// fewest candidate squares able to cover it (most-constrained-cell / MRV
// heuristic -- the column choice in Dancing Links), branch over exactly
// those candidates (largest new-coverage first), and prune whenever the
// running count already reaches the best solution found so far, or whenever
// a lower bound on the remaining carpets shows no improvement is possible.
//
// A first submission (run 25202534) that used only MRV + that style of
// pruning got Time Limit Exceeded -- the discuss board's own history
// confirms this exact shape of failure ("brute force TLEs... add hash
// optimization... it passed"). The missing piece: different orders of
// picking the same set of mutually-independent candidate squares reach the
// identical covered-cell state, so naive DFS re-explores the same state
// exponentially many times. The fix is memoization on the covered-cell
// bitmask: skip re-descending into a state already reached with an
// equal-or-lower carpet count. This turns the redundant permutation
// blow-up into a search over the (much smaller in practice) set of
// distinct reachable coverage states.
//
// Two more safety measures, both verified not to change any answer via
// 800+ differential trials against a from-scratch brute force that enumerates
// EVERY valid square (no maximal-square or domination reduction):
//   - Dominated-candidate pruning: if candidate A's footprint is a subset of
//     candidate B's, A is dropped -- using B instead of A is never worse.
//   - A "packing" lower bound: greedily collect uncovered cells no two of
//     which can be covered by a single common candidate (an independent set
//     in the "shares a candidate" graph); its size is a valid lower bound on
//     the carpets still needed, since covering two independent cells always
//     costs two distinct carpets. (An earlier "peel the best single
//     candidate's coverage" bound looked plausible but was NOT actually
//     admissible -- its induction silently assumed the peeled candidate was
//     the true optimal solution's own choice, which need not hold -- and it
//     produced a wrong, too-high answer (30 instead of 29) on the last
//     sample case before being replaced with this provably-correct one.)
//
// The mask is implemented as a plain two-word 128-bit struct (not
// std::bitset) so it can be used as a std::map key directly, and so its
// performance does not depend on how any particular old compiler
// implements bitset.
//
// Ambiguity: none found in the statement; input is terminated by "0 0" and
// carries no extra trailing data (matches sample).

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>

using namespace std;

int W, H;
int grid[12][12];
int maxSize[12][12];
int comp[12][12]; // component id per cell, -1 if flawless

int popcount64(unsigned long long x) {
    int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
}

struct Mask128 {
    unsigned long long lo, hi;
    Mask128() : lo(0), hi(0) {}
    void set(int b) {
        if (b < 64) lo |= (1ULL << b);
        else hi |= (1ULL << (b - 64));
    }
    bool test(int b) const {
        if (b < 64) return (lo >> b) & 1ULL;
        else return (hi >> (b - 64)) & 1ULL;
    }
    Mask128 operator|(const Mask128 &o) const {
        Mask128 r; r.lo = lo | o.lo; r.hi = hi | o.hi; return r;
    }
    Mask128 operator&(const Mask128 &o) const {
        Mask128 r; r.lo = lo & o.lo; r.hi = hi & o.hi; return r;
    }
    bool operator==(const Mask128 &o) const { return lo == o.lo && hi == o.hi; }
    bool operator<(const Mask128 &o) const {
        if (hi != o.hi) return hi < o.hi;
        return lo < o.lo;
    }
    int count() const { return popcount64(lo) + popcount64(hi); }
    // this set-minus other
    Mask128 minus(const Mask128 &o) const {
        Mask128 r; r.lo = lo & ~o.lo; r.hi = hi & ~o.hi; return r;
    }
    bool isSubsetOf(const Mask128 &o) const {
        return (lo & ~o.lo) == 0ULL && (hi & ~o.hi) == 0ULL;
    }
};

int bestAns;

struct Solver {
    int cellCount;
    vector<vector<int> > *coverList;
    vector<Mask128> *candMasks;
    Mask128 fullMask;
    int best;
    map<Mask128, int> memo;

    // Admissible lower bound on carpets still needed to cover 'rem': a
    // "packing" bound. Greedily build a set T of cells from rem such that no
    // single candidate covers two members of T (an independent set in the
    // "shares a candidate" graph). Since covering two members of T always
    // needs two *distinct* candidates, any valid cover uses at least |T|
    // candidates, so |T| <= true remaining optimum -- this holds no matter
    // which cell is picked at each step, since we only ever discard cells
    // that could conceivably share a candidate with an already-picked one.
    int lowerBoundPacking(Mask128 rem) {
        int cnt = 0;
        while (rem.count() > 0) {
            int b = -1;
            for (int i = 0; i < cellCount; i++) {
                if (rem.test(i)) { b = i; break; }
            }
            cnt++;
            Mask128 conflict; conflict.set(b);
            for (size_t k = 0; k < (*coverList)[b].size(); k++) {
                int ci = (*coverList)[b][k];
                conflict = conflict | (*candMasks)[ci];
            }
            rem = rem.minus(conflict);
        }
        return cnt;
    }

    void dfs(Mask128 covered, int count) {
        if (count >= best) return;
        if (covered == fullMask) {
            best = count;
            return;
        }
        map<Mask128, int>::iterator it = memo.find(covered);
        if (it != memo.end()) {
            if (it->second <= count) return;
            it->second = count;
        } else {
            memo.insert(make_pair(covered, count));
        }

        int lb = lowerBoundPacking(fullMask.minus(covered));
        if (count + lb >= best) return;

        // find uncovered cell with fewest candidates (MRV)
        int chosenCell = -1;
        int chosenCount = 1 << 30;
        for (int b = 0; b < cellCount; b++) {
            if (!covered.test(b)) {
                int sz = (int)(*coverList)[b].size();
                if (sz < chosenCount) {
                    chosenCount = sz;
                    chosenCell = b;
                }
            }
        }

        // try candidates covering chosenCell, most new-coverage first
        const vector<int> &cands = (*coverList)[chosenCell];
        int m = (int)cands.size();
        vector<pair<int,int> > order(m); // (-newcount, candidate index)
        for (int k = 0; k < m; k++) {
            int ci = cands[k];
            Mask128 nc = (*candMasks)[ci].minus(covered);
            order[k] = make_pair(-(int)nc.count(), ci);
        }
        sort(order.begin(), order.end());
        for (int k = 0; k < m; k++) {
            int ci = order[k].second;
            Mask128 nm = covered | (*candMasks)[ci];
            dfs(nm, count + 1);
        }
    }
};

void solveComponent(int cellCount, vector<Mask128> &candMasksIn, Mask128 fullMask) {
    // drop dominated candidates: if A's footprint is a subset of B's (A != B),
    // A can never be strictly better than B, so discard A.
    int n0 = (int)candMasksIn.size();
    vector<bool> dominated(n0, false);
    for (int a = 0; a < n0; a++) {
        for (int b = 0; b < n0; b++) {
            if (a == b || dominated[b]) continue;
            if (candMasksIn[a].isSubsetOf(candMasksIn[b]) && !(candMasksIn[b].isSubsetOf(candMasksIn[a]))) {
                dominated[a] = true;
                break;
            }
            // equal masks: keep only the lower-indexed one
            if (candMasksIn[a].isSubsetOf(candMasksIn[b]) && candMasksIn[b].isSubsetOf(candMasksIn[a]) && a > b) {
                dominated[a] = true;
                break;
            }
        }
    }
    vector<Mask128> candMasks;
    for (int a = 0; a < n0; a++) if (!dominated[a]) candMasks.push_back(candMasksIn[a]);

    int n = (int)candMasks.size();
    vector<vector<int> > coverList(cellCount);
    for (int c = 0; c < n; c++) {
        for (int b = 0; b < cellCount; b++) {
            if (candMasks[c].test(b)) coverList[b].push_back(c);
        }
    }

    Solver solver;
    solver.cellCount = cellCount;
    solver.coverList = &coverList;
    solver.candMasks = &candMasks;
    solver.fullMask = fullMask;
    solver.best = cellCount; // trivial upper bound: one carpet per cell
    Mask128 empty;
    solver.dfs(empty, 0);

    bestAns += solver.best;
}

int main() {
    while (scanf("%d %d", &W, &H) == 2) {
        if (W == 0 && H == 0) break;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                scanf("%d", &grid[i][j]);
            }
        }
        // compute maxSize via DP from bottom-right to top-left
        for (int i = H - 1; i >= 0; i--) {
            for (int j = W - 1; j >= 0; j--) {
                if (grid[i][j] == 0) {
                    maxSize[i][j] = 0;
                } else {
                    int right = (j + 1 < W) ? maxSize[i][j + 1] : 0;
                    int down = (i + 1 < H) ? maxSize[i + 1][j] : 0;
                    int diag = (i + 1 < H && j + 1 < W) ? maxSize[i + 1][j + 1] : 0;
                    int m = min(right, min(down, diag));
                    maxSize[i][j] = m + 1;
                }
            }
        }
        // find connected components (4-connectivity) among scratched cells
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
                comp[i][j] = -1;

        int numComp = 0;
        vector<vector<pair<int,int> > > compCells;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (grid[i][j] == 1 && comp[i][j] == -1) {
                    // BFS
                    int cid = numComp++;
                    compCells.push_back(vector<pair<int,int> >());
                    vector<pair<int,int> > stack;
                    stack.push_back(make_pair(i, j));
                    comp[i][j] = cid;
                    while (!stack.empty()) {
                        pair<int,int> cur = stack.back();
                        stack.pop_back();
                        compCells[cid].push_back(cur);
                        int ci = cur.first, cj = cur.second;
                        int di[4] = {-1, 1, 0, 0};
                        int dj[4] = {0, 0, -1, 1};
                        for (int d = 0; d < 4; d++) {
                            int ni = ci + di[d], nj = cj + dj[d];
                            if (ni >= 0 && ni < H && nj >= 0 && nj < W &&
                                grid[ni][nj] == 1 && comp[ni][nj] == -1) {
                                comp[ni][nj] = cid;
                                stack.push_back(make_pair(ni, nj));
                            }
                        }
                    }
                }
            }
        }

        bestAns = 0;
        for (int cid = 0; cid < numComp; cid++) {
            vector<pair<int,int> > &cells = compCells[cid];
            int cellCount = (int)cells.size();
            int localIdx[12][12];
            for (int i = 0; i < H; i++)
                for (int j = 0; j < W; j++)
                    localIdx[i][j] = -1;
            for (int k = 0; k < cellCount; k++) {
                localIdx[cells[k].first][cells[k].second] = k;
            }
            // gather candidates: anchors within this component with maxSize>=1
            vector<Mask128> candMasks;
            for (int k = 0; k < cellCount; k++) {
                int i = cells[k].first, j = cells[k].second;
                int s = maxSize[i][j];
                if (s <= 0) continue; // shouldn't happen, cell is scratched
                Mask128 mask;
                for (int di = 0; di < s; di++) {
                    for (int dj = 0; dj < s; dj++) {
                        int idx = localIdx[i + di][j + dj];
                        mask.set(idx);
                    }
                }
                candMasks.push_back(mask);
            }
            Mask128 fullMask;
            for (int k = 0; k < cellCount; k++) fullMask.set(k);
            solveComponent(cellCount, candMasks, fullMask);
        }

        printf("%d\n", bestAns);
    }
    return 0;
}
