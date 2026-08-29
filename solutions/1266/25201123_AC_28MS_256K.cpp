// POJ 1266 - Cover an Arc.
// Model: claude-sonnet-5
//
// Approach: reconstruct the circle through the two arc endpoints A,B and the
// given inner point C. The arc we must cover is the one running from A to B
// through C (not the complementary arc). For an axis-aligned bounding box of
// that arc we only need to know, for each of the four cardinal directions
// (angle 0, 90, 180, 270 degrees = rightmost/topmost/leftmost/bottommost
// point of the *full* circle), whether that cardinal point lies on our arc.
// If it does, that coordinate's extreme is cx+-r (or cy+-r); otherwise the
// extreme along that axis is attained at one of the two arc endpoints A,B
// (the coordinate function is monotonic between consecutive cardinal
// angles). Finally the carpet's corners must have integer coordinates, so
// we floor() the low bounds and ceil() the high bounds (with an epsilon
// fudge to avoid floating point noise nudging an exact integer the wrong
// way, and using floor/ceil rather than a cast to int, which truncates
// toward zero and mishandles negative values).
//
// Ambiguity: the statement's arc is unambiguous once "the arc through the
// given inner point" is taken literally -- the discuss board's own worked
// example (0 1)/(0 -1)/(1 0) => answer 2, not 8, confirms this reading and
// that plain (int) truncation is the trap, not the geometry itself.
#include <cstdio>
#include <cmath>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

double norm(double t) {
    while (t < 0) t += 2 * PI;
    while (t >= 2 * PI) t -= 2 * PI;
    return t;
}

int main() {
    double x1, y1, x2, y2, x3, y3;
    while (scanf("%lf %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &x3, &y3) == 6) {
        // circumcenter of (x1,y1),(x2,y2),(x3,y3)
        double ax = x2 - x1, ay = y2 - y1;
        double bx = x3 - x1, by = y3 - y1;
        double d = 2.0 * (ax * by - ay * bx);
        double ux = (by * (ax * ax + ay * ay) - ay * (bx * bx + by * by)) / d + x1;
        double uy = (ax * (bx * bx + by * by) - bx * (ax * ax + ay * ay)) / d + y1;
        double cx = ux, cy = uy;
        double r = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));

        double thetaA = norm(atan2(y1 - cy, x1 - cx));
        double thetaB = norm(atan2(y2 - cy, x2 - cx));
        double thetaC = norm(atan2(y3 - cy, x3 - cx));

        // ccw arc from A to B
        double dccw = norm(thetaB - thetaA);
        double dC = norm(thetaC - thetaA);

        double s, e; // arc = angles reachable from s going ccw by up to e
        if (dC <= dccw + EPS) {
            s = thetaA;
            e = dccw;
        } else {
            s = thetaB;
            e = norm(thetaA - thetaB);
        }

        double cardinal[4] = {0.0, PI / 2.0, PI, 3.0 * PI / 2.0};
        bool onArc[4];
        for (int k = 0; k < 4; k++) {
            double dd = norm(cardinal[k] - s);
            onArc[k] = (dd <= e + EPS);
        }

        double maxX = onArc[0] ? (cx + r) : (x1 > x2 ? x1 : x2);
        double maxY = onArc[1] ? (cy + r) : (y1 > y2 ? y1 : y2);
        double minX = onArc[2] ? (cx - r) : (x1 < x2 ? x1 : x2);
        double minY = onArc[3] ? (cy - r) : (y1 < y2 ? y1 : y2);

        double left = floor(minX + EPS);
        double right = ceil(maxX - EPS);
        double bottom = floor(minY + EPS);
        double top = ceil(maxY - EPS);

        int area = (int)((right - left) * (top - bottom) + 0.5);
        printf("%d\n", area);
    }
    return 0;
}
