// POJ 1687 - Buggy Sat
// Model: gpt-5.6-terra
// The outer face has the largest geometric area among the supplied faces.
// Compare twice-areas with the shoelace formula, so no division is needed.

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

struct Point {
    long long x;
    long long y;
};

int main() {
    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int n;
        cin >> n;
        vector<Point> p(n + 1);
        int i;
        for (i = 1; i <= n; ++i) cin >> p[i].x >> p[i].y;

        int faces;
        cin >> faces;
        long long bestArea = -1;
        int answer = 1;
        for (int face = 1; face <= faces; ++face) {
            int count;
            cin >> count;
            vector<int> v(count);
            for (i = 0; i < count; ++i) cin >> v[i];
            long long twiceArea = 0;
            for (i = 0; i < count; ++i) {
                const Point& a = p[v[i]];
                const Point& b = p[v[(i + 1) % count]];
                twiceArea += a.x * b.y - a.y * b.x;
            }
            if (twiceArea < 0) twiceArea = -twiceArea;
            if (twiceArea > bestArea) {
                bestArea = twiceArea;
                answer = face;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
