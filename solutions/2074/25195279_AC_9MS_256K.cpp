// POJ 2074 - Line of Sight
// Model: claude-opus-5
//
// House [hx1,hx2] at y=hy, property line [px1,px2] at y=py < hy, obstructions
// [ox1,ox2] at y=oy.  Find the longest stretch of the property line from which
// the WHOLE house is visible.
//
// An obstruction can only cut a ray from the property line to the house when
// py < oy < hy, so every other obstruction is dropped (see ambiguity note).
// For such an obstruction, a viewpoint p loses sight of part of the house iff
// the fan from p to [hx1,hx2] meets [ox1,ox2].  Writing u = (oy-py)/(hy-py),
// the ray p -> (h,hy) crosses y=oy at (1-u)p + u*h, so the crossings sweep
// [(1-u)p + u*hx1, (1-u)p + u*hx2].  That meets [ox1,ox2] exactly when
//     (ox1 - u*hx2)/(1-u)  <  p  <  (ox2 - u*hx1)/(1-u),
// i.e. the obstruction blocks an open interval of viewpoints whose endpoints
// are the shadows of (hx2,oy-side) and (hx1,oy-side) cast onto the property
// line.  Clip each such interval to [px1,px2], sort by left endpoint, sweep
// keeping the furthest right end covered so far, and take the largest gap
// (including the two ends).  O(n log n) per data set.
//
// Statement ambiguity, settled by the problem's discuss board: obstructions
// with oy == hy or oy == py, and obstructions outside (py,hy) altogether, are
// legal input and block nothing -- the statement only promises that such an
// obstruction does not overlap the house / property line in x, it never
// promises they are absent.  Also, "longest continuous portion" is measured as
// a length, so two blocked intervals that merely touch leave a zero-length
// gap, which is "No View", not a viewable point.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    double hx1, hx2, hy, px1, px2, py;
    while (scanf("%lf %lf %lf", &hx1, &hx2, &hy) == 3) {
        if (hx1 == 0.0 && hx2 == 0.0 && hy == 0.0) break;
        if (scanf("%lf %lf %lf", &px1, &px2, &py) != 3) break;
        int n;
        if (scanf("%d", &n) != 1) break;
        vector<pair<double, double> > iv;
        for (int i = 0; i < n; i++) {
            double a, b, y;
            if (scanf("%lf %lf %lf", &a, &b, &y) != 3) return 0;
            if (!(y > py + 1e-9 && y < hy - 1e-9)) continue;
            double lo = a - (hx2 - a) * (y - py) / (hy - y);
            double hi = b - (hx1 - b) * (y - py) / (hy - y);
            if (lo < px1) lo = px1;
            if (hi > px2) hi = px2;
            if (lo < hi) iv.push_back(make_pair(lo, hi));
        }
        sort(iv.begin(), iv.end());
        double best = 0.0, far = px1;
        for (size_t i = 0; i < iv.size(); i++) {
            if (iv[i].first - far > best) best = iv[i].first - far;
            if (iv[i].second > far) far = iv[i].second;
        }
        if (px2 - far > best) best = px2 - far;
        if (best < 1e-9) printf("No View\n");
        else printf("%.2f\n", best);
    }
    return 0;
}
