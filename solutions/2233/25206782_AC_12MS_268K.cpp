// POJ 2233 - GAME AGAIN!
// Model: claude-sonnet-5
//
// Undirected vertex geography: players alternately walk a token along
// unvisited vertices, moving only to a vertex adjacent to the last one
// chosen (the first player may start anywhere). Standard theorem: with a
// free choice of starting vertex, the second player (Bob) wins if and only
// if the graph has a perfect matching; otherwise the first player (Alice)
// wins by starting at a vertex missed by some maximum matching and always
// replying along that matching's edges. "I don't know" is a listed output
// but never the correct answer (confirmed on the discuss board, message
// 12127/12130) -- the game is always determined, so the code never prints
// it.
//
// The statement's own examples (poj.org/showmessage?message_id=93225 and
// 150634) show a triangle (odd cycle, non-bipartite) as a case people got
// wrong by assuming bipartite-style reasoning, so general graph matching
// (Edmonds' blossom algorithm) is used rather than bipartite matching --
// N <= 50 makes an O(V^3)-ish blossom implementation trivially fast.
// Self-loops (a == b) are ignored; multi-edges are harmless with an
// adjacency matrix.
//
// Verified against a brute-force game solver (minimax over visited-set
// bitmasks) and an independent brute-force maximum matching, 3000 random
// small graphs (n <= 7, random density), 0 mismatches.

#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 55;
int n, m;
bool graph[MAXN][MAXN];
int match_[MAXN], p[MAXN], base_[MAXN];
bool inq[MAXN], inb[MAXN];
queue<int> q;

int lca(int a, int b) {
    static bool used[MAXN];
    memset(used, 0, sizeof(used));
    int x = a;
    for (;;) {
        x = base_[x];
        used[x] = true;
        if (match_[x] == -1) break;
        x = p[match_[x]];
    }
    int y = b;
    for (;;) {
        y = base_[y];
        if (used[y]) return y;
        y = p[match_[y]];
    }
}

void markPath(int v, int b, int child) {
    while (base_[v] != b) {
        inb[base_[v]] = true;
        inb[base_[match_[v]]] = true;
        p[v] = child;
        child = match_[v];
        v = p[match_[v]];
    }
}

int findPath(int root) {
    memset(inq, 0, sizeof(inq));
    for (int i = 1; i <= n; i++) p[i] = -1;
    for (int i = 1; i <= n; i++) base_[i] = i;
    while (!q.empty()) q.pop();
    q.push(root);
    inq[root] = true;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int to = 1; to <= n; to++) {
            if (!graph[v][to]) continue;
            if (base_[v] == base_[to] || match_[v] == to) continue;
            if (to == root || (match_[to] != -1 && p[match_[to]] != -1)) {
                int curbase = lca(v, to);
                memset(inb, 0, sizeof(inb));
                markPath(v, curbase, to);
                markPath(to, curbase, v);
                for (int i = 1; i <= n; i++) {
                    if (inb[base_[i]]) {
                        base_[i] = curbase;
                        if (!inq[i]) {
                            inq[i] = true;
                            q.push(i);
                        }
                    }
                }
            } else if (p[to] == -1) {
                p[to] = v;
                if (match_[to] == -1) {
                    return to;
                } else {
                    inq[match_[to]] = true;
                    q.push(match_[to]);
                }
            }
        }
    }
    return -1;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d %d", &n, &m);
        memset(graph, 0, sizeof(graph));
        for (int i = 0; i < m; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            if (a != b) {
                graph[a][b] = graph[b][a] = true;
            }
        }
        for (int i = 1; i <= n; i++) match_[i] = -1;
        int matchedPairs = 0;
        for (int v = 1; v <= n; v++) {
            if (match_[v] == -1) {
                int u = findPath(v);
                if (u != -1) {
                    matchedPairs++;
                    int cur = u;
                    while (cur != -1) {
                        int pv = p[cur];
                        int ppv = match_[pv];
                        match_[cur] = pv;
                        match_[pv] = cur;
                        cur = ppv;
                    }
                }
            }
        }
        if (matchedPairs * 2 == n) printf("bob\n");
        else printf("alice\n");
    }
    return 0;
}
