// POJ 1763 - Shortcut
// Model: gpt-5.6-terra
// Sort the break points twice.  The globally shortest horizontal or vertical
// connection must join adjacent points in the corresponding coordinate order.
// A connection is useful only when it is shorter than the route it replaces.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
    int pos;
};

static bool byX(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

static bool byY(const Point& a, const Point& b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

int main() {
    int n;
    string route;
    if (!(cin >> n >> route)) return 0;

    vector<Point> p(n + 1);
    int x = 0, y = 0, i;
    p[0].x = p[0].y = p[0].pos = 0;
    for (i = 0; i < n; ++i) {
        if (route[i] == 'N') ++y;
        else if (route[i] == 'S') --y;
        else if (route[i] == 'E') ++x;
        else --x;
        p[i + 1].x = x;
        p[i + 1].y = y;
        p[i + 1].pos = i + 1;
    }

    int bestLen = n + 1, bestB = n + 1, bestE = -1;
    char bestDir = '?';
    for (int pass = 0; pass < 2; ++pass) {
        if (pass == 0) sort(p.begin(), p.end(), byX);
        else sort(p.begin(), p.end(), byY);
        for (i = 1; i <= n; ++i) {
            bool aligned = pass == 0 ? p[i - 1].x == p[i].x
                                     : p[i - 1].y == p[i].y;
            if (!aligned) continue;
            int len = pass == 0 ? p[i].y - p[i - 1].y
                                : p[i].x - p[i - 1].x;
            int b = p[i - 1].pos < p[i].pos ? p[i - 1].pos : p[i].pos;
            int e = p[i - 1].pos < p[i].pos ? p[i].pos : p[i - 1].pos;
            if (e - b <= len) continue;
            if (len < bestLen || (len == bestLen &&
                (b < bestB || (b == bestB && e > bestE)))) {
                const Point& from = p[i - 1].pos == b ? p[i - 1] : p[i];
                const Point& to = p[i - 1].pos == e ? p[i - 1] : p[i];
                bestLen = len;
                bestB = b;
                bestE = e;
                if (to.x > from.x) bestDir = 'E';
                else if (to.x < from.x) bestDir = 'W';
                else if (to.y > from.y) bestDir = 'N';
                else bestDir = 'S';
            }
        }
    }
    cout << bestLen << ' ' << bestB << ' ' << bestE << ' ' << bestDir << '\n';
    return 0;
}
