// POJ 1418 - Viva Confetti
// Model: gpt-5.6-terra
// Approach: partition every circle boundary at all intersection points and
// inspect a point on both sides of each resulting arc.  Every face of the
// circle arrangement is adjacent to such an arc, so its topmost disc is seen.
// The offset is chosen from the actual clearance to every other boundary.
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-14;

struct Circle {
    double x, y, r;
};

double dist(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2, dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

double normAngle(double a) {
    while (a < 0.0) a += 2.0 * PI;
    while (a >= 2.0 * PI) a -= 2.0 * PI;
    return a;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        vector<Circle> c(n);
        vector< vector<double> > cut(n);
        for (int i = 0; i < n; ++i) {
            scanf("%lf%lf%lf", &c[i].x, &c[i].y, &c[i].r);
            cut[i].push_back(0.0);
            cut[i].push_back(2.0 * PI);
        }
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
            double d = dist(c[i].x, c[i].y, c[j].x, c[j].y);
            if (d <= EPS || d >= c[i].r + c[j].r - EPS || d <= fabs(c[i].r - c[j].r) + EPS)
                continue;
            double base = atan2(c[j].y - c[i].y, c[j].x - c[i].x);
            double ai = acos((c[i].r * c[i].r + d * d - c[j].r * c[j].r) / (2.0 * c[i].r * d));
            double aj = acos((c[j].r * c[j].r + d * d - c[i].r * c[i].r) / (2.0 * c[j].r * d));
            double back = atan2(c[i].y - c[j].y, c[i].x - c[j].x);
            cut[i].push_back(normAngle(base - ai)); cut[i].push_back(normAngle(base + ai));
            cut[j].push_back(normAngle(back - aj)); cut[j].push_back(normAngle(back + aj));
        }
        vector<int> seen(n, 0);
        for (int i = 0; i < n; ++i) {
            double px = c[i].x, py = c[i].y;
            for (int j = n - 1; j >= 0; --j)
                if (dist(px, py, c[j].x, c[j].y) <= c[j].r + EPS) { seen[j] = 1; break; }
        }
        for (int i = 0; i < n; ++i) {
            sort(cut[i].begin(), cut[i].end());
            vector<double> a;
            for (size_t k = 0; k < cut[i].size(); ++k)
                if (a.empty() || cut[i][k] - a.back() > EPS) a.push_back(cut[i][k]);
            for (size_t k = 0; k + 1 < a.size(); ++k) {
                double ang = (a[k] + a[k + 1]) * 0.5;
                double ux = cos(ang), uy = sin(ang);
                double bx = c[i].x + c[i].r * ux, by = c[i].y + c[i].r * uy;
                double step = c[i].r;
                for (int j = 0; j < n; ++j) if (j != i) {
                    double clearance = fabs(dist(bx, by, c[j].x, c[j].y) - c[j].r);
                    if (clearance < step) step = clearance;
                }
                if (step <= EPS) continue;
                step *= 0.25;
                for (int side = -1; side <= 1; side += 2) {
                    double px = bx + side * step * ux, py = by + side * step * uy;
                    for (int j = n - 1; j >= 0; --j)
                        if (dist(px, py, c[j].x, c[j].y) <= c[j].r + EPS) { seen[j] = 1; break; }
                }
            }
        }
        int ans = 0;
        for (int i = 0; i < n; ++i) ans += seen[i];
        printf("%d\n", ans);
    }
    return 0;
}
