// POJ 2932 - Coneology
// Model: claude-opus-5
//
// Every cone has height == radius, so all cones share one slope and cone B
// lies inside cone A exactly when B's base disk lies inside A's base disk:
// dist(A,B) + rB <= rA.  "No two cones touch" therefore means the base circles
// are pairwise either strictly nested or strictly disjoint -- they never cross.
// The answer is the set of circles contained in no other circle, i.e. the roots
// of the containment forest.
//
// Sweep a vertical line over x.  Circle i is an entry event at x-r and an exit
// event at x+r.  The active set holds only the circles already known to be
// roots; being pairwise disjoint and all crossing the sweep line, they cut it
// in disjoint y-intervals, and each one's centre y lies in its own interval, so
// ordering the set by centre y matches the geometric order of those intervals.
// When circle i enters, its leftmost point is (x-r, y).  If some root A
// contains i then y lies inside A's interval, while every other root's interval
// -- hence its centre y -- lies wholly on one side of A's, so A is i's
// immediate predecessor or successor by centre y.  Checking those two
// neighbours is thus enough; if neither contains i, i is a root and is
// inserted.  A circle dominated by a non-root is dominated by that non-root's
// root, which is in the set, so nothing is missed.  O(n log n).
//
// Ties in event x are broken entries-before-exits: the data contains radius-0
// cones (reported on the discuss board and reproduced here as a test class),
// whose entry and exit share an x, and exits-first would leave such a cone in
// the active set forever.  A circle B inside A always satisfies
// A.x-A.r < B.x-B.r and B.x+B.r < A.x+A.r strictly, so no containment pair can
// tie and processing entries first never hides a container.
//
// Containment is tested on squared distances with no epsilon at all:
// dr = rA - rB > 0 and dx*dx + dy*dy < dr*dr.  Since touching is excluded by
// the statement, the exact comparison is the correct one, and the board reports
// that an eps of 1e-6 draws WA where a bare comparison is Accepted.

#include <cstdio>
#include <algorithm>
#include <set>
#include <utility>

using namespace std;

static const int MAXN = 40005;

int n;
double R[MAXN], X[MAXN], Y[MAXN];

struct Event {
    double x;
    int type; /* 0 = enter, 1 = leave */
    int id;
};

static bool evLess(const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.type < b.type;
}

/* is circle b strictly inside circle a ? */
static inline bool inside(int a, int b) {
    double dr = R[a] - R[b];
    if (dr <= 0) return false;
    double dx = X[a] - X[b];
    double dy = Y[a] - Y[b];
    return dx * dx + dy * dy < dr * dr;
}

Event ev[2 * MAXN];
bool top[MAXN];

int main() {
    if (scanf("%d", &n) != 1) return 0;
    int m = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%lf %lf %lf", &R[i], &X[i], &Y[i]);
        ev[m].x = X[i] - R[i]; ev[m].type = 0; ev[m].id = i; ++m;
        ev[m].x = X[i] + R[i]; ev[m].type = 1; ev[m].id = i; ++m;
    }
    sort(ev, ev + m, evLess);

    set<pair<double, int> > s;
    for (int e = 0; e < m; ++e) {
        int i = ev[e].id;
        if (ev[e].type == 0) {
            set<pair<double, int> >::iterator it =
                s.lower_bound(make_pair(Y[i], -1));
            bool dominated = false;
            if (it != s.end() && inside(it->second, i)) dominated = true;
            if (!dominated && it != s.begin()) {
                --it;
                if (inside(it->second, i)) dominated = true;
            }
            if (!dominated) {
                top[i] = true;
                s.insert(make_pair(Y[i], i));
            }
        } else {
            if (top[i]) s.erase(make_pair(Y[i], i));
        }
    }

    int cnt = 0;
    for (int i = 0; i < n; ++i) if (top[i]) ++cnt;
    printf("%d\n", cnt);
    bool first = true;
    for (int i = 0; i < n; ++i) {
        if (!top[i]) continue;
        if (!first) putchar(' ');
        printf("%d", i + 1);
        first = false;
    }
    putchar('\n');
    return 0;
}
