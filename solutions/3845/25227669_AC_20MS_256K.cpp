// POJ 3845 - Fractal
// Model: gpt-6-astra
// Every replacement multiplies all segment lengths by the same factor.
// Descend through the segment containing the requested length fraction,
// composing orientation-preserving similarities in complex coordinates.
// Depth one is the input polyline; output has no spaces inside (x,y).
#include <cstdio>
#include <cmath>
#include <complex>
using namespace std;
typedef complex<double> Point;
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int n, depth;
        scanf("%d", &n);
        Point p[100], q[100];
        double length[100], total = 0;
        for (int i = 0; i < n; ++i) {
            double x, y;
            scanf("%lf%lf", &x, &y);
            p[i] = Point(x, y);
            if (i) {
                length[i-1] = abs(p[i] - p[i-1]);
                total += length[i-1];
            }
        }
        double f;
        scanf("%d%lf", &depth, &f);
        Point origin = p[0], scale = p[n-1] - p[0];
        for (int i = 0; i < n; ++i) q[i] = (p[i] - p[0]) / scale;
        for (int level = 0; level < depth; ++level) {
            if (f <= 0) { f = 0; break; }
            if (f >= 1) { f = 1; break; }
            double remain = f * total;
            int i = 0;
            while (i < n-2 && (length[i] == 0 || remain > length[i])) {
                remain -= length[i];
                ++i;
            }
            origin += scale * q[i];
            scale *= q[i+1] - q[i];
            f = length[i] == 0 ? 0 : remain / length[i];
        }
        Point answer = origin + scale * f;
        printf("(%.10f,%.10f)\n", answer.real(), answer.imag());
    }
    return 0;
}
