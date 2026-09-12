// POJ 1813 - Overlapped Shapes
// Model: claude-sonnet-5
// Approach: two shapes, each a rectangle or a circle; compute the area of
// their intersection and round to nearest integer.
//   - rect/rect: trivial axis-aligned clip.
//   - circle/circle: classic two-circular-segment lens formula.
//   - rect/circle: integrate, over x in the overlap of the rect's x-range
//     and the circle's x-range, the length of the y-interval that lies in
//     both the rectangle and the circle's vertical chord at that x. This
//     integrand is continuous (bounded) but has a sqrt-type derivative
//     singularity at the circle's tangent x-values, so it is integrated
//     with adaptive Simpson quadrature (time limit is generous: 10s).
// Statement gives "top-left"/"bottom-right" corners for the rectangle but
// since only the resulting point set matters, the four given coordinates
// are normalized to xmin/xmax/ymin/ymax rather than trusted for order.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

static const double PI = 3.14159265358979323846;

// ---- rectangle/circle integration ----
static double gRX1, gRX2, gRY1, gRY2, gCX, gCY, gCR;

double widthAt(double x) {
    double dx = x - gCX;
    double rr = gCR * gCR - dx * dx;
    if (rr < 0) return 0.0;
    double dy = sqrt(rr);
    double lo = max(gRY1, gCY - dy);
    double hi = min(gRY2, gCY + dy);
    if (hi < lo) return 0.0;
    return hi - lo;
}

double simpsonEval(double a, double b) {
    double c = (a + b) / 2.0;
    return (b - a) * (widthAt(a) + 4 * widthAt(c) + widthAt(b)) / 6.0;
}

double adaptiveSimpson(double a, double b, double eps, double whole, int depth) {
    double c = (a + b) / 2.0;
    double left = simpsonEval(a, c);
    double right = simpsonEval(c, b);
    if (depth <= 0 || fabs(left + right - whole) <= 15 * eps) {
        return left + right + (left + right - whole) / 15.0;
    }
    return adaptiveSimpson(a, c, eps / 2, left, depth - 1) +
           adaptiveSimpson(c, b, eps / 2, right, depth - 1);
}

double rectCircleArea(double xmin, double xmax, double ymin, double ymax,
                       double cx, double cy, double cr) {
    gRX1 = xmin; gRX2 = xmax; gRY1 = ymin; gRY2 = ymax;
    gCX = cx; gCY = cy; gCR = cr;
    double lo = max(xmin, cx - cr);
    double hi = min(xmax, cx + cr);
    if (hi <= lo) return 0.0;
    double whole = simpsonEval(lo, hi);
    return adaptiveSimpson(lo, hi, 1e-10, whole, 60);
}

double rectRectArea(double x1min, double x1max, double y1min, double y1max,
                     double x2min, double x2max, double y2min, double y2max) {
    double ox1 = max(x1min, x2min), ox2 = min(x1max, x2max);
    double oy1 = max(y1min, y2min), oy2 = min(y1max, y2max);
    double w = ox2 - ox1, h = oy2 - oy1;
    if (w <= 0 || h <= 0) return 0.0;
    return w * h;
}

double circleCircleArea(double cx1, double cy1, double r1,
                         double cx2, double cy2, double r2) {
    double dx = cx1 - cx2, dy = cy1 - cy2;
    double d = sqrt(dx * dx + dy * dy);
    if (d >= r1 + r2) return 0.0;
    if (d <= fabs(r1 - r2)) {
        double r = min(r1, r2);
        return PI * r * r;
    }
    double a1 = (d * d + r1 * r1 - r2 * r2) / (2 * d * r1);
    double a2 = (d * d + r2 * r2 - r1 * r1) / (2 * d * r2);
    if (a1 > 1) a1 = 1; if (a1 < -1) a1 = -1;
    if (a2 > 1) a2 = 1; if (a2 < -1) a2 = -1;
    double alpha = 2 * acos(a1);
    double beta = 2 * acos(a2);
    return 0.5 * r1 * r1 * (alpha - sin(alpha)) + 0.5 * r2 * r2 * (beta - sin(beta));
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 0; tc < n; tc++) {
        char buf[8];
        double area = 0.0;

        // shape 1
        char t1;
        double r1xmin=0,r1xmax=0,r1ymin=0,r1ymax=0,c1x=0,c1y=0,c1r=0;
        scanf("%s", buf); t1 = buf[0];
        if (t1 == 'R') {
            double x1,y1,x2,y2;
            scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
            r1xmin = min(x1,x2); r1xmax = max(x1,x2);
            r1ymin = min(y1,y2); r1ymax = max(y1,y2);
        } else {
            scanf("%lf %lf %lf", &c1x, &c1y, &c1r);
        }

        // shape 2
        char t2;
        double r2xmin=0,r2xmax=0,r2ymin=0,r2ymax=0,c2x=0,c2y=0,c2r=0;
        scanf("%s", buf); t2 = buf[0];
        if (t2 == 'R') {
            double x1,y1,x2,y2;
            scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
            r2xmin = min(x1,x2); r2xmax = max(x1,x2);
            r2ymin = min(y1,y2); r2ymax = max(y1,y2);
        } else {
            scanf("%lf %lf %lf", &c2x, &c2y, &c2r);
        }

        if (t1 == 'R' && t2 == 'R') {
            area = rectRectArea(r1xmin,r1xmax,r1ymin,r1ymax, r2xmin,r2xmax,r2ymin,r2ymax);
        } else if (t1 == 'C' && t2 == 'C') {
            area = circleCircleArea(c1x,c1y,c1r, c2x,c2y,c2r);
        } else if (t1 == 'R' && t2 == 'C') {
            area = rectCircleArea(r1xmin,r1xmax,r1ymin,r1ymax, c2x,c2y,c2r);
        } else { // C, R
            area = rectCircleArea(r2xmin,r2xmax,r2ymin,r2ymax, c1x,c1y,c1r);
        }

        printf("%d\n", (int)floor(area + 0.5));
    }
    return 0;
}
