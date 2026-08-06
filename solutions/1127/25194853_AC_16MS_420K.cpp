// POJ 1127 - Jack Straws
// Model: gpt-5.6-terra
// Union every pair of straws whose closed line segments intersect, including endpoint touches
// and collinear overlap; connectivity queries then reduce to disjoint-set membership.
#include <algorithm>
#include <iostream>

using namespace std;

struct Point {
    int x;
    int y;
};

struct Segment {
    Point a;
    Point b;
};

int cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool between(int value, int first, int second) {
    return min(first, second) <= value && value <= max(first, second);
}

bool onSegment(const Point& p, const Segment& s) {
    return cross(s.a, s.b, p) == 0 && between(p.x, s.a.x, s.b.x) && between(p.y, s.a.y, s.b.y);
}

bool hasOppositeSigns(int x, int y) {
    return (x < 0 && y > 0) || (x > 0 && y < 0);
}

bool intersects(const Segment& first, const Segment& second) {
    int c1 = cross(first.a, first.b, second.a);
    int c2 = cross(first.a, first.b, second.b);
    int c3 = cross(second.a, second.b, first.a);
    int c4 = cross(second.a, second.b, first.b);
    if (c1 == 0 && onSegment(second.a, first)) return true;
    if (c2 == 0 && onSegment(second.b, first)) return true;
    if (c3 == 0 && onSegment(first.a, second)) return true;
    if (c4 == 0 && onSegment(first.b, second)) return true;
    return hasOppositeSigns(c1, c2) && hasOppositeSigns(c3, c4);
}

struct DisjointSet {
    int parent[20];

    DisjointSet() {
        int i;
        for (i = 0; i < 20; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) parent[b] = a;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    while (cin >> n && n != 0) {
        Segment straws[20];
        int i;
        for (i = 0; i < n; ++i) {
            cin >> straws[i].a.x >> straws[i].a.y >> straws[i].b.x >> straws[i].b.y;
        }
        DisjointSet sets;
        for (i = 0; i < n; ++i) {
            int j;
            for (j = 0; j < i; ++j) {
                if (intersects(straws[i], straws[j])) sets.join(i, j);
            }
        }
        int a, b;
        while (cin >> a >> b && (a != 0 || b != 0)) {
            if (sets.find(a - 1) == sets.find(b - 1)) {
                cout << "CONNECTED\n";
            } else {
                cout << "NOT CONNECTED\n";
            }
        }
    }
    return 0;
}
