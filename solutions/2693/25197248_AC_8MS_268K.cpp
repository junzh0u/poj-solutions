// POJ 2693 - Chocolate Chip Cookies
// Model: gpt-5.6-terra
// Enumerate the two radius-2.5 circles through every feasible pair of chips.
// An optimal nonempty circle can be translated until one chip is on its boundary,
// then rotated until a second is on the boundary (unless it contains one chip).

#include <cmath>
#include <cstdio>
#include <vector>

struct Point {
    double x, y;
};

static const double R = 2.5;
static const double EPS = 1e-8;

static int count_inside(const std::vector<Point>& p, double cx, double cy) {
    int result = 0;
    int i;
    for (i = 0; i < (int)p.size(); ++i) {
        double dx = p[i].x - cx;
        double dy = p[i].y - cy;
        if (dx * dx + dy * dy <= R * R + EPS) ++result;
    }
    return result;
}

int main() {
    std::vector<Point> p;
    Point q;
    int answer = 0;
    int i, j;
    while (std::scanf("%lf%lf", &q.x, &q.y) == 2) p.push_back(q);
    for (i = 0; i < (int)p.size(); ++i) {
        int single = count_inside(p, p[i].x, p[i].y);
        if (single > answer) answer = single;
        for (j = i + 1; j < (int)p.size(); ++j) {
            double dx = p[j].x - p[i].x;
            double dy = p[j].y - p[i].y;
            double d2 = dx * dx + dy * dy;
            double d;
            double mx, my, h, ox, oy;
            if (d2 > 4.0 * R * R + EPS || d2 < EPS) continue;
            d = std::sqrt(d2);
            mx = (p[i].x + p[j].x) * 0.5;
            my = (p[i].y + p[j].y) * 0.5;
            h = std::sqrt(R * R - d2 * 0.25);
            ox = -dy / d * h;
            oy = dx / d * h;
            int a = count_inside(p, mx + ox, my + oy);
            int b = count_inside(p, mx - ox, my - oy);
            if (a > answer) answer = a;
            if (b > answer) answer = b;
        }
    }
    std::printf("%d\n", answer);
    return 0;
}
