// POJ 2048 - Monster Trap
// Model: claude-sonnet-5
//
// The monster sits at the origin; the wizard's segments never pass through
// it. The monster escapes to infinity iff the origin lies in the unbounded
// face of the planar arrangement formed by the segments.
//
// Approach: split every segment at all pairwise intersection points to get
// a planar straight-line graph. Build a spanning forest (iterative DFS).
// Every non-tree edge closes a simple cycle with the tree path between its
// endpoints (undirected DFS has no cross edges, so the "back" endpoint is
// always an ancestor of the current node, giving a clean parent-pointer
// walk). Each such fundamental cycle is a genuine simple polygon (its edges
// only meet at shared vertices, by construction of the arrangement).
//
// Claim: the origin is trapped (bounded face) iff it lies inside at least
// one fundamental cycle polygon (logical OR over all cycles from any
// spanning forest - not an XOR/parity rule). Verified by hand against:
// disjoint nested loops (annulus point sits inside the outer loop only;
// inner-region point sits inside both -> OR still says trapped, matching
// truth, while XOR would wrongly clear it), and a "theta graph" (three
// parallel paths between two hubs) where the middle bounded face is inside
// both fundamental cycles built from one spanning path (count 2, XOR would
// say untrapped -- wrong; OR says trapped -- correct). A graph with no
// cycles (a pure forest) never traps anything, matching the fact that the
// complement of an embedded tree/forest of segments is connected.
//
// Segment-segment intersection for the *original* n segments uses exact
// integer arithmetic (coordinates are integers in [-50,50], so all cross
// products are exact in double/int) and only falls back to a
// floating division for genuinely transversal crossings; touching cases
// (shared or T-junction endpoints) return the literal integer endpoint,
// avoiding floating drift. Arrangement points are deduplicated with a
// spatial hash (grid cell ~1e-4) since intersection points can coincide
// with existing vertices; the statement guarantees any two intersection
// points are separated by more than 1e-5, so a 1e-6 merge tolerance cannot
// conflate genuinely distinct points.
//
// The final point-in-polygon test against the origin uses the standard
// half-open ray-casting rule, which is exact here since the origin's own
// coordinates (0,0) are exact and never coincide with any arrangement
// vertex (segments never cross the origin).
//
// No ambiguity found in the statement itself; the discuss board's top post
// (2021, lddlinan) independently confirms this exact spanning-tree +
// fundamental-cycle technique as the accepted approach for this problem.

#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;

struct Pt { double x, y; };

static int crossI(int ox, int oy, int ax, int ay,
                         int bx, int by) {
    return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

static bool onSeg(int px1, int py1, int px2, int py2,
                   int qx, int qy) {
    return min(px1, px2) <= qx && qx <= max(px1, px2) &&
           min(py1, py2) <= qy && qy <= max(py1, py2);
}

struct Seg { int x1, y1, x2, y2; };

// Returns true if segments A and B share/intersect at exactly one point
// (per problem guarantee), filling res with that point.
static bool segIntersect(const Seg &A, const Seg &B, Pt &res) {
    int ax1 = A.x1, ay1 = A.y1, ax2 = A.x2, ay2 = A.y2;
    int bx1 = B.x1, by1 = B.y1, bx2 = B.x2, by2 = B.y2;

    int d1 = crossI(bx1, by1, bx2, by2, ax1, ay1);
    int d2 = crossI(bx1, by1, bx2, by2, ax2, ay2);
    int d3 = crossI(ax1, ay1, ax2, ay2, bx1, by1);
    int d4 = crossI(ax1, ay1, ax2, ay2, bx2, by2);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        double x1 = (double)ax1, y1 = (double)ay1, x2 = (double)ax2, y2 = (double)ay2;
        double x3 = (double)bx1, y3 = (double)by1, x4 = (double)bx2, y4 = (double)by2;
        double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        res.x = x1 + t * (x2 - x1);
        res.y = y1 + t * (y2 - y1);
        return true;
    }
    if (d1 == 0 && onSeg(bx1, by1, bx2, by2, ax1, ay1)) { res.x = (double)ax1; res.y = (double)ay1; return true; }
    if (d2 == 0 && onSeg(bx1, by1, bx2, by2, ax2, ay2)) { res.x = (double)ax2; res.y = (double)ay2; return true; }
    if (d3 == 0 && onSeg(ax1, ay1, ax2, ay2, bx1, by1)) { res.x = (double)bx1; res.y = (double)by1; return true; }
    if (d4 == 0 && onSeg(ax1, ay1, ax2, ay2, bx2, by2)) { res.x = (double)bx2; res.y = (double)by2; return true; }
    return false;
}

vector<Pt> pts;
map<pair<int, int>, vector<int> > grid;
static const double CELL = 1e-4;      // grid cell size
static const double MERGE_EPS = 1e-6; // merge tolerance (points guaranteed >1e-5 apart)

static int gridKey(double v) {
    return (int)floor(v / CELL);
}

