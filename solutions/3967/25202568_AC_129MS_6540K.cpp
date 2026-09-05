// POJ 3967 - Ideal Path
// Model: claude-sonnet-5
// Approach: Path length is the number of edges (colors are labels, not
// weights), so BFS from room n gives dist[v] = shortest distance to n for
// every room. The answer length is k = dist[1]. To build the
// lexicographically smallest color sequence among all shortest paths, walk
// forward from room 1 maintaining a *frontier set* of rooms reachable by the
// best color prefix found so far (not a single room, since several rooms can
// tie on the same minimal color and only the union of their outgoing edges
// determines the true next minimal color). At each of the k steps: scan all
// edges out of the current frontier that lead to a room one step closer to
// n, take the minimum color among them, then rebuild the frontier as the set
// of rooms reached via an edge of exactly that color. Since every room has a
// single fixed dist value, it can only belong to the frontier at the one
// step matching that value, so the whole process is O(m) total. Board
// message 179954 explicitly warns that a naive single-node greedy walk (pick
// one room, always take its cheapest valid edge) is wrong for this reason;
// verified against the swapped-order duplicate-edge sample (colors 1 and 2
// between rooms 1 and 3) which only comes out right when the full frontier
// is considered. Undirected multigraph with self-loops permitted; self-loop
// edges are naturally skipped since dist[u] can never equal dist[u]-1.
// Ambiguity: none in the statement itself; the risk is purely algorithmic
// (see above).
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

static const int MAXN = 100005;
static const int MAXE = 400020; // 2 * m, m <= 200000

int head[MAXN];
int nxt_[MAXE];
int to_[MAXE];
int col_[MAXE];
int ecnt = 0;

static inline void addEdge(int u, int v, int c) {
    to_[ecnt] = v; col_[ecnt] = c; nxt_[ecnt] = head[u]; head[u] = ecnt++;
}

int dist_[MAXN];
int qarr[MAXN];
int mark_[MAXN];

// simple fast input
static inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    bool neg = false;
    if (c == '-') { neg = true; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return neg ? -x : x;
}

int main() {
    int n = readInt();
    int m = readInt();
    for (int i = 0; i <= n; ++i) head[i] = -1;
    for (int i = 0; i < m; ++i) {
        int a = readInt(), b = readInt(), c = readInt();
        addEdge(a, b, c);
        addEdge(b, a, c);
    }

    for (int i = 0; i <= n; ++i) dist_[i] = -1;
    dist_[n] = 0;
    int qh = 0, qt = 0;
    qarr[qt++] = n;
    while (qh < qt) {
        int u = qarr[qh++];
        for (int e = head[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                qarr[qt++] = v;
            }
        }
    }

    int k = dist_[1];

    vector<int> frontier, nextFrontier;
    frontier.reserve(n + 1);
    nextFrontier.reserve(n + 1);
    frontier.push_back(1);

    vector<int> ans(k > 0 ? k : 0);
    for (int i = 0; i <= n; ++i) mark_[i] = -1;

    for (int step = 0; step < k; ++step) {
        int D = dist_[frontier[0]];
        int minColor = 0x7fffffff;
        for (size_t i = 0; i < frontier.size(); ++i) {
            int u = frontier[i];
            for (int e = head[u]; e != -1; e = nxt_[e]) {
                int v = to_[e];
                if (dist_[v] == D - 1) {
                    if (col_[e] < minColor) minColor = col_[e];
                }
            }
        }
        ans[step] = minColor;
        nextFrontier.clear();
        for (size_t i = 0; i < frontier.size(); ++i) {
            int u = frontier[i];
            for (int e = head[u]; e != -1; e = nxt_[e]) {
                int v = to_[e];
                if (dist_[v] == D - 1 && col_[e] == minColor) {
                    if (mark_[v] != step) {
                        mark_[v] = step;
                        nextFrontier.push_back(v);
                    }
                }
            }
        }
        frontier.swap(nextFrontier);
    }

    printf("%d\n", k);
    for (int i = 0; i < k; ++i) {
        printf("%d%c", ans[i], (i + 1 < k) ? ' ' : '\n');
    }
    if (k == 0) printf("\n");
    return 0;
}
