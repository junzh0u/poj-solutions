// POJ 3787 - Convex Hull of Lattice Points
// Model: claude-sonnet-5
// Approach: standard monotone-chain convex hull (Andrew's algorithm) using a
// strict cross-product test (<=0 pops) so collinear points lying on a hull
// edge are excluded from the vertex list, per the statement's "not all
// points on the convex hull polygon are vertices". This also naturally
// degenerates to the two extreme points when the whole point set is
// collinear. The hull comes out counter-clockwise; the answer needs
// clockwise starting at the vertex with largest y (ties: smallest x), so
// the hull is reversed and rotated to that start vertex before printing.
// Coordinates are unbounded in the statement but are clearly small lattice
// values (sample stays single/double digit); plain long keeps cross
// products safely within 32-bit range and avoids relying on long long,
// which POJ's older compilers do not all support.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

typedef long lg;
typedef pair<lg,lg> pt;

static lg cross(const pt&O, const pt&A, const pt&B){
    return (A.first-O.first)*(B.second-O.second) - (A.second-O.second)*(B.first-O.first);
}

// returns hull in CCW order, strictly convex (no collinear points), no
// duplicate closing point.
static vector<pt> convexHull(vector<pt> pts){
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    if (n < 3) return pts;
    vector<pt> H(2*n);
    int k = 0;
    for (int i = 0; i < n; ++i){
        while (k >= 2 && cross(H[k-2], H[k-1], pts[i]) <= 0) k--;
        H[k++] = pts[i];
    }
    int t = k + 1;
    for (int i = n - 2; i >= 0; --i){
        while (k >= t && cross(H[k-2], H[k-1], pts[i]) <= 0) k--;
        H[k++] = pts[i];
    }
    H.resize(k - 1);
    return H;
}

int main(){
    int P;
    if (scanf("%d", &P) != 1) return 0;
    for (int c = 0; c < P; ++c){
        int dsid, n;
        scanf("%d %d", &dsid, &n);
        vector<pt> pts(n);
        for (int i = 0; i < n; ++i){
            lg x, y;
            scanf("%ld %ld", &x, &y);
            pts[i] = pt(x, y);
        }
        vector<pt> hull = convexHull(pts);
        int m = (int)hull.size();

        vector<pt> out;
        if (m <= 1){
            out = hull;
        } else if (m == 2){
            // order: larger y first, tie smaller x
            pt a = hull[0], b = hull[1];
            bool aFirst = (a.second > b.second) || (a.second == b.second && a.first < b.first);
            out.push_back(aFirst ? a : b);
            out.push_back(aFirst ? b : a);
        } else {
            // hull is CCW; reverse to get CW
            reverse(hull.begin(), hull.end());
            // find start vertex: max y, tie min x
            int startIdx = 0;
            for (int i = 1; i < m; ++i){
                if (hull[i].second > hull[startIdx].second ||
                    (hull[i].second == hull[startIdx].second && hull[i].first < hull[startIdx].first)){
                    startIdx = i;
                }
            }
            out.resize(m);
            for (int i = 0; i < m; ++i) out[i] = hull[(startIdx + i) % m];
        }

        printf("%d %d\n", dsid, (int)out.size());
        for (size_t i = 0; i < out.size(); ++i){
            printf("%ld %ld\n", out[i].first, out[i].second);
        }
    }
    return 0;
}
