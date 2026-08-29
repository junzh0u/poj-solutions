// POJ 3747 - Scout YYF II
// Model: claude-sonnet-5
// Approach: Bomb i and YYF's start S expand/move at equal speed, so the set of
// boundary points YYF can safely reach by running in a straight line is exactly
// the intersection, over all bombs B_i, of the half-plane "closer to S than to
// B_i" (points P with (B_i-S)*P < (|B_i|^2-|S|^2)/2, the side of the
// perpendicular bisector of S,B_i containing S). This region is convex and
// contains S, so any straight path from S that stays inside it is safe the
// whole way (distance-to-bomb always exceeds elapsed time). Since the region is
// convex, the farthest point from the origin over the (possibly unbounded,
// here clipped to a box far larger than R) region is attained at a vertex, and
// by the intermediate value theorem the region reaches the base boundary
// (radius R) iff some vertex has distance from the origin >= R. Implemented via
// iterative half-plane clipping (Sutherland-Hodgman) of a large bounding box.
// Ambiguity: none found in the statement; the discuss board confirms a
// straight-line path is optimal (a curved path cannot help versus the best
// straight line, proved there via a cone/space-time argument) and matches this
// perpendicular-bisector characterization.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

struct Pt { double x, y; };

Pt makePt(double x, double y){ Pt r; r.x = x; r.y = y; return r; }
Pt operator-(Pt a, Pt b){ return makePt(a.x-b.x, a.y-b.y); }
Pt operator+(Pt a, Pt b){ return makePt(a.x+b.x, a.y+b.y); }
Pt operator*(Pt a, double t){ return makePt(a.x*t, a.y*t); }

static const double EPS = 1e-9;

// Clip convex polygon poly, keeping the side nx*x+ny*y <= c.
vector<Pt> clipHalfPlane(const vector<Pt>& poly, double nx, double ny, double c) {
    vector<Pt> res;
    int m = (int)poly.size();
    if (m == 0) return res;
    for (int i = 0; i < m; i++) {
        Pt cur = poly[i];
        Pt nxt = poly[(i+1)%m];
        double dcur = nx*cur.x + ny*cur.y - c;
        double dnxt = nx*nxt.x + ny*nxt.y - c;
        bool curIn = dcur <= EPS;
        bool nxtIn = dnxt <= EPS;
        if (curIn) res.push_back(cur);
        if (curIn != nxtIn) {
            double t = dcur / (dcur - dnxt);
            res.push_back(cur + (nxt - cur) * t);
        }
    }
    return res;
}

int main(){
    int n;
    double R;
    while (scanf("%d %lf", &n, &R) == 2) {
        Pt S;
        scanf("%lf %lf", &S.x, &S.y);
        vector<Pt> bombs(n);
        for (int i = 0; i < n; i++) scanf("%lf %lf", &bombs[i].x, &bombs[i].y);

        double L = 1e5; // bounding box half-extent, far larger than R<=1000
        vector<Pt> poly;
        poly.push_back(makePt(-L,-L));
        poly.push_back(makePt( L,-L));
        poly.push_back(makePt( L, L));
        poly.push_back(makePt(-L, L));

        for (int i = 0; i < n && !poly.empty(); i++) {
            Pt B = bombs[i];
            double nx = B.x - S.x;
            double ny = B.y - S.y;
            // A bomb exactly at YYF's own position gives a degenerate
            // (zero-normal) half-plane: no point is ever strictly safe from it,
            // so escape is impossible regardless of the other bombs.
            if (fabs(nx) < EPS && fabs(ny) < EPS) { poly.clear(); break; }
            double c = (B.x*B.x + B.y*B.y - S.x*S.x - S.y*S.y) / 2.0;
            poly = clipHalfPlane(poly, nx, ny, c);
        }

        double maxd2 = -1;
        for (size_t i = 0; i < poly.size(); i++) {
            double d2 = poly[i].x*poly[i].x + poly[i].y*poly[i].y;
            if (d2 > maxd2) maxd2 = d2;
        }

        bool escape = (maxd2 >= R*R - 1e-6);
        printf(escape ? "Yes\n" : "No\n");
    }
    return 0;
}
