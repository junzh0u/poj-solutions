// POJ 2280 - Amphiphilic Carbon Molecules
// Model: claude-sonnet-5
// Approach: The optimal separating line can always be shifted, without
// decreasing the dissolved count, until it touches at least one input
// point (a point exactly on the line always dissolves, which is never
// worse than forcing it to one side). So it suffices to try, for every
// point P as a pivot the line passes through, the best line through P.
// For a fixed pivot, reflecting every hydrophobic (color 1) point through
// P turns "red must be on side A or on the line, blue must be on side B or
// on the line" into a single condition: the transformed point must lie in
// a closed half-plane of angular width pi through P. Sorting the other
// n-1 (transformed) vectors by angle and sweeping a closed half-turn
// window with a standard two-pointer (the window's low boundary can
// likewise always be moved, without loss, to coincide with some point's
// angle) finds the best count for that pivot in O(n log n); summing over
// all n choices of pivot gives O(n^2 log n), comfortably inside the 20s
// limit for n<=1000. Points exactly coincident with the pivot always lie
// on any line through it and are counted for free, outside the angular
// sweep (their direction is undefined). n<=3 is always fully dissolvable
// (run the line through any two of the points -- they dissolve regardless
// of color -- and label the remaining point's side to match its own
// color), kept as a cheap base case, though the general algorithm handles
// it correctly too.
// No statement ambiguity found; verified against a from-scratch brute
// force (direct simulation of every candidate direction/offset) on
// hundreds of small random cases, including forced coincident-point and
// collinear-triple configurations.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1005;

struct Pt { int x, y, c; };
static Pt pts[MAXN];

struct Vec { int x, y; };
static Vec vecs[MAXN];

static inline int half_of(const Vec &v) {
    if (v.y > 0 || (v.y == 0 && v.x > 0)) return 0;
    return 1;
}

static inline int cross(const Vec &a, const Vec &b) {
    return a.x * b.y - a.y * b.x;
}

static bool angleLess(const Vec &a, const Vec &b) {
    int ha = half_of(a), hb = half_of(b);
    if (ha != hb) return ha < hb;
    int cr = cross(a, b);
    if (cr != 0) return cr > 0;
    return false;
}

static int n;

static int solveCase() {
    if (n <= 3) return n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int k = 0;
        int freeCnt = 0;
        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            int dx = pts[j].x - pts[i].x;
            int dy = pts[j].y - pts[i].y;
            if (dx == 0 && dy == 0) { freeCnt++; continue; }
            if (pts[j].c == 1) { dx = -dx; dy = -dy; }
            vecs[k].x = dx; vecs[k].y = dy;
            k++;
        }
        sort(vecs, vecs + k, angleLess);
        int best = 0;
        if (k > 0) {
            int R = 0;
            best = 1;
            for (int L = 0; L < k; L++) {
                if (R < L) R = L;
                while (R - L < k - 1) {
                    int nr = (R + 1) % k;
                    if (cross(vecs[L], vecs[nr]) >= 0) R++;
                    else break;
                }
                int sz = R - L + 1;
                if (sz > best) best = sz;
            }
        }
        int total = 1 + freeCnt + best;
        if (total > ans) ans = total;
    }
    return ans;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d", &pts[i].x, &pts[i].y, &pts[i].c);
        }
        printf("%d\n", solveCase());
    }
    return 0;
}
