// POJ 1765 - November Rain
// Model: gpt-5.6-terra
// Sweep the distinct endpoint x-coordinates.  Between two coordinates the
// highest active segment receives all rain; separately link every lower end
// to the segment below it, then propagate flows in decreasing lower-end height.
// Segments touching a stream endpoint are deliberately included in the query.
#include <algorithm>
#include <cstdio>
#include <map>
#include <vector>
using namespace std;

struct Segment {
    int x1, y1, x2, y2;
    double water;
    double height(double x) const {
        return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
    }
    int lowX() const { return y1 < y2 ? x1 : x2; }
    int lowY() const { return y1 < y2 ? y1 : y2; }
};

static vector<Segment> seg;

struct LowerFirst {
    bool operator()(int a, int b) const { return seg[a].lowY() > seg[b].lowY(); }
};

static int highest(const vector<int>& active, double x) {
    int best = -1;
    for (size_t i = 0; i < active.size(); ++i)
        if (best == -1 || seg[active[i]].height(x) > seg[best].height(x)) best = active[i];
    return best;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    seg.resize(n);
    map<int, vector<int> > starts, ends, lows;
    vector<int> next(n, -1);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d%d", &seg[i].x1, &seg[i].y1, &seg[i].x2, &seg[i].y2);
        seg[i].water = 0.0;
        starts[seg[i].x1].push_back(i);
        ends[seg[i].x2].push_back(i);
        lows[seg[i].lowX()].push_back(i);
    }
    vector<int> active;
    int previous = starts.begin()->first;
    map<int, int> xs;
    for (map<int, vector<int> >::iterator it = starts.begin(); it != starts.end(); ++it) xs[it->first] = 1;
    for (map<int, vector<int> >::iterator it = ends.begin(); it != ends.end(); ++it) xs[it->first] = 1;
    bool first = true;
    for (map<int, int>::iterator xi = xs.begin(); xi != xs.end(); ++xi) {
        int x = xi->first;
        if (!first && x > previous && !active.empty()) {
            int top = highest(active, (previous + x) * 0.5);
            seg[top].water += x - previous;
        }
        vector<int> at = active;
        for (size_t i = 0; i < starts[x].size(); ++i) at.push_back(starts[x][i]);
        vector<int>& out = lows[x];
        for (size_t i = 0; i < out.size(); ++i) {
            int s = out[i], below = -1;
            double y = seg[s].lowY();
            for (size_t j = 0; j < at.size(); ++j) {
                int t = at[j];
                if (t == s) continue;
                double hy = seg[t].height(x);
                if (hy < y && (below == -1 || hy > seg[below].height(x))) below = t;
            }
            next[s] = below;
        }
        for (size_t i = 0; i < ends[x].size(); ++i) {
            int s = ends[x][i];
            active.erase(find(active.begin(), active.end(), s));
        }
        for (size_t i = 0; i < starts[x].size(); ++i) active.push_back(starts[x][i]);
        previous = x;
        first = false;
    }
    vector<int> order;
    for (int i = 0; i < n; ++i) order.push_back(i);
    sort(order.begin(), order.end(), LowerFirst());
    for (size_t i = 0; i < order.size(); ++i) {
        int s = order[i];
        if (next[s] != -1) seg[next[s]].water += seg[s].water;
    }
    for (int i = 0; i < n; ++i) printf("%.0f\n", seg[i].water);
    return 0;
}
