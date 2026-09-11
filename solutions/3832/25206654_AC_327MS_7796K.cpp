// POJ 3832 - Posters
// Model: claude-sonnet-5
//
// Each poster is an outer rectangle with a rectangular hole cut out of it
// (the hole may touch the outer rectangle's edges, degenerating the poster
// into a C/L/O shape rather than a proper frame). We need the total area
// of the window covered by the union of all these "framed" shapes.
//
// Key idea: split each poster's frame (outer minus hole) into at most 4
// disjoint plain rectangles - a full-width bottom band, a full-width top
// band, and a left/right band spanning only the hole's y-range:
//   bottom: (x1,y1)-(x2,y3)
//   top:    (x1,y4)-(x2,y2)
//   left:   (x1,y3)-(x3,y4)
//   right:  (x4,y3)-(x2,y4)
// These 4 rectangles are pairwise disjoint and their union is exactly
// outer \ hole (verified against the board's discussion: sample input
// (0,0,10,10,0,0,9,9)+(2,2,8,8,3,3,7,7) gives area 39, matching the
// decomposition above with degenerate bands skipped when a band has zero
// width/height, e.g. when the hole touches an outer edge). Once every
// poster is turned into up to 4 plain rectangles, the answer is simply the
// area of the union of all these rectangles, computed with the standard
// sweep-line + segment-tree "union of rectangles" algorithm.
//
// Since all coordinates are bounded integers in [0,50000], the segment
// tree is built directly over that integer range (leaf i represents the
// unit interval [i,i+1)) instead of doing coordinate compression - this
// keeps the implementation simple and avoids any need for 64-bit ints:
// the running area is accumulated in a double (exact for integers up to
// 2^53, comfortably covering the max possible answer of 50000*50000 =
// 2.5e9, which does not fit in a 32-bit signed long anyway). This sidesteps
// the discuss board's confused "unsigned int AC / long long WA" report
// (almost certainly an unrelated bug in that submission, since the true
// answer fits in an unsigned 32-bit int and long long should handle it
// fine too) - using double removes the question entirely and works
// unchanged under either POJ compiler.
//
// After processing all matched +1/-1 update pairs for one test case the
// segment tree naturally returns to an all-zero state, so it does not need
// to be reset between test cases.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXC = 50000; // coordinate range [0, MAXC]

int segCnt[4 * MAXC + 10];
double segLen[4 * MAXC + 10];

void update(int node, int nl, int nr, int l, int r, int val) {
    if (r < nl || nr < l || l > r) return;
    if (l <= nl && nr <= r) {
        segCnt[node] += val;
    } else {
        int mid = (nl + nr) / 2;
        update(node * 2, nl, mid, l, r, val);
        update(node * 2 + 1, mid + 1, nr, l, r, val);
    }
    if (segCnt[node] > 0) {
        segLen[node] = (double)(nr - nl + 1);
    } else if (nl == nr) {
        segLen[node] = 0.0;
    } else {
        segLen[node] = segLen[node * 2] + segLen[node * 2 + 1];
    }
}

struct Event {
    int x, y1, y2, type;
};

Event events[400010];
int evCount;

void addRect(int x1, int y1, int x2, int y2) {
    if (x1 >= x2 || y1 >= y2) return; // degenerate band, skip
    events[evCount].x = x1; events[evCount].y1 = y1; events[evCount].y2 = y2; events[evCount].type = 1;
    evCount++;
    events[evCount].x = x2; events[evCount].y1 = y1; events[evCount].y2 = y2; events[evCount].type = -1;
    evCount++;
}

bool cmpEvent(const Event &a, const Event &b) {
    return a.x < b.x;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        evCount = 0;
        for (int i = 0; i < n; i++) {
            int x1, y1, x2, y2, x3, y3, x4, y4;
            scanf("%d%d%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
            addRect(x1, y1, x2, y3); // bottom band
            addRect(x1, y4, x2, y2); // top band
            addRect(x1, y3, x3, y4); // left band
            addRect(x4, y3, x2, y4); // right band
        }
        sort(events, events + evCount, cmpEvent);
        double area = 0.0;
        if (evCount > 0) {
            int prevX = events[0].x;
            for (int i = 0; i < evCount; i++) {
                area += (double)(events[i].x - prevX) * segLen[1];
                update(1, 0, MAXC - 1, events[i].y1, events[i].y2 - 1, events[i].type);
                prevX = events[i].x;
            }
        }
        printf("%.0f\n", area);
    }
    return 0;
}
