// POJ 1636 - Prison rearrangement
// Model: claude-sonnet-5
//
// Approach: Prisoner x of prison 1 and prisoner y of prison 2 must not end
// up in the same prison after the swap. x ends in prison 1 unless it is
// moved, y ends in prison 2 unless it is moved. So a dangerous pair (x,y)
// ends up together exactly when "x moved" != "y moved" -- i.e. safety
// requires x and y to share the same moved/not-moved status. This ties
// prison-1 nodes and prison-2 nodes together via a union-find over 2*m
// nodes (edge per dangerous pair); every node inside one connected
// component must move together or stay together.
//
// For each component let a = #prison-1 members, b = #prison-2 members.
// Choosing to move a component contributes a to the moved-count from
// prison 1 and b to the moved-count from prison 2; for a valid k-for-k
// exchange the selected components must have sum(a) == sum(b) == k, i.e.
// sum(a-b) == 0 over the selected set, and k is that common sum, capped by
// k <= m/2. This is a 0/1 knapsack over components with two coupled
// dimensions (net a-b difference, and total a moved): dp[diff] is a
// bitset over achievable "a moved" totals for that net difference. Answer
// is the largest k <= floor(m/2) with dp[0][k] set (diff measured from an
// offset since it can go negative).
//
// Ambiguity: the statement doesn't say prisoner numbering is independent
// per prison, and the discuss board confirms IDs can coincide between the
// two prisons (a pair "5 5" means prisoner 5 of prison 1 vs prisoner 5 of
// prison 2, unrelated people) -- handled here by giving prison-1 and
// prison-2 nodes disjoint index ranges in the union-find.
#include <cstdio>
#include <cstring>
#include <bitset>
#include <vector>
using namespace std;

const int MAXM = 205;
const int MAXBIT = 210;       // > MAXM, headroom for "a moved" totals
const int MAXDIFF = 2 * MAXM + 5;

int parent[2 * MAXM];
int cnt1[2 * MAXM], cnt2[2 * MAXM];

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) parent[a] = b;
}

bitset<MAXBIT> dp[MAXDIFF];
bitset<MAXBIT> ndp[MAXDIFF];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int m, r;
        scanf("%d %d", &m, &r);
        int total = 2 * m;
        for (int i = 0; i < total; i++) parent[i] = i;
        for (int i = 0; i < r; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            unite(x - 1, m + (y - 1));
        }
        for (int i = 0; i < total; i++) { cnt1[i] = 0; cnt2[i] = 0; }
        for (int i = 0; i < m; i++) cnt1[find(i)]++;
        for (int i = 0; i < m; i++) cnt2[find(m + i)]++;

        int offset = m;
        int diffSize = 2 * m + 1;
        for (int d = 0; d < diffSize; d++) dp[d].reset();
        dp[offset][0] = 1;

        for (int i = 0; i < total; i++) {
            if (find(i) != i) continue;
            int a = cnt1[i], b = cnt2[i];
            if (a == 0 && b == 0) continue;
            int delta = a - b;
            for (int d = 0; d < diffSize; d++) ndp[d] = dp[d];
            for (int d = 0; d < diffSize; d++) {
                if (dp[d].any()) {
                    int nd = d + delta;
                    if (nd >= 0 && nd < diffSize) {
                        ndp[nd] |= (dp[d] << a);
                    }
                }
            }
            for (int d = 0; d < diffSize; d++) dp[d] = ndp[d];
        }

        int half = m / 2;
        int ans = 0;
        for (int k = half; k >= 0; k--) {
            if (dp[offset][k]) { ans = k; break; }
        }
        printf("%d\n", ans);
    }
    return 0;
}
