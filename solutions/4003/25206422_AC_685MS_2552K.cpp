// POJ 4003 - Bob's Race
// Model: claude-sonnet-5
//
// For each house i, define f(i) = the length of the longest simple path
// starting at i in the tree (the "eccentricity" of i). For a tree with
// nonnegative edge weights, f(i) = max(dist(i,u), dist(i,v)) where u,v are
// the two endpoints of a diameter of the tree (found by: BFS from any node
// to get farthest node u, BFS from u to get farthest node v). This is a
// standard fact about trees.
//
// Bob's query: given Q, find the maximum length of a contiguous range of
// house numbers [l,r] such that max(f[l..r]) - min(f[l..r]) <= Q. This is
// the classic "longest subarray with max-min <= limit" problem, solved with
// a two-pointer sweep using two monotonic deques (max-deque, min-deque) in
// O(N) per query (same technique as POJ 3162 / LeetCode 1438). With
// N<=50000 and M<=500 this is O(N*M) per test case, well within the time
// limit (confirmed on the discuss board: an O(N)-per-query solution using
// 32-bit int passes around 1300ms, while switching distances to 64-bit
// __int64 caused TLE - distances fit comfortably in 32-bit int since
// N<=50000, z<=5000 gives a max path length of about 2.5*10^8).
//
// Sample verified by hand: chain 1-2-3-4-5 with weights 3,4,2,3 gives
// f = [12,9,7,9,12], and the sliding-window answers for Q=1..5 are
// 1,3,3,3,5, matching the official sample output.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 50005;
const int MAXE = 2 * MAXN;

int head[MAXN], nxt[MAXE], to_[MAXE], wt[MAXE], ecnt;
int distU[MAXN], distV[MAXN];
int f[MAXN];
int q_[MAXN];
int dqMax[MAXN], dqMin[MAXN];

void addEdge(int u, int v, int w) {
    to_[ecnt] = v; wt[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt++;
}

void bfs(int src, int n, int *dist) {
    for (int i = 1; i <= n; i++) dist[i] = -1;
    int qh = 0, qt = 0;
    q_[qt++] = src;
    dist[src] = 0;
    while (qh < qt) {
        int u = q_[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to_[e];
            if (dist[v] == -1) {
                dist[v] = dist[u] + wt[e];
                q_[qt++] = v;
            }
        }
    }
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        ecnt = 0;
        for (int i = 1; i <= n; i++) head[i] = -1;
        for (int i = 0; i < n - 1; i++) {
            int x, y, z;
            scanf("%d %d %d", &x, &y, &z);
            addEdge(x, y, z);
            addEdge(y, x, z);
        }

        bfs(1, n, distU);
        int u = 1;
        for (int i = 1; i <= n; i++) if (distU[i] > distU[u]) u = i;
        bfs(u, n, distU);
        int v = 1;
        for (int i = 1; i <= n; i++) if (distU[i] > distU[v]) v = i;
        bfs(v, n, distV);
        for (int i = 1; i <= n; i++) f[i] = distU[i] > distV[i] ? distU[i] : distV[i];

        for (int qi = 0; qi < m; qi++) {
            int Q;
            scanf("%d", &Q);
            int mqMaxL = 0, mqMaxR = -1;
            int mqMinL = 0, mqMinR = -1;
            int l = 1, best = 0;
            for (int r = 1; r <= n; r++) {
                while (mqMaxR >= mqMaxL && f[dqMax[mqMaxR]] <= f[r]) mqMaxR--;
                dqMax[++mqMaxR] = r;
                while (mqMinR >= mqMinL && f[dqMin[mqMinR]] >= f[r]) mqMinR--;
                dqMin[++mqMinR] = r;
                while (f[dqMax[mqMaxL]] - f[dqMin[mqMinL]] > Q) {
                    l++;
                    if (dqMax[mqMaxL] < l) mqMaxL++;
                    if (dqMin[mqMinL] < l) mqMinL++;
                }
                if (r - l + 1 > best) best = r - l + 1;
            }
            printf("%d\n", best);
        }
    }
    return 0;
}
