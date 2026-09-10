// POJ 3148 - ASCII Art
// Model: claude-sonnet-5
// Approach: for each unit pixel square, compute the exact area of its
// intersection with the input polygon by clipping the polygon against the
// four half-planes bounding the pixel (Sutherland-Hodgman, applied
// sequentially), then taking |shoelace area| of the resulting polygon.
// This handles non-convex polygons correctly: even if a single half-plane
// clip disconnects the retained region into several pieces, the emitted
// vertex sequence still traces a closed boundary whose signed area equals
// the sum of the pieces' areas, because the connecting segments between an
// exit point and the next entry point lie exactly on the clip line.
// Percentage-to-character thresholds are compared with a small epsilon to
// avoid floating point boundary misclassification (the board's own hint:
// "注意精度" - watch out for precision).
// No statement ambiguity beyond the standard clip-and-measure geometry;
// output row order follows OY-up convention (row y=h-1 printed first).
#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

struct Pt { double x, y; };

static vector<Pt> clipHalfPlane(const vector<Pt>& poly, double a, double b, double c) {
    const double eps = 1e-9;
    vector<Pt> res;
    int n = (int)poly.size();
    if (n == 0) return res;
    for (int i = 0; i < n; i++) {
        const Pt& cur = poly[i];
        const Pt& prev = poly[(i + n - 1) % n];
        double curVal = a * cur.x + b * cur.y + c;
        double prevVal = a * prev.x + b * prev.y + c;
        bool curIn = curVal >= -eps;
        bool prevIn = prevVal >= -eps;
        if (curIn != prevIn) {
            double t = prevVal / (prevVal - curVal);
            Pt ip;
            ip.x = prev.x + t * (cur.x - prev.x);
            ip.y = prev.y + t * (cur.y - prev.y);
            res.push_back(ip);
        }
        if (curIn) res.push_back(cur);
    }
    return res;
}

static double polyArea(const vector<Pt>& poly) {
    double s = 0;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        const Pt& p1 = poly[i];
        const Pt& p2 = poly[(i + 1) % n];
        s += p1.x * p2.y - p2.x * p1.y;
    }
    return fabs(s) / 2.0;
}

int main() {
    int n, w, h;
    if (scanf("%d %d %d", &n, &w, &h) != 3) return 0;
    vector<Pt> poly(n);
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &poly[i].x, &poly[i].y);
    }

    vector<vector<char> > grid(h, vector<char>(w));
    const double eps = 1e-9;
    for (int py = 0; py < h; py++) {
        for (int px = 0; px < w; px++) {
            vector<Pt> cur = poly;
            cur = clipHalfPlane(cur, 1, 0, -(double)px);
            cur = clipHalfPlane(cur, -1, 0, (double)(px + 1));
            cur = clipHalfPlane(cur, 0, 1, -(double)py);
            cur = clipHalfPlane(cur, 0, -1, (double)(py + 1));
            double area = cur.empty() ? 0.0 : polyArea(cur);
            char c;
            if (area >= 1.0 - eps) c = '#';
            else if (area >= 0.75 - eps) c = '$';
            else if (area >= 0.5 - eps) c = 'o';
            else if (area >= 0.25 - eps) c = '+';
            else c = '.';
            grid[py][px] = c;
        }
    }

    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) putchar(grid[y][x]);
        putchar('\n');
    }
    return 0;
}
