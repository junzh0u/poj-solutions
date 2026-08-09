// POJ 1693 - Counting Rectangles
// Model: gpt-5.6-terra
// For every pair of horizontal segments, count the vertical segments that
// intersect both; every pair among those verticals forms one rectangle.
// The statement guarantees that intersections cannot be created by collinear
// segments, so a rectangle has exactly these four segment witnesses.
#include <iostream>
#include <vector>
using namespace std;

struct Segment {
    int a, b, c;
};

static bool crosses(const Segment& h, const Segment& v) {
    return v.c >= h.a && v.c <= h.b && h.c >= v.a && h.c <= v.b;
}

int main() {
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int s;
        cin >> s;
        vector<Segment> horizontal, vertical;
        for (int i = 0; i < s; ++i) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            if (y1 == y2) {
                Segment h;
                h.a = x1 < x2 ? x1 : x2;
                h.b = x1 < x2 ? x2 : x1;
                h.c = y1;
                horizontal.push_back(h);
            } else {
                Segment v;
                v.a = y1 < y2 ? y1 : y2;
                v.b = y1 < y2 ? y2 : y1;
                v.c = x1;
                vertical.push_back(v);
            }
        }
        int answer = 0;
        for (int i = 0; i < (int)horizontal.size(); ++i) {
            for (int j = i + 1; j < (int)horizontal.size(); ++j) {
                int common = 0;
                for (int k = 0; k < (int)vertical.size(); ++k) {
                    if (crosses(horizontal[i], vertical[k]) && crosses(horizontal[j], vertical[k])) ++common;
                }
                answer += common * (common - 1) / 2;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
