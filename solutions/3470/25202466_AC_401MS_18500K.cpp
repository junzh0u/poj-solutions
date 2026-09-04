// POJ 3470 - Walls
// Model: claude-sonnet-5
//
// Approach: for each of the 4 axis directions, the bird flying from a point
// can be stopped either "perpendicularly" by a wall running across its path
// (a vertical wall stops horizontal flight, a horizontal wall stops vertical
// flight) or "collinearly" by a wall lying exactly along its flight line,
// where the wall's near endpoint stops it (touching an endpoint counts).
// The discuss board (message 171551 / 357255) makes clear the collinear case
// is required and gives a large worked example used here as test data.
//
// For each direction we run a coordinate-compressed sweep with a segment
// tree that supports range-assign + point-query. Sweeping in the direction
// that visits walls from far to near (relative to future queries) means a
// later assign always represents a strictly closer wall, so plain
// last-write-wins range assignment gives the nearest wall directly, with no
// need to remove/compare distances inside the tree.
//
// RIGHT sweep (x descending): vertical wall w (x=X, y in [ylo,yhi]) does a
// range-assign over y in [ylo,yhi] with value X at time X (perpendicular).
// horizontal wall w (y=Y, x in [xlo,xhi]) does a point-assign at y=Y with
// value xlo at time xlo (collinear: its left endpoint is what a rightward
// flight along y=Y would hit). A query point does a point-query at its own y
// at time = its own x; ties (an assign at exactly the query's x) must lose,
// so queries are processed before assigns at equal sweep coordinate.
// LEFT/UP/DOWN are the mirrored versions.
//
// Coordinates can be huge (the board posts -200000000000), so everything is
// long long and I/O goes through iostream to sidestep any %lld/%I64d
// question entirely.

#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
typedef long long ll;

struct SegTree {
    int n;
    vector<ll> coord;
    vector<int> id;
    vector<char> lazy;
    void init(int n_) {
        n = n_;
        int sz = 4 * n + 4;
        coord.assign(sz, 0);
        id.assign(sz, -1);
        lazy.assign(sz, 0);
    }
    void apply(int node, ll c, int i) {
        coord[node] = c;
        id[node] = i;
        lazy[node] = 1;
    }
    void pushDown(int node) {
        if (lazy[node]) {
            apply(node << 1, coord[node], id[node]);
            apply(node << 1 | 1, coord[node], id[node]);
            lazy[node] = 0;
        }
    }
    void update(int node, int l, int r, int ql, int qr, ll c, int i) {
        if (qr < l || r < ql || ql > qr) return;
        if (ql <= l && r <= qr) { apply(node, c, i); return; }
        pushDown(node);
        int mid = (l + r) >> 1;
        update(node << 1, l, mid, ql, qr, c, i);
        update(node << 1 | 1, mid + 1, r, ql, qr, c, i);
    }
    void query(int node, int l, int r, int pos, ll &oc, int &oi) {
        if (l == r) { oc = coord[node]; oi = id[node]; return; }
        pushDown(node);
        int mid = (l + r) >> 1;
        if (pos <= mid) query(node << 1, l, mid, pos, oc, oi);
        else query(node << 1 | 1, mid + 1, r, pos, oc, oi);
    }
    // convenience wrappers
    void update(int l, int r, ll c, int i) { update(1, 0, n - 1, l, r, c, i); }
    void query(int pos, ll &oc, int &oi) { query(1, 0, n - 1, pos, oc, oi); }
};

struct VWall { ll x, ylo, yhi; int id; };
struct HWall { ll y, xlo, xhi; int id; };
struct Pt { ll x, y; };

struct Ev {
    ll sweep;    // coordinate the sweep is ordered by
    int isQuery; // 0 = wall assign (process first at equal sweep coord... wait see note)
    int lo, hi;  // compressed index range for range-assign, or single index for point ops
    ll val;      // coordinate value to store on assign
    int id;      // wall id for assign
    int pointIdx;
};

// Sort: primary by sweep coordinate in the given direction (desc or asc),
// and at equal coordinate, queries (isQuery=1) must be processed BEFORE
// wall assigns (isQuery=0), so a wall exactly at a query's coordinate is not
// yet visible to that query.
bool cmpDesc(const Ev &a, const Ev &b) {
    if (a.sweep != b.sweep) return a.sweep > b.sweep;
    return a.isQuery > b.isQuery; // query(1) before wall(0)
}
bool cmpAsc(const Ev &a, const Ev &b) {
    if (a.sweep != b.sweep) return a.sweep < b.sweep;
    return a.isQuery > b.isQuery;
}

