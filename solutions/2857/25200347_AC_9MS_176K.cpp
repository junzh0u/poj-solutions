// POJ 2857 - Flipping Colors
// Model: gpt-5.6-terra
// Repeatedly locate the point in the two recursively painted diagonal
// rectangles.  Entering either one flips its color; the other diagonal ends
// the process.  Coordinates are renormalized after every descent.
// The input guarantees that no queried point lies on a generated boundary.
#include <cstdio>

int main() {
    double H, V, h, v;
    int cs = 0;
    while (std::scanf("%lf%lf%lf%lf", &H, &V, &h, &v) == 4 &&
           (H != 0.0 || V != 0.0 || h != 0.0 || v != 0.0)) {
        int n;
        std::scanf("%d", &n);
        std::printf("Case %d:\n", ++cs);
        while (n--) {
            double x, y;
            std::scanf("%lf%lf", &x, &y);
            x /= H;
            y /= V;
            bool white = false;
            for (;;) {
                if ((x < h && y > v) || (x > h && y < v)) break;
                white = !white;
                if (x < h) {
                    x /= h;
                    y /= v;
                } else {
                    x = (x - h) / (1.0 - h);
                    y = (y - v) / (1.0 - v);
                }
            }
            std::puts(white ? "white" : "black");
        }
    }
    return 0;
}
