// POJ 2227 - The Wedding Juicer
// Model: claude-opus-5
//
// Trapping rain water in 2D.  The board is a W x H grid of unit columns of
// integer height; water poured on top settles at the level of the lowest rim
// that separates a cell from the outside of the board, so the amount held over
// a cell is max(0, level(c) - h(c)) where
//
//     level(c) = min over paths c -> outside of ( max height along the path ).
//
// That minimax value is computed with a Dijkstra-style flood fill: push every
// border cell with key = its own height (border cells leak, so their level is
// their height), then repeatedly pop the globally lowest key.  When the popped
// cell has level L, every unvisited neighbour n is reachable from the outside
// over a rim of height max(L, h(n)) and no cheaper rim can exist, because any
// other route would have to pass through a still-unpopped cell whose key is
// >= L.  So level(n) = max(L, h(n)), the neighbour contributes
// max(0, L - h(n)) cc of juice, and it is pushed with that level.
//
// W,H <= 300 gives 90000 cells, so O(WH log(WH)) is far inside the 2s limit.
// Heights reach 1e9 and 90000 * 1e9 = 9e13 overflows 32 bits: the total is
// accumulated in long long and printed with cout, since POJ's C++ compiler
// predates %lld.
//
// No statement ambiguity: the sample's hint spells out the per-cell fills, and
// the "corner blocks really don't matter" remark is consistent with the
// standard reading (corners are border cells, which never hold water).

#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Node {
    int level;
    int idx;
    Node() : level(0), idx(0) {}
    Node(int l, int i) : level(l), idx(i) {}
    bool operator<(const Node& o) const { return level > o.level; }  // min-heap
};

static int h[300 * 300];
static bool seen[300 * 300];

int main() {
    int W, H;
    if (scanf("%d %d", &W, &H) != 2) return 0;

    int n = W * H;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &h[i]);
        seen[i] = false;
    }

    priority_queue<Node> pq;
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if (r == 0 || r == H - 1 || c == 0 || c == W - 1) {
                int i = r * W + c;
                seen[i] = true;
                pq.push(Node(h[i], i));
            }
        }
    }

    long long total = 0;
    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();
        int r = cur.idx / W, c = cur.idx % W;
        static const int dr[4] = {-1, 1, 0, 0};
        static const int dc[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= H || nc < 0 || nc >= W) continue;
            int ni = nr * W + nc;
            if (seen[ni]) continue;
            seen[ni] = true;
            int lvl = cur.level;
            if (h[ni] > lvl) lvl = h[ni];
            else total += (long long)(lvl - h[ni]);
            pq.push(Node(lvl, ni));
        }
    }

    cout << total << endl;
    return 0;
}
