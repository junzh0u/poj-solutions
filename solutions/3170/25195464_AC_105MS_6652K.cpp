// POJ 3170 - Knights of Ni
// Model: claude-sonnet-5
// Approach: two flood-fill BFS on the WxH grid (W,H <= 1000, read as a flat
// stream of ints regardless of line wrapping). BFS1 runs from Bessie's start
// square with the Knights' square (value 3) treated as impassable, since she
// may not cross it without a shrubbery. BFS2 runs from the Knights' square
// with no such restriction (she already has the shrubbery on the way back,
// and the board confirms passing through a second shrubbery square while
// still carrying the first is fine, so nothing besides value-1 cells blocks
// either pass). Answer is min over shrubbery cells (value 4) of
// dist1[cell] + dist2[cell]; the problem guarantees a route exists.
// Ambiguity: none in the statement itself; the discuss board mainly warns
// that this is exactly a "two BFS from points 2 and 3" problem and that only
// value-1 cells block movement (checked, matches the reading above), plus a
// cin-vs-scanf TLE warning (addressed by using scanf).
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static const int MAXN = 1000 * 1000 + 5;
static signed char grid[MAXN];
static int dist1[MAXN];
static int dist2[MAXN];
static int qbuf[MAXN];

int W, H;

inline int idx(int r, int c) { return r * W + c; }

void bfs(int src, int* dist, bool blockKnight, int knightIdx) {
    for (int i = 0; i < W * H; ++i) dist[i] = -1;
    int head = 0, tail = 0;
    dist[src] = 0;
    qbuf[tail++] = src;
    while (head < tail) {
        int cur = qbuf[head++];
        int r = cur / W, c = cur % W;
        int nr[4] = {r - 1, r + 1, r, r};
        int nc[4] = {c, c, c - 1, c + 1};
        for (int k = 0; k < 4; ++k) {
            int rr = nr[k], cc = nc[k];
            if (rr < 0 || rr >= H || cc < 0 || cc >= W) continue;
            int ni = idx(rr, cc);
            if (grid[ni] == 1) continue;
            if (blockKnight && ni == knightIdx) continue;
            if (dist[ni] != -1) continue;
            dist[ni] = dist[cur] + 1;
            qbuf[tail++] = ni;
        }
    }
}

int main() {
    if (scanf("%d %d", &W, &H) != 2) return 0;
    int total = W * H;
    int startIdx = -1, knightIdx = -1;
    vector<int> shrubs;
    for (int i = 0; i < total; ++i) {
        int v;
        scanf("%d", &v);
        grid[i] = (signed char)v;
        if (v == 2) startIdx = i;
        else if (v == 3) knightIdx = i;
        else if (v == 4) shrubs.push_back(i);
    }

    bfs(startIdx, dist1, true, knightIdx);
    bfs(knightIdx, dist2, false, -1);

    int best = -1;
    for (size_t i = 0; i < shrubs.size(); ++i) {
        int s = shrubs[i];
        if (dist1[s] == -1 || dist2[s] == -1) continue;
        int total_d = dist1[s] + dist2[s];
        if (best == -1 || total_d < best) best = total_d;
    }

    printf("%d\n", best);
    return 0;
}
