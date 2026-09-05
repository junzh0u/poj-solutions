// POJ 2760 - End of Windless Days
// Model: claude-sonnet-5
//
// A point light at (lx,ly,lh) casts axis-aligned rectangular curtains'
// shadows onto the floor (z=0). Each curtain at height h projects to a
// floor rectangle obtained by scaling around (lx,ly) by factor
// t = lh/(lh-h) (central projection through the light). Because the
// scaling is separable and t>0, x1<x2 and y1<y2 are preserved under
// projection. The projected rectangle is then clipped to the floor
// bounds. The lit area is floor area minus the union of all clipped
// shadow rectangles, computed with a classic segment-tree rectangle
// union sweep (coordinate-compressed y, sweep over x events).
//
// Ambiguity/robustness notes from the discuss board: some judge data
// violates the stated x1<x2,y1<y2 (message 169071), so inputs are
// normalized with swaps; and shadows can project entirely outside the
// floor (message 356046/356047, "x1's projection can exceed maxx"),
// so degenerate/empty clipped rectangles are dropped before the sweep.
// The statement says "there may be many many cases", i.e. multiple
// datasets read until EOF with no leading count.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

static int M; // number of compressed y coordinates
static vector<double> ys;
static vector<int> cnt_;
static vector<double> len_;

void build(int node, int l, int r) {
    cnt_[node] = 0;
    len_[node] = 0;
    if (r - l > 1) {
        build(node * 2, l, (l + r) / 2);
        build(node * 2 + 1, (l + r) / 2, r);
    }
}

void update(int node, int l, int r, int ql, int qr, int val) {
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) {
        cnt_[node] += val;
    } else {
        update(node * 2, l, (l + r) / 2, ql, qr, val);
        update(node * 2 + 1, (l + r) / 2, r, ql, qr, val);
    }
    if (cnt_[node] > 0) {
        len_[node] = ys[r] - ys[l];
    } else if (r - l == 1) {
        len_[node] = 0;
    } else {
        len_[node] = len_[node * 2] + len_[node * 2 + 1];
    }
}

struct Rect {
    double x1, y1, x2, y2;
};

struct Event {
    double x;
    int y1idx, y2idx;
    int type;
};

bool cmpEvent(const Event &a, const Event &b) { return a.x < b.x; }

double unionArea(vector<Rect> &rects) {
    int n = (int)rects.size();
    if (n == 0) return 0.0;

    ys.clear();
    for (int i = 0; i < n; ++i) {
        ys.push_back(rects[i].y1);
        ys.push_back(rects[i].y2);
    }
    sort(ys.begin(), ys.end());
    // Exact-value dedup only. An epsilon-based merge here is a trap: if a
    // near-duplicate value gets dropped in favor of a representative it is
    // not bit-equal to, the later lower_bound() lookup for that exact
    // original value can miss the representative and jump to the next
    // cluster, silently shifting the mapped index range. Exact dedup keeps
    // every lookup value guaranteed present in the compressed array; any
    // leftover razor-thin gaps between near-equal doubles contribute
    // negligible (not incorrect) area.
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    M = (int)ys.size();
    if (M < 2) return 0.0;

    cnt_.assign(4 * M, 0);
    len_.assign(4 * M, 0.0);
    build(1, 0, M - 1);

    vector<Event> events;
    events.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        int li = (int)(lower_bound(ys.begin(), ys.end(), rects[i].y1) - ys.begin());
        int ri = (int)(lower_bound(ys.begin(), ys.end(), rects[i].y2) - ys.begin());
        if (ri <= li) continue;
        Event e1; e1.x = rects[i].x1; e1.y1idx = li; e1.y2idx = ri; e1.type = 1;
        Event e2; e2.x = rects[i].x2; e2.y1idx = li; e2.y2idx = ri; e2.type = -1;
        events.push_back(e1);
        events.push_back(e2);
    }
    sort(events.begin(), events.end(), cmpEvent);

    double area = 0.0;
    for (size_t i = 0; i + 1 <= events.size(); ++i) {
        update(1, 0, M - 1, events[i].y1idx, events[i].y2idx, events[i].type);
        if (i + 1 < events.size()) {
            double dx = events[i + 1].x - events[i].x;
            if (dx > 0) area += dx * len_[1];
        }
    }
    return area;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        double minx, miny, maxx, maxy;
        double lx, ly, lh;
        scanf("%lf %lf %lf %lf", &minx, &miny, &maxx, &maxy);
        scanf("%lf %lf %lf", &lx, &ly, &lh);

        vector<Rect> rects;
        rects.reserve(n);
        for (int i = 0; i < n; ++i) {
            double x1, y1, x2, y2, h;
            scanf("%lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &h);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            // guard against malformed heights (defensive; not expected
            // per statement 0<h<lh, but board reports other malformed
            // fields, so be safe against division blow-up).
            double denom = lh - h;
            if (denom <= 1e-9) continue;
            double t = lh / denom;
            double px1 = lx + t * (x1 - lx);
            double px2 = lx + t * (x2 - lx);
            double py1 = ly + t * (y1 - ly);
            double py2 = ly + t * (y2 - ly);
            if (px1 < minx) px1 = minx;
            if (px2 > maxx) px2 = maxx;
            if (py1 < miny) py1 = miny;
            if (py2 > maxy) py2 = maxy;
            if (px2 - px1 <= 1e-9) continue;
            if (py2 - py1 <= 1e-9) continue;
            Rect r; r.x1 = px1; r.y1 = py1; r.x2 = px2; r.y2 = py2;
            rects.push_back(r);
        }

        double floorArea = (maxx - minx) * (maxy - miny);
        double shadow = unionArea(rects);
        double lit = floorArea - shadow;
        if (lit < 0) lit = 0;
        printf("%.4f\n", lit);
    }
    return 0;
}
