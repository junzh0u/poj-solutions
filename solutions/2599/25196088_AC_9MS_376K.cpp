// POJ 2599 - A funny game
// Model: claude-opus-5
//
// The flights form a tree (n-1 edges, unique path between any pair).  A move
// flies from the current airport to a neighbour and destroys the airport just
// left, so the token can never come back: rooted at the start K, the game is
// exactly "walk down from parent to child", and a player who reaches a leaf
// loses.  Hence win(v) = OR over children c of !win(c), computed by one
// post-order DFS from K, and the answer is the smallest neighbour L of K with
// win(L) == false.
//
// Input framing (the whole trap here): the statement describes a single test
// case, but POJ's file concatenates several -- the discuss board reports
// "有多组数据" (2013) and, back in 2006, "some test cases has more than n-1
// lines after the first line ... numbers larger than n", which is just the
// next case's bigger N showing through.  The original is Timus 1045, one case
// per file, so POJ evidently glued the Timus test files together.  So read
// N K plus exactly N-1 edges, print one line, and repeat to EOF.
//
// DFS is iterative and indices are range-checked, so a deeper-than-stated tree
// cannot smash the stack and an out-of-range endpoint cannot smash an array.

#include <cstdio>

const int MAXN = 100005;

static int head[MAXN], nxt[2 * MAXN], dest_[2 * MAXN];
static int stk[MAXN], iter_[MAXN];
static bool win_[MAXN], vis[MAXN];

int main() {
    int n, k;
    while (scanf("%d %d", &n, &k) == 2) {
        if (n < 1 || n > 100000) break;
        int i;
        for (i = 1; i <= n; i++) { head[i] = -1; vis[i] = false; win_[i] = false; }
        int ecnt = 0;
        for (i = 0; i < n - 1; i++) {
            int a, b;
            if (scanf("%d %d", &a, &b) != 2) { a = 0; b = 0; }
            if (a >= 1 && a <= n && b >= 1 && b <= n && a != b) {
                dest_[ecnt] = b; nxt[ecnt] = head[a]; head[a] = ecnt++;
                dest_[ecnt] = a; nxt[ecnt] = head[b]; head[b] = ecnt++;
            }
        }
        if (k < 1 || k > n) break;

        int top = 0;
        stk[0] = k; iter_[k] = head[k]; vis[k] = true;
        while (top >= 0) {
            int v = stk[top];
            int e = iter_[v];
            if (e != -1) {
                iter_[v] = nxt[e];
                int u = dest_[e];
                if (!vis[u]) {
                    vis[u] = true; iter_[u] = head[u];
                    stk[++top] = u;
                }
            } else {
                top--;
                if (top >= 0) {
                    int p = stk[top];
                    if (!win_[v]) win_[p] = true;
                }
            }
        }

        int best = 0;
        for (int e = head[k]; e != -1; e = nxt[e]) {
            int u = dest_[e];
            if (!win_[u] && (best == 0 || u < best)) best = u;
        }
        if (best != 0) printf("First player wins flying to airport %d\n", best);
        else printf("First player loses\n");
    }
    return 0;
}
