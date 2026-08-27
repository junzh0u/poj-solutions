// POJ 2172 - Bricks
// Model: gpt-5.6-terra
// Try each brick edge as the direction down the well.  The opposite face is
// a rectangle that may be rotated in the rectangular opening.  A minimum of
// max(projected_width / D, projected_height / E) is attained at an endpoint
// or where the two normalized projections are equal.
#include <cmath>
#include <iostream>

using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-9;

bool fitsFace(double a, double b, double d, double e) {
    double candidates[4];
    int count = 0;
    candidates[count++] = 0.0;
    candidates[count++] = PI / 2.0;

    // (a cos t + b sin t) / d = (a sin t + b cos t) / e.
    double x = a * e - b * d;
    double y = b * e - a * d;
    if (fabs(y) > EPS) {
        double t = atan2(-x, y);
        if (t < 0.0) t += PI;
        if (t >= -EPS && t <= PI / 2.0 + EPS) candidates[count++] = t;
    }

    for (int i = 0; i < count; ++i) {
        double t = candidates[i];
        double width = a * cos(t) + b * sin(t);
        double height = a * sin(t) + b * cos(t);
        if (width <= d + EPS && height <= e + EPS) return true;
        if (width <= e + EPS && height <= d + EPS) return true;
    }
    return false;
}

int main() {
    double side[3], d, e;
    if (!(cin >> side[0] >> side[1] >> side[2] >> d >> e)) return 0;
    bool ok = fitsFace(side[0], side[1], d, e) ||
              fitsFace(side[0], side[2], d, e) ||
              fitsFace(side[1], side[2], d, e);
    cout << (ok ? "YES" : "NO") << '\n';
    return 0;
}
