// POJ 3714 - Raid
//
// Closest bichromatic pair (one point from the station set, one from the
// agent set) among up to 1e5 points per set, coordinates up to 1e9.
//
// Approach: build a balanced 2D k-d tree over the stations (splitting on
// x/y alternately using nth_element on the median index, which keeps the
// tree depth O(log n) regardless of coordinate values -- important since
// the sample has 8 coincident points). Query every agent against it with
// the standard k-d tree nearest-neighbour search, but prune using a single
// GLOBAL running best-squared-distance shared across all queries (not a
// fresh bound per query): once the current global best is small, most of
// the tree gets pruned away for every subsequent query, since a branch
// whose closest possible point can't beat the already-known global best
// can never contribute the answer. All distance comparisons are kept as
// exact integer squared distances (fits in a 64-bit long long, since
// coordinates are bounded by 1e9); the single sqrt happens only once per
// test case, on the final answer, before formatting to 3 decimals.
//
// This deliberately avoids the classic divide-and-conquer closest-pair
// merge step: naively adapting it to "only mixed-colour pairs count" can
// degenerate to O(n^2) when a recursive half is monochromatic (e.g. all
// stations end up left of all agents after sorting by x) since the usual
// strip-pruning distance is undefined in that case. The k-d tree side-
// steps this because it doesn't need color-aware recursion at all: any
// tree, regardless of the color split across it, still has O(log n) depth
// and correctness doesn't depend on both branches containing both colors.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long long ll;

struct Point {
    ll x, y;
};

const int MAXN = 100005;
Point pts[MAXN];       // stations, used as scratch during kd-tree build
Point agents[MAXN];

struct Node {
    ll x, y;
    int l, r;
};
Node tree[MAXN];
int treeCnt;

bool cmpX(const Point &a, const Point &b) { return a.x < b.x; }
bool cmpY(const Point &a, const Point &b) { return a.y < b.y; }

int build(int l, int r, int depth) {
    if (l >= r) return -1;
    int mid = l + (r - l) / 2;
    if (depth % 2 == 0)
        nth_element(pts + l, pts + mid, pts + r, cmpX);
    else
        nth_element(pts + l, pts + mid, pts + r, cmpY);
    int cur = treeCnt++;
    tree[cur].x = pts[mid].x;
    tree[cur].y = pts[mid].y;
    tree[cur].l = build(l, mid, depth + 1);
    tree[cur].r = build(mid + 1, r, depth + 1);
    return cur;
}

ll bestSq;
ll qx, qy;

void query(int node, int depth) {
    if (node < 0) return;
    ll dx = tree[node].x - qx;
    ll dy = tree[node].y - qy;
    ll d = dx * dx + dy * dy;
    if (d < bestSq) bestSq = d;

    ll diff = (depth % 2 == 0) ? (qx - tree[node].x) : (qy - tree[node].y);
    int near = (diff < 0) ? tree[node].l : tree[node].r;
    int far = (diff < 0) ? tree[node].r : tree[node].l;

    query(near, depth + 1);
    if (diff * diff < bestSq) {
        query(far, depth + 1);
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            long long x, y;
            scanf("%lld %lld", &x, &y);
            pts[i].x = x;
            pts[i].y = y;
        }
        for (int i = 0; i < n; ++i) {
            long long x, y;
            scanf("%lld %lld", &x, &y);
            agents[i].x = x;
            agents[i].y = y;
        }
        treeCnt = 0;
        int root = build(0, n, 0);
        const ll INF = (ll)4e18;
        bestSq = INF;
        for (int i = 0; i < n; ++i) {
            qx = agents[i].x;
            qy = agents[i].y;
            query(root, 0);
        }
        double dist = sqrt((double)bestSq);
        printf("%.3f\n", dist);
    }
    return 0;
}
