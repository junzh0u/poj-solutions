// POJ 2111 - Millenium Leapcow
// Model: claude-opus-5
//
// Longest strictly-increasing knight path on an N x N board (N <= 365) holding
// a permutation of 1..N*N; the score is the number of squares visited.
//
// Sort the cells by value descending and relax in that order, so every cell's
// successors are already final: dp[c] = 1 + max{ dp[t] : t a knight neighbour
// with val[t] > val[c] }, or 1 when there is none.  W = max dp.  O(8 N^2).
//
// The tie-break ("show the path that would be the smallest if the paths were
// sorted by comparing their respective square numbers") is a lexicographic
// minimisation over the sequence of square numbers.  Since every candidate path
// has the same length W, a forward greedy is exact: start from the smallest
// value with dp == W, then at each step take the smallest neighbour value among
// successors with dp == remaining-1.  Both tie-breaks matter -- the discuss
// board reports Wrong Answers from omitting either, and the official sample
// catches neither (mutants of both survive it; a randomized differential test
// against a brute force enumerating all maximal paths kills them at N = 3).
//
// The statement guarantees distinct values, so the greedy's candidate set is
// always a single cell; it is kept as a set anyway, which makes the answer
// correct even on boards with repeated numbers (all comparisons are strict).

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

static const int MAXN = 366;
static const int DX[8] = { 1, 1, -1, -1, 2, 2, -2, -2 };
static const int DY[8] = { 2, -2, 2, -2, 1, -1, 1, -1 };

static int n;
static int val[MAXN * MAXN];
static int dp[MAXN * MAXN];
static int ord[MAXN * MAXN];

struct ByValDesc {
    bool operator()(int a, int b) const { return val[a] > val[b]; }
};

int main() {
    if (scanf("%d", &n) != 1) return 0;
    int tot = n * n;
    for (int i = 0; i < tot; i++) {
        if (scanf("%d", &val[i]) != 1) return 0;
        ord[i] = i;
    }
    sort(ord, ord + tot, ByValDesc());

    int best = 0;
    for (int k = 0; k < tot; k++) {
        int c = ord[k];
        int r = c / n, cc = c % n;
        int b = 1;
        for (int d = 0; d < 8; d++) {
            int nr = r + DX[d], nc = cc + DY[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
            int t = nr * n + nc;
            if (val[t] > val[c] && dp[t] + 1 > b) b = dp[t] + 1;
        }
        dp[c] = b;
        if (b > best) best = b;
    }

    printf("%d\n", best);

    /* Lexicographically smallest sequence of square numbers among all paths of
       length `best`.  Greedy: keep the set of cells reachable by the currently
       chosen prefix (all share the same value, so the prefix is fixed), then
       pick the smallest continuation value across the whole set. */
    vector<int> cur;
    int bestStart = 0;
    for (int i = 0; i < tot; i++) {
        if (dp[i] != best) continue;
        if (cur.empty() || val[i] < bestStart) {
            bestStart = val[i];
            cur.clear();
            cur.push_back(i);
        } else if (val[i] == bestStart) {
            cur.push_back(i);
        }
    }

    int remaining = best;
    int curVal = bestStart;
    while (true) {
        printf("%d\n", curVal);
        if (--remaining == 0) break;
        int nextVal = -1;
        vector<int> nxt;
        for (size_t s = 0; s < cur.size(); s++) {
            int c = cur[s];
            int r = c / n, cc = c % n;
            for (int d = 0; d < 8; d++) {
                int nr = r + DX[d], nc = cc + DY[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                int t = nr * n + nc;
                if (val[t] <= val[c] || dp[t] != remaining) continue;
                if (nextVal < 0 || val[t] < nextVal) {
                    nextVal = val[t];
                    nxt.clear();
                    nxt.push_back(t);
                } else if (val[t] == nextVal) {
                    nxt.push_back(t);
                }
            }
        }
        sort(nxt.begin(), nxt.end());
        nxt.erase(unique(nxt.begin(), nxt.end()), nxt.end());
        cur.swap(nxt);
        curVal = nextVal;
    }
    return 0;
}
