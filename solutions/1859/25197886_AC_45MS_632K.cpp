// POJ 1859 - The Perfect Symmetry
// Model: claude-sonnet-5
//
// If a finite point set X has a center of symmetry S, then reflecting every
// point about S is a bijection of X onto itself (each point pairs with its
// image, or is its own image when it coincides with S). Summing the
// identity P + P' = 2S over all points therefore gives sum(X) = N*S, so S
// is forced to be the centroid of the points -- there is at most one
// candidate. Compute 2*S = (2*sumX/N, 2*sumY/N); since all input points are
// integers, a genuine center of symmetry requires 2*S to be an integer
// point (otherwise some point's mirror image would be non-integer and thus
// absent from the set). If 2*sumX or 2*sumY isn't divisible by N, no center
// exists. Otherwise, for every point P check that its mirror 2S - P is also
// in the input set (points sorted + binary search on an encoded 64-bit
// key). If every mirror is present, S is the center; print it rounded to
// one decimal (exact, since 2*S is an integer so S is a multiple of 0.5).
//
// No statement ambiguity found; verified against both sample cases
// (symmetric octagon -> (2.0,3.0); four collinear points with sum not
// divisible by N -> "dangerous").
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

int main() {
    int n;
    static ll xs[20005], ys[20005];
    static ll keys[20005];
    while (scanf("%d", &n) == 1 && n != 0) {
        ll sumX = 0, sumY = 0;
        for (int i = 0; i < n; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            xs[i] = x;
            ys[i] = y;
            sumX += x;
            sumY += y;
        }
        // encode key = (x+200000) * 400001 + (y+200000)
        // x,y range roughly [-100000,100000], use generous offset
        const ll OFF = 200000;
        const ll BASE = 400001;
        for (int i = 0; i < n; i++) {
            keys[i] = (xs[i] + OFF) * BASE + (ys[i] + OFF);
        }
        sort(keys, keys + n);

        bool ok = false;
        ll twoSx = 0, twoSy = 0;
        if ((2 * sumX) % n == 0 && (2 * sumY) % n == 0) {
            twoSx = (2 * sumX) / n;
            twoSy = (2 * sumY) / n;
            ok = true;
            for (int i = 0; i < n && ok; i++) {
                ll mx = twoSx - xs[i];
                ll my = twoSy - ys[i];
                ll mk = (mx + OFF) * BASE + (my + OFF);
                if (!binary_search(keys, keys + n, mk)) {
                    ok = false;
                }
            }
        }

        if (ok) {
            double sx = twoSx / 2.0;
            double sy = twoSy / 2.0;
            printf("V.I.P. should stay at (%.1f,%.1f).\n", sx, sy);
        } else {
            printf("This is a dangerous situation!\n");
        }
    }
    return 0;
}
