// POJ 3023 - Submarines
// Model: claude-sonnet-5
//
// Each submarine is a line segment (front to rear). Each island is a simple
// polygon (possibly non-convex, per the discuss board). Islands never
// overlap or touch. Classify the segment as fully in water, fully on land,
// or partially on land.
//
// Approach: collect every parameter t in (0,1) where the segment properly
// crosses some island edge (segment-segment intersection via cross
// products). Sort these breakpoints together with t=0 and t=1. Between two
// consecutive breakpoints the segment cannot change land/water status (any
// crossing of any island boundary is itself a breakpoint), so testing the
// midpoint of each sub-interval with a point-in-polygon test against every
// island tells us whether that whole sub-interval is land or water. This
// works for non-convex polygons and is robust to a segment passing exactly
// through a polygon vertex: even if that produces a redundant/duplicate
// breakpoint, the midpoint test on either side still gives the correct
// classification independently of how the crossings were counted.
//
// The statement guarantees no submarine endpoint lies on an island edge and
// no submarine lies along an island edge, so degenerate collinear overlaps
// don't occur; parallel (non-intersecting) edges are simply skipped.

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Pt { double x, y; };

static bool pointInPoly(const Pt &p, const vector<Pt> &poly) {
    int n = (int)poly.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        double xi = poly[i].x, yi = poly[i].y;
        double xj = poly[j].x, yj = poly[j].y;
        if (((yi > p.y) != (yj > p.y)) &&
            (p.x < (xj - xi) * (p.y - yi) / (yj - yi) + xi))
            inside = !inside;
    }
    return inside;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    vector<Pt> subF(T), subR(T);
    for (int i = 0; i < T; i++) {
        double xf, yf, xr, yr;
        scanf("%lf %lf %lf %lf", &xf, &yf, &xr, &yr);
        subF[i].x = xf; subF[i].y = yf;
        subR[i].x = xr; subR[i].y = yr;
    }
    int M;
    scanf("%d", &M);
    vector<vector<Pt> > islands(M);
    for (int i = 0; i < M; i++) {
        int k;
        scanf("%d", &k);
        islands[i].resize(k);
        for (int j = 0; j < k; j++) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            islands[i][j].x = x;
            islands[i][j].y = y;
        }
    }

    for (int i = 0; i < T; i++) {
        Pt A = subF[i], B = subR[i];
        double dx = B.x - A.x, dy = B.y - A.y;

        vector<double> ts;
        ts.push_back(0.0);
        ts.push_back(1.0);

        for (int m = 0; m < M; m++) {
            const vector<Pt> &poly = islands[m];
            int n = (int)poly.size();
            for (int e = 0; e < n; e++) {
                Pt C = poly[e];
                Pt D = poly[(e + 1) % n];
                double ex = D.x - C.x, ey = D.y - C.y;
                double denom = dx * ey - dy * ex;
                if (fabs(denom) < 1e-12) continue; // parallel
                double t = ((C.x - A.x) * ey - (C.y - A.y) * ex) / denom;
                double s = ((C.x - A.x) * dy - (C.y - A.y) * dx) / denom;
                if (s >= -1e-9 && s <= 1.0 + 1e-9 && t > 1e-9 && t < 1.0 - 1e-9) {
                    ts.push_back(t);
                }
            }
        }

        sort(ts.begin(), ts.end());
        bool hasLand = false, hasWater = false;
        for (size_t k = 0; k + 1 < ts.size(); k++) {
            double t0 = ts[k], t1 = ts[k + 1];
            if (t1 - t0 < 1e-9) continue;
            double tm = (t0 + t1) / 2.0;
            Pt mid;
            mid.x = A.x + tm * dx;
            mid.y = A.y + tm * dy;
            bool land = false;
            for (int m = 0; m < M && !land; m++) {
                if (pointInPoly(mid, islands[m])) land = true;
            }
            if (land) hasLand = true; else hasWater = true;
        }

        printf("Submarine %d is ", i + 1);
        if (hasLand && hasWater) printf("partially on land.\n");
        else if (hasLand) printf("completely on land.\n");
        else printf("still in water.\n");
    }
    return 0;
}
