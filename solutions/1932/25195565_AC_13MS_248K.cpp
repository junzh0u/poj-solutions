// POJ 1932 - XYZZY
// Model: claude-opus-5
//
// Longest path with a liveness constraint, plus positive-cycle detection.
//
// State: standing alive in room v with energy e.  The player starts alive in
// room 1 with e = 100; moving through a doorway into v gives e + w[v], and she
// dies the moment her energy runs out.  Since more energy is never worse, the
// single value best[v] = max energy achievable while alive in v is a sufficient
// summary of the state, and Bellman-Ford maximizes it.  Two guards encode the
// rules: a room may only be left while alive (best[u] > 0), and a move is only
// legal if it leaves her alive (best[u] + w[v] > 0).
//
// Ambiguity: "dies by running out of energy" - energy exactly 0 is death, not
// survival.  The discuss board supplies the separating case (1->3->2->4->5 with
// weights 0,0,+1,-101,0, where the best arrival at room 4 is exactly 0), which
// is hopeless; the official sample cannot tell the two readings apart.
//
// An update in round >= n cannot come from a simple path, so it proves the room
// sits on or after a positive cycle and its energy is unbounded.  From such a
// room every graph-reachable room is reachable alive (a simple path costs at
// most 100*(n-1)), so the answer is also winnable when the finish is reachable
// from any marked room - a positive cycle that cannot reach the finish is
// worthless, which is the board's most-reported wrong answer.

#include <cstdio>
#include <cstring>

const int MAXN = 105;

int n;
int w[MAXN];
int adjCnt[MAXN];
int adj[MAXN][MAXN];
int best[MAXN];      /* max energy achievable while alive in the room, 0 = unreached */
bool marked[MAXN];   /* on or after a positive cycle: energy is unbounded here */
bool seen[MAXN];
int stk[MAXN];

/* rooms reachable from any marked room, ignoring energy (it is unbounded there) */
bool reachesFinishFromMarked()
{
    int top = 0;
    memset(seen, 0, sizeof(seen));
    for (int i = 1; i <= n; i++)
        if (marked[i] && !seen[i]) { seen[i] = true; stk[top++] = i; }
    while (top > 0) {
        int u = stk[--top];
        if (u == n) return true;
        for (int j = 0; j < adjCnt[u]; j++) {
            int v = adj[u][j];
            if (!seen[v]) { seen[v] = true; stk[top++] = v; }
        }
    }
    return false;
}

bool solve()
{
    for (int i = 1; i <= n; i++) { best[i] = 0; marked[i] = false; }
    best[1] = 100;
    for (int round = 1; round <= 2 * n; round++) {
        bool changed = false;
        for (int u = 1; u <= n; u++) {
            if (best[u] <= 0) continue;
            for (int j = 0; j < adjCnt[u]; j++) {
                int v = adj[u][j];
                int nd = best[u] + w[v];
                if (nd > 0 && nd > best[v]) {
                    best[v] = nd;
                    changed = true;
                    if (round >= n) marked[v] = true;
                }
            }
        }
        if (!changed) break;
    }
    if (best[n] > 0) return true;
    return reachesFinishFromMarked();
}

int main()
{
    while (scanf("%d", &n) == 1 && n != -1) {
        for (int i = 1; i <= n; i++) {
            int m;
            scanf("%d %d", &w[i], &m);
            adjCnt[i] = m;
            for (int j = 0; j < m; j++)
                scanf("%d", &adj[i][j]);
        }
        printf("%s\n", solve() ? "winnable" : "hopeless");
    }
    return 0;
}
