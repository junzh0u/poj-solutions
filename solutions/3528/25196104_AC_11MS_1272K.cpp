// POJ 3528 - Ultimate Weapon
// Model: claude-fable-5
// Approach: 3D convex hull by incremental insertion (O(n^2) with directed-edge
// horizon marking), then sum of triangle face areas, printed to 3 decimals.
// The hint guarantees no four coplanar battleships, so the hull is simplicial
// and a strict visibility test (signed volume > eps) suffices. Coordinates are
// integers up to 10000 (setter confirmed range on the board), so all cross and
// triple products are exact in doubles. Degenerate inputs (n < 4, all points
// collinear or coplanar) are handled separately: 0 for dimension < 2, twice
// the planar 2D-hull area for a flat point set.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

struct P {
    double x, y, z;
};

static P vsub(const P &a, const P &b) {
    P r; r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; return r;
}
static P vcross(const P &a, const P &b) {
    P r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
static double vdot(const P &a, const P &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static double vlen(const P &a) {
    return sqrt(vdot(a, a));
}

static P pts[505];
// signed volume*6 of tetrahedron (a,b,c,d): >0 when d is on the outward side
// of oriented triangle (a,b,c)
static double vol(int a, int b, int c, int d) {
    return vdot(vcross(vsub(pts[b], pts[a]), vsub(pts[c], pts[a])),
                vsub(pts[d], pts[a]));
}

struct Face {
    int a, b, c;
    Face() {}
    Face(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
};

static int markEdge[505][505];
static const double EPS = 1e-8;

static double triArea2(int a, int b, int c) { // twice the triangle area
    return vlen(vcross(vsub(pts[b], pts[a]), vsub(pts[c], pts[a])));
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++)
        scanf("%lf%lf%lf", &pts[i].x, &pts[i].y, &pts[i].z);

    // find an initial non-degenerate tetrahedron in positions 0..3
    int dim = 0; // highest structure found so far
    // 1: a point distinct from pts[0]
    for (int i = 1; i < n; i++)
        if (vlen(vsub(pts[i], pts[0])) > EPS) { swap(pts[1], pts[i]); dim = 1; break; }
    // 2: a point not collinear with pts[0], pts[1]
    if (dim == 1)
        for (int i = 2; i < n; i++)
            if (vlen(vcross(vsub(pts[1], pts[0]), vsub(pts[i], pts[0]))) > EPS) {
                swap(pts[2], pts[i]); dim = 2; break;
            }
    // 3: a point not coplanar with pts[0..2]
    if (dim == 2)
        for (int i = 3; i < n; i++)
            if (fabs(vol(0, 1, 2, i)) > EPS) { swap(pts[3], pts[i]); dim = 3; break; }

    if (dim <= 1) { // single point or a segment: zero surface
        printf("0.000\n");
        return 0;
    }
    if (dim == 2) { // all points coplanar: closed region degenerates to the
                    // flat 2D hull; surface counts both sides
        P nrm = vcross(vsub(pts[1], pts[0]), vsub(pts[2], pts[0]));
        double nl = vlen(nrm);
        nrm.x /= nl; nrm.y /= nl; nrm.z /= nl;
        P e1 = vsub(pts[1], pts[0]);
        double e1l = vlen(e1);
        e1.x /= e1l; e1.y /= e1l; e1.z /= e1l;
        P e2 = vcross(nrm, e1);
        vector< pair<double, double> > q(n);
        for (int i = 0; i < n; i++) {
            P d = vsub(pts[i], pts[0]);
            q[i] = make_pair(vdot(d, e1), vdot(d, e2));
        }
        sort(q.begin(), q.end());
        // Andrew monotone chain
        vector< pair<double, double> > h(2 * n);
        int k = 0;
        for (int i = 0; i < n; i++) {
            while (k >= 2 && (h[k-1].first - h[k-2].first) * (q[i].second - h[k-2].second)
                           - (h[k-1].second - h[k-2].second) * (q[i].first - h[k-2].first) <= 0) k--;
            h[k++] = q[i];
        }
        int lower = k + 1;
        for (int i = n - 2; i >= 0; i--) {
            while (k >= lower && (h[k-1].first - h[k-2].first) * (q[i].second - h[k-2].second)
                               - (h[k-1].second - h[k-2].second) * (q[i].first - h[k-2].first) <= 0) k--;
            h[k++] = q[i];
        }
        double area2 = 0.0; // twice the polygon area
        for (int i = 0; i < k - 1; i++)
            area2 += h[i].first * h[i+1].second - h[i+1].first * h[i].second;
        printf("%.3f\n", fabs(area2)); // 2 * hull area = both sides
        return 0;
    }

    // build the initial tetrahedron, each face oriented outward
    vector<Face> faces;
    for (int a = 0; a < 4; a++)
        for (int b = a + 1; b < 4; b++)
            for (int c = b + 1; c < 4; c++) {
                int d = 0 + 1 + 2 + 3 - a - b - c;
                if (vol(a, b, c, d) > 0) faces.push_back(Face(a, c, b));
                else faces.push_back(Face(a, b, c));
            }

    memset(markEdge, 0, sizeof(markEdge));
    int stamp = 0;
    vector<char> vis;
    for (int i = 4; i < n; i++) {
        int m = (int)faces.size();
        vis.assign(m, 0);
        bool any = false;
        for (int f = 0; f < m; f++)
            if (vol(faces[f].a, faces[f].b, faces[f].c, i) > EPS) {
                vis[f] = 1; any = true;
            }
        if (!any) continue; // point inside (or on) the hull
        stamp++;
        for (int f = 0; f < m; f++)
            if (vis[f]) {
                markEdge[faces[f].a][faces[f].b] = stamp;
                markEdge[faces[f].b][faces[f].c] = stamp;
                markEdge[faces[f].c][faces[f].a] = stamp;
            }
        vector<Face> nf;
        nf.reserve(m + 8);
        for (int f = 0; f < m; f++) {
            if (!vis[f]) { nf.push_back(faces[f]); continue; }
            int e[3][2];
            e[0][0] = faces[f].a; e[0][1] = faces[f].b;
            e[1][0] = faces[f].b; e[1][1] = faces[f].c;
            e[2][0] = faces[f].c; e[2][1] = faces[f].a;
            for (int t = 0; t < 3; t++) {
                int u = e[t][0], v = e[t][1];
                if (markEdge[v][u] != stamp) // horizon edge
                    nf.push_back(Face(u, v, i));
            }
        }
        faces.swap(nf);
    }

    double area = 0.0;
    for (size_t f = 0; f < faces.size(); f++)
        area += triArea2(faces[f].a, faces[f].b, faces[f].c);
    printf("%.3f\n", area / 2.0);
    return 0;
}
