// POJ 2610 - Dog & Gopher
// Model: claude-sonnet-5
// Approach: the dog moves at double the gopher's speed, so it reaches a
// given hole in half the time the same distance would take the gopher.
// The gopher escapes through hole h iff dist(gopher,h) < dist(dog,h)/2,
// i.e. 4*dist(gopher,h)^2 < dist(dog,h)^2 (squared to avoid sqrt); a tie
// means the dog arrives first and gobbles the gopher, per both the
// statement and the discuss board. Scan holes in input order and report
// the first escapable one, or that the gopher cannot escape if none is.
// Ambiguity: none found - statement and board agree ties go to the dog
// and multiple escapable holes resolve to the first given in the input.
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double gx, gy, dx, dy;
    if (!(cin >> gx >> gy >> dx >> dy)) return 0;

    double hx, hy;
    bool found = false;
    double fx = 0.0, fy = 0.0;
    const double EPS = 1e-6;

    while (cin >> hx >> hy) {
        if (found) continue;
        double dgx = hx - gx, dgy = hy - gy;
        double ddx = hx - dx, ddy = hy - dy;
        double dg2 = dgx * dgx + dgy * dgy;
        double dd2 = ddx * ddx + ddy * ddy;
        if (dd2 - 4.0 * dg2 > EPS) {
            found = true;
            fx = hx;
            fy = hy;
        }
    }

    cout << fixed << setprecision(3);
    if (found) {
        cout << "The gopher can escape through the hole at (" << fx << "," << fy << ")." << endl;
    } else {
        cout << "The gopher cannot escape." << endl;
    }
    return 0;
}
