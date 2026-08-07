// POJ 2373 - Dividing the Path
// Model: claude-opus-5
//
// Each sprinkler with radius r covers [p-r, p+r], a segment of even length 2r,
// and the ridge must be tiled exactly with no overlap and no spill past either
// end.  So a solution is a partition of [0, L] into consecutive segments whose
// lengths are even and lie in [2A, 2B]; every segment boundary is therefore at
// an even coordinate.  Work in half units: j = x/2, m = L/2, and a segment
// spans r half units with A <= r <= B.
//
// A cow's range [S, E] must sit inside one segment, i.e. no boundary may fall
// strictly inside it.  Boundaries are the even x, so the forbidden j are
// S/2+1 .. (E-1)/2 (from S < 2j < E); mark them with a difference array.
//
// dp[j] = fewest sprinklers tiling [0, 2j] exactly, INF if j is a forbidden
// boundary or unreachable; dp[j] = 1 + min(dp[j-B..j-A]).  The sliding-window
// minimum is a monotone deque, so the whole thing is O(L).  Answer dp[m], or
// -1 when it is unreachable (this covers A > m and a cow range longer than 2B).
//
// Ambiguity: "covers each location by exactly one sprinkler" is an exact tiling
// (the hint confirms touching endpoints do not count as overlap), and a cow
// range shares a boundary freely -- only a boundary strictly inside it is bad.
// The discuss board's four test cases (3 6 / 1 2 / 1 3 / 3 4 / 5 6 -> 2;
// 2 8 / 10 20 / ... -> -1; 2 8 / 1 20 / 0 4 / 2 8 -> 1; 0 6 / 2 2 -> -1) all
// reproduce, and the board also reports G++ (language 0) giving CE where C++
// (language 4) accepts.

#include <cstdio>

static const int INF = 1000000000;

static int bad[500005];   /* bad[j] != 0  =>  cut at position 2*j is forbidden */
static int f[500005];
static int dq[500005];

int main() {
    int n, L, A, B;
    while (scanf("%d %d", &n, &L) == 2) {
        if (scanf("%d %d", &A, &B) != 2) return 0;
        int m = L / 2;
        int i, j;
        for (j = 0; j <= m + 1; ++j) bad[j] = 0;
        for (i = 0; i < n; ++i) {
            int S, E;
            if (scanf("%d %d", &S, &E) != 2) return 0;
            int lo = S / 2 + 1;
            int hi = (E - 1) / 2;
            if (lo < 0) lo = 0;
            if (hi > m) hi = m;
            if (lo <= hi) { bad[lo] += 1; bad[hi + 1] -= 1; }
        }
        int run = 0;
        for (j = 0; j <= m; ++j) { run += bad[j]; bad[j] = (run > 0) ? 1 : 0; }

        int head = 0, tail = 0; /* indices with increasing f, front = window min */
        f[0] = 0;
        for (j = 1; j <= m; ++j) {
            int cand = j - A;
            if (cand >= 0 && f[cand] < INF) {
                while (tail > head && f[dq[tail - 1]] >= f[cand]) --tail;
                dq[tail++] = cand;
            }
            while (tail > head && dq[head] < j - B) ++head;
            if (bad[j] || tail == head) f[j] = INF;
            else f[j] = f[dq[head]] + 1;
        }
        printf("%d\n", f[m] >= INF ? -1 : f[m]);
    }
    return 0;
}