int N, M;
vector<VWall> vWalls;
vector<HWall> hWalls;
vector<Pt> pts;

ll resDist[4][50005];
int resId[4][50005];

void compress(vector<ll> &v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}
int idx(vector<ll> &v, ll x) {
    return (int)(lower_bound(v.begin(), v.end(), x) - v.begin());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M;
    vWalls.reserve(N);
    hWalls.reserve(N);
    for (int i = 1; i <= N; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 == x2) {
            VWall w;
            w.x = x1;
            w.ylo = min(y1, y2);
            w.yhi = max(y1, y2);
            w.id = i;
            vWalls.push_back(w);
        } else {
            HWall w;
            w.y = y1;
            w.xlo = min(x1, x2);
            w.xhi = max(x1, x2);
            w.id = i;
            hWalls.push_back(w);
        }
    }
    pts.resize(M + 1);
    for (int i = 1; i <= M; i++) {
        cin >> pts[i].x >> pts[i].y;
    }

    // compressed coordinate domains
    vector<ll> allY, allX;
    for (size_t i = 0; i < vWalls.size(); i++) {
        allY.push_back(vWalls[i].ylo);
        allY.push_back(vWalls[i].yhi);
        allX.push_back(vWalls[i].x);
    }
    for (size_t i = 0; i < hWalls.size(); i++) {
        allX.push_back(hWalls[i].xlo);
        allX.push_back(hWalls[i].xhi);
        allY.push_back(hWalls[i].y);
    }
    for (int i = 1; i <= M; i++) {
        allX.push_back(pts[i].x);
        allY.push_back(pts[i].y);
    }
    compress(allY);
    compress(allX);

    for (int i = 1; i <= M; i++)
        for (int d = 0; d < 4; d++) {
            resDist[d][i] = -1; // -1 marks invalid/unset
            resId[d][i] = -1;
        }

    // ---- RIGHT (x descending), segment tree indexed by Y ----
    {
        SegTree seg;
        seg.init((int)allY.size());
        vector<Ev> ev;
        ev.reserve(vWalls.size() + hWalls.size() + M);
        for (size_t i = 0; i < vWalls.size(); i++) {
            Ev e; e.sweep = vWalls[i].x; e.isQuery = 0;
            e.lo = idx(allY, vWalls[i].ylo); e.hi = idx(allY, vWalls[i].yhi);
            e.val = vWalls[i].x; e.id = vWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (size_t i = 0; i < hWalls.size(); i++) {
            Ev e; e.sweep = hWalls[i].xlo; e.isQuery = 0;
            e.lo = e.hi = idx(allY, hWalls[i].y);
            e.val = hWalls[i].xlo; e.id = hWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (int i = 1; i <= M; i++) {
            Ev e; e.sweep = pts[i].x; e.isQuery = 1;
            e.lo = e.hi = idx(allY, pts[i].y);
            e.val = 0; e.id = -1; e.pointIdx = i;
            ev.push_back(e);
        }
        sort(ev.begin(), ev.end(), cmpDesc);
        for (size_t i = 0; i < ev.size(); i++) {
            if (ev[i].isQuery) {
                ll oc; int oi;
                seg.query(ev[i].lo, oc, oi);
                if (oi != -1) {
                    resDist[0][ev[i].pointIdx] = oc - pts[ev[i].pointIdx].x;
                    resId[0][ev[i].pointIdx] = oi;
                }
            } else {
                seg.update(ev[i].lo, ev[i].hi, ev[i].val, ev[i].id);
            }
        }
    }

    // ---- LEFT (x ascending), segment tree indexed by Y ----
    {
        SegTree seg;
        seg.init((int)allY.size());
        vector<Ev> ev;
        ev.reserve(vWalls.size() + hWalls.size() + M);
        for (size_t i = 0; i < vWalls.size(); i++) {
            Ev e; e.sweep = vWalls[i].x; e.isQuery = 0;
            e.lo = idx(allY, vWalls[i].ylo); e.hi = idx(allY, vWalls[i].yhi);
            e.val = vWalls[i].x; e.id = vWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (size_t i = 0; i < hWalls.size(); i++) {
            Ev e; e.sweep = hWalls[i].xhi; e.isQuery = 0;
            e.lo = e.hi = idx(allY, hWalls[i].y);
            e.val = hWalls[i].xhi; e.id = hWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (int i = 1; i <= M; i++) {
            Ev e; e.sweep = pts[i].x; e.isQuery = 1;
            e.lo = e.hi = idx(allY, pts[i].y);
            e.val = 0; e.id = -1; e.pointIdx = i;
            ev.push_back(e);
        }
        sort(ev.begin(), ev.end(), cmpAsc);
        for (size_t i = 0; i < ev.size(); i++) {
            if (ev[i].isQuery) {
                ll oc; int oi;
                seg.query(ev[i].lo, oc, oi);
                if (oi != -1) {
                    resDist[1][ev[i].pointIdx] = pts[ev[i].pointIdx].x - oc;
                    resId[1][ev[i].pointIdx] = oi;
                }
            } else {
                seg.update(ev[i].lo, ev[i].hi, ev[i].val, ev[i].id);
            }
        }
    }

    // ---- UP (y descending), segment tree indexed by X ----
    {
        SegTree seg;
        seg.init((int)allX.size());
        vector<Ev> ev;
        ev.reserve(vWalls.size() + hWalls.size() + M);
        for (size_t i = 0; i < hWalls.size(); i++) {
            Ev e; e.sweep = hWalls[i].y; e.isQuery = 0;
            e.lo = idx(allX, hWalls[i].xlo); e.hi = idx(allX, hWalls[i].xhi);
            e.val = hWalls[i].y; e.id = hWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (size_t i = 0; i < vWalls.size(); i++) {
            Ev e; e.sweep = vWalls[i].ylo; e.isQuery = 0;
            e.lo = e.hi = idx(allX, vWalls[i].x);
            e.val = vWalls[i].ylo; e.id = vWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (int i = 1; i <= M; i++) {
            Ev e; e.sweep = pts[i].y; e.isQuery = 1;
            e.lo = e.hi = idx(allX, pts[i].x);
            e.val = 0; e.id = -1; e.pointIdx = i;
            ev.push_back(e);
        }
        sort(ev.begin(), ev.end(), cmpDesc);
        for (size_t i = 0; i < ev.size(); i++) {
            if (ev[i].isQuery) {
                ll oc; int oi;
                seg.query(ev[i].lo, oc, oi);
                if (oi != -1) {
                    resDist[2][ev[i].pointIdx] = oc - pts[ev[i].pointIdx].y;
                    resId[2][ev[i].pointIdx] = oi;
                }
            } else {
                seg.update(ev[i].lo, ev[i].hi, ev[i].val, ev[i].id);
            }
        }
    }

    // ---- DOWN (y ascending), segment tree indexed by X ----
    {
        SegTree seg;
        seg.init((int)allX.size());
        vector<Ev> ev;
        ev.reserve(vWalls.size() + hWalls.size() + M);
        for (size_t i = 0; i < hWalls.size(); i++) {
            Ev e; e.sweep = hWalls[i].y; e.isQuery = 0;
            e.lo = idx(allX, hWalls[i].xlo); e.hi = idx(allX, hWalls[i].xhi);
            e.val = hWalls[i].y; e.id = hWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (size_t i = 0; i < vWalls.size(); i++) {
            Ev e; e.sweep = vWalls[i].yhi; e.isQuery = 0;
            e.lo = e.hi = idx(allX, vWalls[i].x);
            e.val = vWalls[i].yhi; e.id = vWalls[i].id; e.pointIdx = 0;
            ev.push_back(e);
        }
        for (int i = 1; i <= M; i++) {
            Ev e; e.sweep = pts[i].y; e.isQuery = 1;
            e.lo = e.hi = idx(allX, pts[i].x);
            e.val = 0; e.id = -1; e.pointIdx = i;
            ev.push_back(e);
        }
        sort(ev.begin(), ev.end(), cmpAsc);
        for (size_t i = 0; i < ev.size(); i++) {
            if (ev[i].isQuery) {
                ll oc; int oi;
                seg.query(ev[i].lo, oc, oi);
                if (oi != -1) {
                    resDist[3][ev[i].pointIdx] = pts[ev[i].pointIdx].y - oc;
                    resId[3][ev[i].pointIdx] = oi;
                }
            } else {
                seg.update(ev[i].lo, ev[i].hi, ev[i].val, ev[i].id);
            }
        }
    }

    vector<int> ans(N + 1, 0);
    for (int i = 1; i <= M; i++) {
        ll best = -1;
        int bestId = -1;
        for (int d = 0; d < 4; d++) {
            if (resId[d][i] == -1) continue;
            if (best == -1 || resDist[d][i] < best) {
                best = resDist[d][i];
                bestId = resId[d][i];
            }
        }
        // guaranteed unique minimum and that at least one direction is valid
        if (bestId != -1) ans[bestId]++;
    }

    for (int i = 1; i <= N; i++) {
        cout << ans[i] << "\n";
    }
    return 0;
}
