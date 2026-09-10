// POJ 1255 - Floors
// Model: claude-sonnet-5
// Approach: recursive guillotine-cut decomposition. At each rectangular
// region containing a set of tiles, look for a full straight cut (vertical
// or horizontal, aligned with a tile edge) that splits every tile cleanly
// into one side or the other. If found, recurse on both halves and take the
// max. If no such cut exists anywhere in the region, that region is an
// irreducible piece (possibly made of several tiles glued together, as in
// Figure 3 of the statement) and its area is a candidate answer.
// Ambiguity: none in the statement itself, but the discuss board (message
// 177412) notes the trick that a single tile may straddle a candidate line
// while the union of tiles does not force invalidity elsewhere -- handled
// naturally here since validity is checked against every tile in the
// current region, not just neighboring pairs.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static int xl_[105], yl_[105], xh_[105], yh_[105];
typedef vector<int> VI;

static int solve(const VI& idx, int x1, int y1, int x2, int y2) {
    int n = (int)idx.size();
    if (n <= 1) {
        return (x2 - x1) * (y2 - y1);
    }
    vector<int> xs;
    for (int i = 0; i < n; i++) {
        int id = idx[i];
        if (xl_[id] > x1 && xl_[id] < x2) xs.push_back(xl_[id]);
        if (xh_[id] > x1 && xh_[id] < x2) xs.push_back(xh_[id]);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    for (size_t k = 0; k < xs.size(); k++) {
        int c = xs[k];
        bool valid = true;
        for (int i = 0; i < n; i++) {
            int id = idx[i];
            if (!(xh_[id] <= c || xl_[id] >= c)) { valid = false; break; }
        }
        if (valid) {
            VI left, right;
            for (int i = 0; i < n; i++) {
                int id = idx[i];
                if (xh_[id] <= c) left.push_back(id); else right.push_back(id);
            }
            int a = solve(left, x1, y1, c, y2);
            int b = solve(right, c, y1, x2, y2);
            return a > b ? a : b;
        }
    }
    vector<int> ys;
    for (int i = 0; i < n; i++) {
        int id = idx[i];
        if (yl_[id] > y1 && yl_[id] < y2) ys.push_back(yl_[id]);
        if (yh_[id] > y1 && yh_[id] < y2) ys.push_back(yh_[id]);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    for (size_t k = 0; k < ys.size(); k++) {
        int c = ys[k];
        bool valid = true;
        for (int i = 0; i < n; i++) {
            int id = idx[i];
            if (!(yh_[id] <= c || yl_[id] >= c)) { valid = false; break; }
        }
        if (valid) {
            VI down, up;
            for (int i = 0; i < n; i++) {
                int id = idx[i];
                if (yh_[id] <= c) down.push_back(id); else up.push_back(id);
            }
            int a = solve(down, x1, y1, x2, c);
            int b = solve(up, x1, c, x2, y2);
            return a > b ? a : b;
        }
    }
    return (x2 - x1) * (y2 - y1);
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int L, W, t;
        scanf("%d %d", &L, &W);
        scanf("%d", &t);
        VI idx;
        for (int i = 0; i < t; i++) {
            scanf("%d %d %d %d", &xl_[i], &yl_[i], &xh_[i], &yh_[i]);
            idx.push_back(i);
        }
        int ans = solve(idx, 0, 0, L, W);
        printf("%d\n", ans);
    }
    return 0;
}
