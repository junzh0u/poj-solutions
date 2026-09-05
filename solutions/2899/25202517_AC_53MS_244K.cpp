// POJ 2899 - Fortune at El Dorado
// Model: claude-sonnet-5
//
// Approach: choose a rectangle with integer-coordinate vertices, sides
// parallel to the axes, positive area, area <= A, maximizing the number of
// points (trees) with border points counted as included (closed rectangle).
//
// Key fact: for the optimal solution's chosen point set S, its tight
// bounding box has width w0 = max(1, xmax(S)-xmin(S)) and height
// h0 = max(1, ymax(S)-ymin(S)), both positive integers in [1,999] since all
// coordinates lie in [1,1000]. Because w0*h0 <= A, taking W = w0 and
// H = floor(A/W) gives H >= h0, so a WxH window can be positioned to cover
// at least the same points, and W*H <= A remains valid. So it suffices to
// try every integer width W in [1,999], set H = min(999, floor(A/W)) (999
// caps the search since y spans at most 999), and for each (W,H) find the
// maximum number of points covered by SOME axis-aligned WxH window sliding
// freely (a classic technique: sort by x, two-pointer sweep for the width,
// and a "range add / global max" segment tree over a shifted y-anchor axis
// for the height, exactly as in POJ 2482's sliding-window-of-fixed-size
// technique). A standard floor-division block-skip (largest W achieving a
// given floor(A/W)) prunes redundant W's without changing correctness.
//
// Complexity: O(min(999, ~sqrt(A)) * F log(maxY)) per test case.
//
// Statement/board notes: the bbs claims real data may use F<=100 (despite
// the stated F<=1000), and one thread ties this to POJ 2482's technique
// (confirmed above). No further input-framing ambiguity found; F=0 or
// tiny A both degrade gracefully to answer 0 without special-casing.

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

static const int OFFSET = 1000;
static const int ND = 2010; // valid anchor-index domain is [2,2000]
static const int TREE_SZ = 4 * ND + 10;

int treeMax[TREE_SZ];
int lazyAdd[TREE_SZ];

inline void resetTree() {
    memset(treeMax, 0, sizeof(treeMax));
    memset(lazyAdd, 0, sizeof(lazyAdd));
}

inline void applyNode(int node, int val) {
    treeMax[node] += val;
    lazyAdd[node] += val;
}

inline void pushDown(int node) {
    if (lazyAdd[node] != 0) {
        applyNode(node * 2, lazyAdd[node]);
        applyNode(node * 2 + 1, lazyAdd[node]);
        lazyAdd[node] = 0;
    }
}

void update(int node, int l, int r, int ql, int qr, int val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        applyNode(node, val);
        return;
    }
    pushDown(node);
    int mid = (l + r) / 2;
    update(node * 2, l, mid, ql, qr, val);
    update(node * 2 + 1, mid + 1, r, ql, qr, val);
    treeMax[node] = max(treeMax[node * 2], treeMax[node * 2 + 1]);
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int F;
        long long A;
        scanf("%d %lld", &F, &A);
        vector<pair<int, int> > pts(F);
        for (int i = 0; i < F; i++) {
            scanf("%d %d", &pts[i].first, &pts[i].second);
        }
        sort(pts.begin(), pts.end());

        long long best = 0;
        if (F > 0 && A >= 1) {
            int W = 1;
            while (W <= 999) {
                long long trueH = A / W;
                if (trueH < 1) break;
                long long Hc = trueH < 999 ? trueH : 999;
                long long Wmax;
                if (trueH >= 999) {
                    Wmax = A / 999;
                } else {
                    Wmax = A / trueH;
                }
                if (Wmax > 999) Wmax = 999;
                if (Wmax < W) Wmax = W;
                int usedW = (int) Wmax;
                int H = (int) Hc;

                resetTree();
                int j = 0;
                for (int i = 0; i < F; i++) {
                    while (pts[j].first < pts[i].first - usedW) {
                        int a1 = pts[j].second - H + OFFSET;
                        int a2 = pts[j].second + OFFSET;
                        update(1, 1, ND, a1, a2, -1);
                        j++;
                    }
                    int a1 = pts[i].second - H + OFFSET;
                    int a2 = pts[i].second + OFFSET;
                    update(1, 1, ND, a1, a2, 1);
                    if (treeMax[1] > best) best = treeMax[1];
                }

                if (best >= F) break; // cannot possibly improve further
                W = usedW + 1;
            }
        }
        printf("%lld\n", best);
    }
    return 0;
}
