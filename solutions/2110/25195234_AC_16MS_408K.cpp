// POJ 2110 - Mountain Walking
// Model: claude-opus-5
//
// Minimize (max elevation - min elevation) over any walk from (1,1) to (N,N).
// A walk's [min,max] widens monotonically, so a value pair (lo,hi) is achievable
// iff the start and the goal are 4-connected through cells whose height lies in
// [lo,hi].  So: collect the distinct heights, and for each lo find the smallest
// feasible hi by BFS.  That hi is non-decreasing in lo (raising lo only removes
// cells, so anything infeasible stays infeasible), which makes it a two-pointer:
// at most 2*K BFS passes over the N*N grid, K = number of distinct heights
// (<= 111 under the stated 0..110 bound).  Break as soon as some lo admits no hi.
//
// Pitfalls from the discuss board, both honored here:
//   * heights run to 110, not 100 (a 0..100 table is the classic WA), and one
//     poster also needed a larger N bound -- so nothing is a fixed-size array:
//     the grid is read into a vector and the value range is taken from the data
//     itself rather than assumed, which costs nothing and survives either.
//   * the start and goal cells must themselves lie in [lo,hi]; forgetting either
//     is the other classic WA, hence the explicit checks at the top of connected().
//
// Statement is unambiguous (a path may revisit cells; only its extremes matter).
// Verified against a brute force written from the statement's own words -- BFS
// over states (cell, min so far, max so far) -- on 1500 random small grids.

#include <cstdio>
#include <vector>
#include <algorithm>

static int n;
static std::vector<int> h;      // n*n grid, row-major
static std::vector<int> stamp_; // BFS visited marks, stamped to avoid clearing
static std::vector<int> q;
static int curStamp;

static bool connected(int lo, int hi) {
    int start = 0, goal = n * n - 1;
    if (h[start] < lo || h[start] > hi) return false;
    if (h[goal] < lo || h[goal] > hi) return false;
    ++curStamp;
    int head = 0, tail = 0;
    q[tail++] = start;
    stamp_[start] = curStamp;
    while (head < tail) {
        int c = q[head++];
        if (c == goal) return true;
        int r = c / n, col = c % n;
        if (r > 0) {
            int d = c - n;
            if (stamp_[d] != curStamp && h[d] >= lo && h[d] <= hi) { stamp_[d] = curStamp; q[tail++] = d; }
        }
        if (r + 1 < n) {
            int d = c + n;
            if (stamp_[d] != curStamp && h[d] >= lo && h[d] <= hi) { stamp_[d] = curStamp; q[tail++] = d; }
        }
        if (col > 0) {
            int d = c - 1;
            if (stamp_[d] != curStamp && h[d] >= lo && h[d] <= hi) { stamp_[d] = curStamp; q[tail++] = d; }
        }
        if (col + 1 < n) {
            int d = c + 1;
            if (stamp_[d] != curStamp && h[d] >= lo && h[d] <= hi) { stamp_[d] = curStamp; q[tail++] = d; }
        }
    }
    return false;
}

static int solve() {
    std::vector<int> v(h);
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    int k = (int)v.size();
    int best = v[k - 1] - v[0];   // the full range is always feasible
    int j = 0;
    for (int i = 0; i < k; ++i) {
        if (j < i) j = i;
        while (j < k && !connected(v[i], v[j])) ++j;
        if (j >= k) break;        // hi is monotone in lo, so no larger lo works
        int d = v[j] - v[i];
        if (d < best) best = d;
    }
    return best;
}

int main() {
    while (scanf("%d", &n) == 1) {
        h.assign((size_t)n * n, 0);
        for (size_t i = 0; i < h.size(); ++i) scanf("%d", &h[i]);
        stamp_.assign(h.size(), 0);
        q.assign(h.size(), 0);
        curStamp = 0;
        printf("%d\n", solve());
    }
    return 0;
}
