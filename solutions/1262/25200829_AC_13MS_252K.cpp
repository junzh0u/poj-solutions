// POJ 1262 - Input
// Model: gpt-5.6-terra
// Check the required precedence directly: pairwise positive-area intersections,
// containment in [0,length] x [0,width], then equality of total tile area.
// The discussion board resolves the coordinate origin as (0,0).
#include <iostream>

using namespace std;

struct Rect {
    long long xl, yl, xh, yh;
};

static bool overlap(const Rect& a, const Rect& b) {
    return a.xl < b.xh && b.xl < a.xh && a.yl < b.yh && b.yl < a.yh;
}

int main() {
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        long long length, width;
        int n;
        cin >> length >> width >> n;
        Rect r[100];
        int i, j;
        for (i = 0; i < n; ++i)
            cin >> r[i].xl >> r[i].yl >> r[i].xh >> r[i].yh;

        bool badOverlap = false;
        for (i = 0; i < n; ++i)
            for (j = 0; j < i; ++j)
                if (overlap(r[i], r[j])) badOverlap = true;

        if (badOverlap) {
            cout << "NONDISJOINT\n";
            continue;
        }

        bool badContainment = false;
        long long area = 0;
        for (i = 0; i < n; ++i) {
            if (r[i].xl < 0 || r[i].yl < 0 || r[i].xh > length || r[i].yh > width)
                badContainment = true;
            area += (r[i].xh - r[i].xl) * (r[i].yh - r[i].yl);
        }

        if (badContainment)
            cout << "NONCONTAINED\n";
        else if (area != length * width)
            cout << "NONCOVERING\n";
        else
            cout << "OK\n";
    }
    return 0;
}