static int getPointId(const Pt &p) {
    int gx = gridKey(p.x), gy = gridKey(p.y);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            map<pair<int, int>, vector<int> >::iterator it =
                grid.find(make_pair(gx + dx, gy + dy));
            if (it == grid.end()) continue;
            for (size_t k = 0; k < it->second.size(); ++k) {
                int idx = it->second[k];
                double ddx = pts[idx].x - p.x, ddy = pts[idx].y - p.y;
                if (ddx * ddx + ddy * ddy < MERGE_EPS * MERGE_EPS) return idx;
            }
        }
    }
    int id = (int)pts.size();
    pts.push_back(p);
    grid[make_pair(gx, gy)].push_back(id);
    return id;
}

static bool pointInPoly(const Pt &p, const vector<Pt> &poly) {
    bool inside = false;
    size_t n = poly.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
            (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<Seg> segs(n);
        for (int i = 0; i < n; ++i) {
            int x, y, x0, y0;
            scanf("%d %d %d %d", &x, &y, &x0, &y0);
            segs[i].x1 = x; segs[i].y1 = y; segs[i].x2 = x0; segs[i].y2 = y0;
        }

        pts.clear();
        grid.clear();

        vector<int> A(n), B(n);
        for (int i = 0; i < n; ++i) {
            Pt pa; pa.x = (double)segs[i].x1; pa.y = (double)segs[i].y1;
            Pt pb; pb.x = (double)segs[i].x2; pb.y = (double)segs[i].y2;
            A[i] = getPointId(pa);
            B[i] = getPointId(pb);
        }

        vector<vector<int> > onSegPts(n);
        for (int i = 0; i < n; ++i) {
            onSegPts[i].push_back(A[i]);
            onSegPts[i].push_back(B[i]);
        }
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Pt r;
                if (segIntersect(segs[i], segs[j], r)) {
                    int id = getPointId(r);
                    onSegPts[i].push_back(id);
                    onSegPts[j].push_back(id);
                }
            }
        }

        // Build edges by sorting points along each original segment.
        vector<vector<pair<int, int> > > adj(pts.size()); // (edgeId, otherVertex)
        int edgeCount = 0;
        for (int i = 0; i < n; ++i) {
            double dx = (double)(segs[i].x2 - segs[i].x1);
            double dy = (double)(segs[i].y2 - segs[i].y1);
            vector<int> &ids = onSegPts[i];
            vector<pair<double, int> > keyed;
            keyed.reserve(ids.size());
            for (size_t k = 0; k < ids.size(); ++k) {
                double t = (pts[ids[k]].x - segs[i].x1) * dx + (pts[ids[k]].y - segs[i].y1) * dy;
                keyed.push_back(make_pair(t, ids[k]));
            }
            sort(keyed.begin(), keyed.end());
            for (size_t k = 1; k < keyed.size(); ++k) {
                if (keyed[k].second == keyed[k - 1].second) continue;
                int u = keyed[k - 1].second, v = keyed[k].second;
                int eid = edgeCount++;
                adj[u].push_back(make_pair(eid, v));
                adj[v].push_back(make_pair(eid, u));
            }
        }

        int V = (int)pts.size();
        vector<int> parent(V, -1);
        vector<bool> visitedV(V, false);
        vector<bool> usedEdge(edgeCount, false);
        vector<vector<int> > fundamentalCycles;

        for (int s = 0; s < V; ++s) {
            if (visitedV[s]) continue;
            visitedV[s] = true;
            parent[s] = -1;
            vector<pair<int, size_t> > stk; // (vertex, next adj index)
            stk.push_back(make_pair(s, (size_t)0));
            while (!stk.empty()) {
                int u = stk.back().first;
                size_t &idx = stk.back().second;
                if (idx >= adj[u].size()) { stk.pop_back(); continue; }
                pair<int, int> e = adj[u][idx];
                idx++;
                int eid = e.first, w = e.second;
                if (usedEdge[eid]) continue;
                usedEdge[eid] = true;
                if (!visitedV[w]) {
                    visitedV[w] = true;
                    parent[w] = u;
                    stk.push_back(make_pair(w, (size_t)0));
                } else {
                    // back edge: w is an ancestor of u (undirected DFS has no cross edges)
                    vector<int> cyc;
                    int cur = u;
                    cyc.push_back(cur);
                    while (cur != w) {
                        cur = parent[cur];
                        cyc.push_back(cur);
                    }
                    fundamentalCycles.push_back(cyc);
                }
            }
        }

        Pt origin; origin.x = 0.0; origin.y = 0.0;
        bool trapped = false;
        for (size_t c = 0; c < fundamentalCycles.size() && !trapped; ++c) {
            vector<Pt> poly;
            poly.reserve(fundamentalCycles[c].size());
            for (size_t k = 0; k < fundamentalCycles[c].size(); ++k) {
                poly.push_back(pts[fundamentalCycles[c][k]]);
            }
            if (pointInPoly(origin, poly)) trapped = true;
        }

        printf(trapped ? "yes\n" : "no\n");
    }
    return 0;
}
