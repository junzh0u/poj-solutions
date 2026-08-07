// POJ 1389 - Area of Simple Polygons
// Model: claude-opus-5
//
// Area of the union of N axis-parallel rectangles, by the classic sweep line:
// compress the y coordinates into elementary intervals, sweep x left to right
// over the 2N vertical edges, and keep a segment tree over the elementary
// intervals holding, per node, a cover count and the covered length. A node
// covered by a pending +1 reports its whole span; otherwise it reports the sum
// of its children (0 at a leaf). Between consecutive event x's the covered
// length is constant, so area += cov[root] * dx. O(N log N) per test case.
//
// Statement ambiguity: the statement talks about picking a subset of the union's
// contour segments to build simple polygons and making the total area "as large
// as possible", which read literally would let an annulus be answered by its
// outer contour alone (hole included). The accepted reading is plainly the plain
// area of the union - it reproduces both official samples (18 and 10), and the
// discuss board reports POJ 1151 (Atlantis, pure union area) solutions AC'ing
// here verbatim. Coded to the union reading.
//
// Input framing: multiple test cases, each terminated by a line of four -1's,
// with an extra such line ending the input (confirmed on the discuss board).
// The board also reports the real data holds well over the stated 1000
// rectangles - one poster needed room for 15000 - so nothing here is a fixed
// array; the rectangles go into vectors. Areas reach 50000*50000 = 2.5e9, past
// 32-bit signed, so the accumulator is long long and printing goes through
// std::cout rather than a printf length modifier (POJ's old MinGW runtime is
// unreliable with %lld).

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Event {
    int x, y1, y2, d;
    Event() {}
    Event(int x_, int a, int b, int d_) : x(x_), y1(a), y2(b), d(d_) {}
    bool operator<(const Event& o) const { return x < o.x; }
};

static vector<int> ys;
static vector<int> cnt;
static vector<long long> cov;

static void pull(int node, int l, int r) {
    if (cnt[node] > 0) cov[node] = (long long)(ys[r] - ys[l]);
    else if (r - l == 1) cov[node] = 0;
    else cov[node] = cov[2 * node] + cov[2 * node + 1];
}

static void update(int node, int l, int r, int ql, int qr, int val) {
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) {
        cnt[node] += val;
    } else {
        int m = (l + r) / 2;
        update(2 * node, l, m, ql, qr, val);
        update(2 * node + 1, m, r, ql, qr, val);
    }
    pull(node, l, r);
}

int main() {
    int x1, y1, x2, y2;
    while (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
        if (x1 == -1 && y1 == -1 && x2 == -1 && y2 == -1) break; /* second -1 line: end of input */

        vector<Event> ev;
        ys.clear();
        for (;;) {
            if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
            if (y1 > y2) { int t = y1; y1 = y2; y2 = t; }
            if (x1 != x2 && y1 != y2) { /* a degenerate rectangle contributes nothing */
                ev.push_back(Event(x1, y1, y2, 1));
                ev.push_back(Event(x2, y1, y2, -1));
                ys.push_back(y1);
                ys.push_back(y2);
            }
            if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) { x1 = y1 = x2 = y2 = -1; }
            if (x1 == -1 && y1 == -1 && x2 == -1 && y2 == -1) break;
        }

        long long area = 0;
        if (!ev.empty()) {
            sort(ys.begin(), ys.end());
            ys.erase(unique(ys.begin(), ys.end()), ys.end());
            int m = (int)ys.size() - 1; /* elementary intervals [ys[i], ys[i+1]) */
            sort(ev.begin(), ev.end());
            cnt.assign(4 * (m + 1), 0);
            cov.assign(4 * (m + 1), 0);
            int prevx = ev[0].x;
            for (size_t i = 0; i < ev.size(); ++i) {
                area += cov[1] * (long long)(ev[i].x - prevx);
                prevx = ev[i].x;
                int a = (int)(lower_bound(ys.begin(), ys.end(), ev[i].y1) - ys.begin());
                int b = (int)(lower_bound(ys.begin(), ys.end(), ev[i].y2) - ys.begin());
                update(1, 0, m, a, b, ev[i].d);
            }
        }
        cout << area << "\n";
    }
    return 0;
}
