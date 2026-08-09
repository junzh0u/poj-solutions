// POJ 3501 - Escape from Enemy Territory
// Model: claude-sonnet-5
//
// For each cell, dist(cell) = Manhattan distance to nearest enemy base,
// computed by multi-source BFS (the grid has no obstacles, so BFS hop
// count from the set of base cells equals exact Manhattan distance to
// the nearest base). We want a route from start to rendezvous that
// maximizes the minimum dist(cell) over the cells visited, and among
// all routes achieving that maximum, the shortest one (fewest steps).
//
// The maximum achievable bottleneck value V is found by activating
// cells in decreasing order of dist and union-find-merging adjacent
// already-active cells (Kruskal-style maximum bottleneck path on a
// grid graph): V is the dist value at the moment start and rendezvous
// first land in the same union-find component. Because connectivity of
// {cells with dist>=t} only grows as t decreases, this first-connection
// threshold is exactly the maximum t for which start and rendezvous are
// connected within {dist>=t}. A final BFS restricted to cells with
// dist>=V then gives the shortest route length under that constraint.
//
// Board (showmessage 135900/154976): the route may pass directly
// through an enemy base cell (dist 0 is a legal value); start and
// rendezvous are guaranteed distinct. Board also repeatedly flags STL
// queue/containers as too slow here (137808, 139667, 174812) so this
// uses hand-rolled arrays throughout (BFS queues, union-find, counting
// sort) instead of std::queue/vector.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXC = 1000000;
const int MAXD = 2005;

static int dist_[MAXC];
static int order_[MAXC];
static int parent_[MAXC];
static int sz_[MAXC];
static bool active_[MAXC];
static int distFromStart[MAXC];
static int q_[MAXC];
static int cnt_[MAXD];
static int offset_[MAXD];

static int find(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

static void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (sz_[a] < sz_[b]) swap(a, b);
    parent_[b] = a;
    sz_[a] += sz_[b];
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N, X, Y;
        scanf("%d %d %d", &N, &X, &Y);
        int xi, yi, xr, yr;
        scanf("%d %d %d %d", &xi, &yi, &xr, &yr);
        int size = X * Y;
        int startIdx = xi * Y + yi;
        int endIdx = xr * Y + yr;

        memset(dist_, -1, sizeof(int) * (size_t)size);

        int qh = 0, qt = 0;
        for (int i = 0; i < N; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            int idx = x * Y + y;
            if (dist_[idx] == -1) {
                dist_[idx] = 0;
                q_[qt++] = idx;
            }
        }
        // multi-source BFS to get Manhattan distance to nearest base
        while (qh < qt) {
            int cur = q_[qh++];
            int cx = cur / Y, cy = cur % Y;
            int nd = dist_[cur] + 1;
            if (cx > 0) { int nb = cur - Y; if (dist_[nb] == -1) { dist_[nb] = nd; q_[qt++] = nb; } }
            if (cx < X - 1) { int nb = cur + Y; if (dist_[nb] == -1) { dist_[nb] = nd; q_[qt++] = nb; } }
            if (cy > 0) { int nb = cur - 1; if (dist_[nb] == -1) { dist_[nb] = nd; q_[qt++] = nb; } }
            if (cy < Y - 1) { int nb = cur + 1; if (dist_[nb] == -1) { dist_[nb] = nd; q_[qt++] = nb; } }
        }
        int maxD = dist_[q_[size - 1]];

        // counting sort cells by dist, descending
        for (int d = 0; d <= maxD; d++) cnt_[d] = 0;
        for (int i = 0; i < size; i++) cnt_[dist_[i]]++;
        offset_[maxD] = 0;
        for (int d = maxD - 1; d >= 0; d--) offset_[d] = offset_[d + 1] + cnt_[d + 1];
        for (int i = 0; i < size; i++) {
            int d = dist_[i];
            order_[offset_[d]++] = i;
        }

        for (int i = 0; i < size; i++) {
            parent_[i] = i;
            sz_[i] = 1;
            active_[i] = false;
        }

        int V = 0;
        for (int i = 0; i < size; i++) {
            int c = order_[i];
            active_[c] = true;
            int cx = c / Y, cy = c % Y;
            if (cx > 0 && active_[c - Y]) unite(c, c - Y);
            if (cx < X - 1 && active_[c + Y]) unite(c, c + Y);
            if (cy > 0 && active_[c - 1]) unite(c, c - 1);
            if (cy < Y - 1 && active_[c + 1]) unite(c, c + 1);
            if (find(startIdx) == find(endIdx)) {
                V = dist_[c];
                break;
            }
        }

        // shortest route within {cell : dist(cell) >= V}
        memset(distFromStart, -1, sizeof(int) * (size_t)size);
        qh = 0; qt = 0;
        distFromStart[startIdx] = 0;
        q_[qt++] = startIdx;
        while (qh < qt) {
            int cur = q_[qh++];
            if (cur == endIdx) break;
            int cx = cur / Y, cy = cur % Y;
            int nd = distFromStart[cur] + 1;
            if (cx > 0) { int nb = cur - Y; if (dist_[nb] >= V && distFromStart[nb] == -1) { distFromStart[nb] = nd; q_[qt++] = nb; } }
            if (cx < X - 1) { int nb = cur + Y; if (dist_[nb] >= V && distFromStart[nb] == -1) { distFromStart[nb] = nd; q_[qt++] = nb; } }
            if (cy > 0) { int nb = cur - 1; if (dist_[nb] >= V && distFromStart[nb] == -1) { distFromStart[nb] = nd; q_[qt++] = nb; } }
            if (cy < Y - 1) { int nb = cur + 1; if (dist_[nb] >= V && distFromStart[nb] == -1) { distFromStart[nb] = nd; q_[qt++] = nb; } }
        }

        printf("%d %d\n", V, distFromStart[endIdx]);
    }
    return 0;
}
