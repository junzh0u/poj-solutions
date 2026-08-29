// POJ 3346 - Treasure of the Chimp Island
// Model: claude-sonnet-5
//
// Multi-source Dijkstra over states (row, col, dynamites_remaining).
// Each boundary cell is either an obstacle (*), a gate without dynamite (#),
// or a gate with a pack of k dynamites (uppercase letter, A=1 .. Z=26).
// Bob enters through exactly one gate (cannot re-enter, cannot ever step on
// any other gate cell), so once inside he is confined to the interior
// (non-boundary) cells only; every non-corner boundary gate has exactly one
// interior neighbor. We model gate entry as a virtual 0-cost edge from the
// gate into that interior neighbor with the gate's dynamite count as the
// starting resource, then run a single Dijkstra over interior cells with
// state (r, c, k) where k = dynamites still available. Stepping onto a
// digit cell costs its face value unless a dynamite is spent (cost 0, k-1);
// since all edge weights are non-negative, an optimal path never revisits a
// physical cell, so this state-space model is exact despite not modeling
// "already destroyed" persistence explicitly.
//
// Ambiguity check: the statement's own worked example (board message
// 169279) explains sample 1's answer of 1 as using the 3-dynamite gate C to
// blast three blocks for free and paying 1 for a remaining '1' block, with
// no further cost -- consistent with letting Dijkstra choose which blocks
// to dynamite rather than assuming a fixed greedy (e.g. "always dynamite
// the hardest") allocation.
//
// "cannot walk on the area of other gates" is enforced by never treating a
// boundary cell as interior after the single entry step, so the path can
// never cross between two otherwise interior-disconnected regions by
// hopping along the boundary through a second gate. A hand-built separating
// case (test_data/3346-other-gates-blocked.in, answer 9) confirms this is
// load-bearing: a mutant that lets the walk pass freely over other gate
// cells still reproduces both sample outputs but answers 0 on that case.

#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

static const int MAXK = 27; // dynamite counts 0..26 (A..Z)
static const int INF = 1000000000;

struct Node {
    int d, r, c, k;
};
struct Cmp {
    bool operator()(const Node& a, const Node& b) const { return a.d > b.d; }
};

static int R, C;
static vector<string> grid;
static vector<vector<vector<int> > > dist_;

static bool isInterior(int r, int c) {
    return r > 0 && r < R - 1 && c > 0 && c < C - 1;
}

static void relax(priority_queue<Node, vector<Node>, Cmp>& pq, int r, int c, int k, int nd) {
    if (nd < dist_[r][c][k]) {
        dist_[r][c][k] = nd;
        Node n;
        n.d = nd; n.r = r; n.c = c; n.k = k;
        pq.push(n);
    }
}

static void processGate(priority_queue<Node, vector<Node>, Cmp>& pq, int r, int c) {
    char ch = grid[r][c];
    int d;
    if (ch == '#') d = 0;
    else if (ch >= 'A' && ch <= 'Z') d = ch - 'A' + 1;
    else return; // asterisk: not a gate

    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    for (int t = 0; t < 4; t++) {
        int nr = r + dr[t], nc = c + dc[t];
        if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
        if (!isInterior(nr, nc)) continue;
        char cell = grid[nr][nc];
        if (cell == '*') continue;
        if (cell == '.' || cell == '$') {
            relax(pq, nr, nc, d, 0);
        } else if (cell >= '1' && cell <= '9') {
            int v = cell - '0';
            relax(pq, nr, nc, d, v);
            if (d > 0) relax(pq, nr, nc, d - 1, 0);
        }
    }
}

static void solve() {
    R = (int)grid.size();
    C = 0;
    for (int i = 0; i < R; i++) if ((int)grid[i].size() > C) C = (int)grid[i].size();
    for (int i = 0; i < R; i++) while ((int)grid[i].size() < C) grid[i] += '*';

    int tr = -1, tc = -1;
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (grid[i][j] == '$') { tr = i; tc = j; }

    dist_.assign(R, vector<vector<int> >(C, vector<int>(MAXK, INF)));

    priority_queue<Node, vector<Node>, Cmp> pq;

    for (int j = 0; j < C; j++) {
        processGate(pq, 0, j);
        processGate(pq, R - 1, j);
    }
    for (int i = 0; i < R; i++) {
        processGate(pq, i, 0);
        processGate(pq, i, C - 1);
    }

    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        if (cur.d > dist_[cur.r][cur.c][cur.k]) continue;
        for (int t = 0; t < 4; t++) {
            int nr = cur.r + dr[t], nc = cur.c + dc[t];
            if (!isInterior(nr, nc)) continue;
            char cell = grid[nr][nc];
            if (cell == '*') continue;
            if (cell == '.' || cell == '$') {
                relax(pq, nr, nc, cur.k, cur.d);
            } else if (cell >= '1' && cell <= '9') {
                int v = cell - '0';
                relax(pq, nr, nc, cur.k, cur.d + v);
                if (cur.k > 0) relax(pq, nr, nc, cur.k - 1, cur.d);
            }
        }
    }

    int ans = INF;
    if (tr >= 0) {
        for (int k = 0; k < MAXK; k++) ans = min(ans, dist_[tr][tc][k]);
    }
    if (ans >= INF) printf("IMPOSSIBLE\n");
    else printf("%d\n", ans);
}

int main() {
    string line;
    while (std::getline(std::cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line == "--") {
            if (!grid.empty()) { solve(); grid.clear(); }
            break;
        }
        if (line.empty()) {
            if (!grid.empty()) { solve(); grid.clear(); }
            continue;
        }
        grid.push_back(line);
    }
    if (!grid.empty()) solve();
    return 0;
}
