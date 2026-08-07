// POJ 1465 - Multiple
// Model: claude-opus-5
//
// Smallest strictly positive multiple of N whose decimal digits all come from the
// given set.  BFS over the N residues mod N: a state is "remainder of the prefix
// built so far".  Seeds are the non-zero allowed digits in ascending order (a
// number has no leading zero); expanding a state appends every allowed digit in
// ascending order, r -> (10r + d) % N.  BFS explores by length, and within a level
// the queue stays in ascending numeric order, so the first visit to residue 0 is
// the smallest such multiple.  Answer length can reach ~N digits (N=4727 with digit
// {4} needs 2268 fours), so storing a string per state would blow the 32MB limit;
// instead each residue keeps only (parent residue, digit) and the answer is
// reconstructed backwards along that tree.  O(N * M) time, O(N) memory.
//
// Statement/judge notes:
//  * N may be 0.  Its only multiple is 0, which is not strictly positive, so the
//    answer is 0 -- and the mod must be skipped to avoid a division by zero.
//  * A digit set of {0} alone (or an empty one) also yields 0: every number built
//    from it is 0, never strictly positive.
//  * Data sets are separated by blank lines and run to EOF; whitespace-skipping
//    scanf handles both.  Digits are read defensively (deduplicated, out-of-range
//    values ignored) since the input is loosely specified.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

static const int MAXN = 5000;

static int par[MAXN];
static int dig[MAXN];
static bool vis[MAXN];
static int q[MAXN];
static int digs[32];
static char out[MAXN + 16];

int main() {
    int n, m;
    while (scanf("%d", &n) == 1) {
        if (scanf("%d", &m) != 1) m = 0;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            int x;
            if (scanf("%d", &x) != 1) break;
            if (cnt < 32) digs[cnt++] = x;
        }
        sort(digs, digs + cnt);
        cnt = (int)(unique(digs, digs + cnt) - digs);

        if (n == 0 || cnt == 0) { printf("0\n"); continue; }

        memset(vis, 0, sizeof(bool) * n);
        int head = 0, tail = 0;
        bool found = false;

        for (int i = 0; i < cnt && !found; i++) {
            if (digs[i] <= 0 || digs[i] > 9) continue;   /* no leading zero */
            int r = digs[i] % n;
            if (!vis[r]) {
                vis[r] = true; par[r] = -1; dig[r] = digs[i]; q[tail++] = r;
                if (r == 0) found = true;
            }
        }
        while (!found && head < tail) {
            int r = q[head++];
            for (int i = 0; i < cnt; i++) {
                if (digs[i] < 0 || digs[i] > 9) continue;
                int nr = (r * 10 + digs[i]) % n;
                if (!vis[nr]) {
                    vis[nr] = true; par[nr] = r; dig[nr] = digs[i]; q[tail++] = nr;
                    if (nr == 0) { found = true; break; }
                }
            }
        }

        if (!found) { printf("0\n"); continue; }

        int len = 0;
        int cur = 0;
        while (cur != -1) { out[len++] = (char)('0' + dig[cur]); cur = par[cur]; }
        for (int i = len - 1; i >= 0; i--) putchar(out[i]);
        putchar('\n');
    }
    return 0;
}
