// POJ 1788 - Building a New Depot
// Model: claude-sonnet-5
//
// Approach: The posts are exactly the corner (direction-change) vertices of
// a simple rectilinear polygon; there are no extra posts on straight runs.
// Each vertex has exactly one incident horizontal edge and one incident
// vertical edge. For a fixed x-coordinate, all vertices sharing that x are
// endpoints of vertical edges among themselves; if you sort them by y, the
// leftmost-unmatched point p1 cannot be joined to anything but its
// immediate neighbour p2 (joining to any farther point would force the
// edge to pass straight through p2, making it a non-corner, unnecessary
// post - contradiction). Removing p1,p2 and recursing shows the vertical
// edges are forced to be the consecutive pairs (1st,2nd), (3rd,4th), ...
// of the y-sorted list at that x. The symmetric argument groups by
// y-coordinate for horizontal edges. This uniquely determines the total
// fence length without ever reconstructing the polygon's cyclic order.
// Sum the paired coordinate differences over all x-groups (vertical) and
// all y-groups (horizontal).
//
// No statement ambiguity beyond the reconstruction argument above; the
// discuss board confirms "sorting/scanning", not convex hull, is the
// intended technique, matching this grouping approach.
#include <cstdio>
#include <algorithm>
#include <iostream>
using namespace std;

struct Pt { int x, y; };

bool cmpXY(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}
bool cmpYX(const Pt &a, const Pt &b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

int main() {
    int p;
    static Pt pts[100005];
    static Pt a[100005];
    static Pt b[100005];
    while (scanf("%d", &p) == 1 && p != 0) {
        for (int i = 0; i < p; ++i) {
            scanf("%d %d", &pts[i].x, &pts[i].y);
            a[i] = pts[i];
            b[i] = pts[i];
        }
        sort(a, a + p, cmpXY);
        sort(b, b + p, cmpYX);

        int total = 0;
        // vertical edges: group by x, pair consecutive y's
        int i = 0;
        while (i < p) {
            int j = i;
            while (j < p && a[j].x == a[i].x) ++j;
            for (int k = i; k + 1 < j; k += 2) {
                total += (a[k+1].y - a[k].y);
            }
            i = j;
        }
        // horizontal edges: group by y, pair consecutive x's
        i = 0;
        while (i < p) {
            int j = i;
            while (j < p && b[j].y == b[i].y) ++j;
            for (int k = i; k + 1 < j; k += 2) {
                total += (b[k+1].x - b[k].x);
            }
            i = j;
        }

        cout << "The length of the fence will be " << total << " units." << endl;
    }
    return 0;
}
