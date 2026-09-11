// POJ 2759 - Distributing tasks
// Model: claude-sonnet-5
//
// Two rows of n difficulty values each. We must cover the 2xn grid with m
// axis-aligned rectangular tasks that exactly tile it (no gaps/overlaps),
// minimizing the maximum task sum. Because the grid has only 2 rows, every
// valid tiling decomposes into: a set of disjoint column ranges assigned as
// a single "joint" task (both rows together), and, on the columns left
// over, each row split independently into its own contiguous single-row
// tasks (the two rows need not share cut points there).
//
// Binary search the answer kk. Feasibility check: f[i] = minimum tasks to
// exactly cover columns 1..i of both rows. Transition to f[i] either via a
// joint task ending at i (use the *maximal* feasible joint window, found
// with a monotonic two-pointer over the combined prefix sum -- extending a
// feasible joint window as far as possible is never worse, since f is
// monotone non-decreasing), or via some number of single-row tasks closing
// the gap between i and a synchronization point, discovered by repeatedly
// jumping back the more-advanced of two per-row "maximal reach" pointers
// (also two-pointer / monotonic, precomputed as bak1[i], bak2[i] = the
// earliest start of the longest single-row task ending at i). Every jump
// strictly decreases an index (validated impossible for a jump to stay put
// once we already know no single column exceeds kk), so the walk
// terminates; each meeting point (cur1<=cur2 or vice versa) yields a
// candidate f[meet] + (number of single-row tasks used so far). The walk
// is capped once the task count exceeds m, since it cannot help there
// after, bounding the whole check to O(n*m).
//
// All difficulty values are non-negative and their total is < 2^30 (per
// the statement / board), so plain 32-bit ints hold every sum without
// overflow.
//
// Verified against the official sample (answer 6) and against a brute
// force (state-space DP over independent per-row reach) on randomized
// small cases.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 10005;
int n, m;
int s1[MAXN], s2[MAXN];
int bak1[MAXN], bak2[MAXN];
int f[MAXN];

bool ok(int kk) {
    int l1 = 0, l2 = 0;
    for (int i = 1; i <= n; i++) {
        while (s1[i] - s1[l1] > kk) l1++;
        while (s2[i] - s2[l2] > kk) l2++;
        bak1[i] = l1;
        bak2[i] = l2;
        if (l1 == i || l2 == i) return false;
    }
    f[0] = 0;
    int j = 0;
    for (int i = 1; i <= n; i++) {
        f[i] = 2 * m;
        while (s1[i] + s2[i] - s1[j] - s2[j] > kk) j++;
        if (f[i] > f[j] + 1) f[i] = f[j] + 1;

        int c = 0;
        int cur1 = i, cur2 = i;
        while (cur1 > 0 || cur2 > 0) {
            c++;
            if (c > m) break;
            if (cur1 > cur2) cur1 = bak1[cur1];
            else cur2 = bak2[cur2];
            if (cur1 > cur2) {
                if (f[i] > f[cur1] + c) f[i] = f[cur1] + c;
            } else {
                if (f[i] > f[cur2] + c) f[i] = f[cur2] + c;
            }
        }
        if (f[i] > m) return false;
    }
    return f[n] <= m;
}

int main() {
    scanf("%d %d", &n, &m);
    s1[0] = s2[0] = 0;
    int ss = 0;
    for (int i = 1; i <= n; i++) {
        int v;
        scanf("%d", &v);
        s1[i] = s1[i - 1] + v;
        if (v > ss) ss = v;
    }
    for (int i = 1; i <= n; i++) {
        int v;
        scanf("%d", &v);
        s2[i] = s2[i - 1] + v;
        if (v > ss) ss = v;
    }
    int lo = ss, hi = s1[n] + s2[n];
    while (hi - lo > 1) {
        int mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid;
    }
    int ans = lo;
    while (!ok(ans)) ans++;
    printf("%d\n", ans);
    return 0;
}
