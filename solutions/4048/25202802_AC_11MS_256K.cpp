// POJ 4048 - Chinese Repeating Crossbow
// Model: gpt-5.6-terra
// Sweep the closed angular intervals subtended by the segments.  A segment
// through the shooter is hit by every ray; equal-angle starts precede ends.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Vec {
    int x, y;
};

int cross(const Vec &a, const Vec &b) {
    return a.x * b.y - a.y * b.x;
}

int dot(const Vec &a, const Vec &b) {
    return a.x * b.x + a.y * b.y;
}

bool upper(const Vec &a) {
    return a.y > 0 || (a.y == 0 && a.x >= 0);
}

bool angleLess(const Vec &a, const Vec &b) {
    bool ua = upper(a), ub = upper(b);
    if (ua != ub) return ua;
    int c = cross(a, b);
    if (c != 0) return c > 0;
    return false;
}

struct Event {
    Vec direction;
    int delta;
    Event(const Vec &d, int value) : direction(d), delta(value) {}
};

bool eventLess(const Event &a, const Event &b) {
    if (angleLess(a.direction, b.direction)) return true;
    if (angleLess(b.direction, a.direction)) return false;
    return a.delta > b.delta;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        int n;
        cin >> n;
        vector<Vec> a(n), b(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i].x >> a[i].y >> b[i].x >> b[i].y;
        Vec origin;
        cin >> origin.x >> origin.y;

        vector<Event> events;
        int always = 0, activeAtZero = 0;
        for (int i = 0; i < n; ++i) {
            Vec p = {a[i].x - origin.x, a[i].y - origin.y};
            Vec q = {b[i].x - origin.x, b[i].y - origin.y};
            if ((p.x == 0 && p.y == 0) || (q.x == 0 && q.y == 0)) {
                ++always;
                continue;
            }
            int c = cross(p, q);
            if (c == 0) {
                if (dot(p, q) < 0) {
                    ++always;
                } else {
                    events.push_back(Event(p, 1));
                    events.push_back(Event(p, -1));
                }
                continue;
            }

            Vec start = p, finish = q;
            if (c < 0) {
                start = q;
                finish = p;
            }
            events.push_back(Event(start, 1));
            events.push_back(Event(finish, -1));
            if (angleLess(finish, start)) ++activeAtZero;
        }

        sort(events.begin(), events.end(), eventLess);
        int current = always + activeAtZero;
        int answer = current;
        for (size_t i = 0; i < events.size(); ++i) {
            current += events[i].delta;
            if (current > answer) answer = current;
        }
        cout << answer << '\n';
    }
    return 0;
}
