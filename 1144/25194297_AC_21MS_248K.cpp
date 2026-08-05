// POJ 1144 - Network
//
// Count articulation points (cut vertices) of an undirected, initially
// connected graph on N (< 100) nodes.
//
// Since N is tiny, the graph is stored as an adjacency matrix rather than an
// adjacency list. This sidesteps the input's redundancy: "each direct
// connection ... is contained at least in one row", meaning an edge may be
// declared from either endpoint's line, or from both (duplicate). A matrix
// naturally dedupes any such repeats, so the classic Tarjan low-link
// articulation-point algorithm's parent-edge skip (`v != parent`) stays
// correct without needing to track multi-edges by index.
//
// Standard rule used: a non-root node u is an articulation point if it has a
// child v in the DFS tree with low[v] >= disc[u]; the DFS root is an
// articulation point instead iff it has 2 or more children in the DFS tree
// (removing it would leave those subtrees disconnected from each other).
//
// Input format ambiguity handled here: each block's node lines have a
// variable number of neighbours per line, and a line's end is what
// terminates that node's neighbour list -- there is no way to tell "still
// this node's neighbours" from "next node's id" in a flat token stream, so
// parsing is done line-by-line (via getline + istringstream) rather than
// token-by-token. A block ends with a line containing the single token 0,
// unambiguous because node ids run 1..N and 0 is never a valid node. The
// whole input ends with a block consisting of only a 0 for N.
//
// The problem guarantees each block's graph is connected, but the DFS loop
// still iterates over all unvisited nodes defensively (harmless if already
// connected, correct if not).

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static const int MAXN = 105;
bool adj[MAXN][MAXN];
int disc[MAXN], low[MAXN];
bool visited_[MAXN];
bool isArt[MAXN];
int N;
int timer_;

void dfs(int u, int parent) {
    visited_[u] = true;
    disc[u] = low[u] = ++timer_;
    int children = 0;
    for (int v = 1; v <= N; ++v) {
        if (!adj[u][v]) continue;
        if (!visited_[v]) {
            ++children;
            dfs(v, u);
            if (low[v] < low[u]) low[u] = low[v];
            if (parent != -1 && low[v] >= disc[u]) isArt[u] = true;
        } else if (v != parent) {
            if (disc[v] < low[u]) low[u] = disc[v];
        }
    }
    if (parent == -1 && children > 1) isArt[u] = true;
}

int main() {
    string line;
    while (getline(cin, line)) {
        // find first token as N
        {
            istringstream iss(line);
            if (!(iss >> N)) continue; // blank line, keep looking
        }
        if (N == 0) break;

        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= N; ++j)
                adj[i][j] = false;
        for (int i = 0; i < MAXN; ++i) { visited_[i] = false; isArt[i] = false; disc[i] = 0; low[i] = 0; }
        timer_ = 0;

        // read block lines until a line whose only token is 0
        while (getline(cin, line)) {
            istringstream iss(line);
            vector<int> nums;
            int x;
            while (iss >> x) nums.push_back(x);
            if (nums.empty()) continue; // skip stray blank lines
            if (nums.size() == 1 && nums[0] == 0) break; // block terminator
            int u = nums[0];
            for (size_t k = 1; k < nums.size(); ++k) {
                int v = nums[k];
                adj[u][v] = true;
                adj[v][u] = true;
            }
        }

        for (int i = 1; i <= N; ++i) {
            if (!visited_[i]) dfs(i, -1);
        }

        int count = 0;
        for (int i = 1; i <= N; ++i) if (isArt[i]) ++count;
        cout << count << "\n";
    }
    return 0;
}
