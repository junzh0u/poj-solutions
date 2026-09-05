// POJ 1344 - Tree Size Problem
// Model: claude-sonnet-5
//
// A tree metric can be reconstructed exactly by repeatedly finding a pair of
// "cherry" leaves (leaves sharing a parent) and collapsing them into their
// parent. Two leaves i,j are a cherry iff D(i,k)-D(j,k) is the SAME constant
// for every other node k (four-point condition specialised to neighbours);
// this is an iff for genuine tree metrics, so any pair passing the check is
// safe to collapse regardless of which valid pair is picked first, and the
// final total edge sum does not depend on the order chosen.
//
// Once (i,j) found (with any other active node k as a reference):
//   w_i = (D(i,j)+D(i,k)-D(j,k))/2
//   w_j = D(i,j) - w_i
// add w_i+w_j to the answer, replace i,j by a single merged node m with
// D(m,k) = (D(i,k)+D(j,k)-D(i,j))/2 for every remaining k. Repeat until 2
// nodes remain; their distance is the final edge and is added directly.
//
// Note (POJ board, message 349623/351957): a tree's edge weights need not be
// integers - they can be half-integers - so the printed total can end in
// .5. All arithmetic here only ever adds/subtracts small integers and halves
// them, which is exact in IEEE double (no accumulated rounding), so the
// final total*2 is an exact integer and the .5 / integer split is read off
// safely with floor(x*2+0.5) (kept to plain int/long/double - no <cstdint>,
// no long long, since language 4 on POJ has no 64-bit type and none is
// needed given the tiny magnitudes here).
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

static double D[32][32];
static bool active[32];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 1; i <= n; i++) {
            D[i][i] = 0;
            active[i] = true;
        }
        for (int i = 1; i <= n - 1; i++) {
            for (int j = i + 1; j <= n; j++) {
                int v;
                scanf("%d", &v);
                D[i][j] = D[j][i] = (double)v;
            }
        }

        double ans = 0.0;
        int cnt = n;

        while (cnt > 2) {
            int bi = -1, bj = -1, bk = -1;
            for (int i = 1; i <= n && bi < 0; i++) {
                if (!active[i]) continue;
                for (int j = 1; j <= n && bi < 0; j++) {
                    if (j == i || !active[j]) continue;
                    // gather references (all other active nodes)
                    int ref = -1;
                    double diff0 = 0.0;
                    bool ok = true;
                    for (int k = 1; k <= n; k++) {
                        if (k == i || k == j || !active[k]) continue;
                        double d = D[i][k] - D[j][k];
                        if (ref < 0) {
                            ref = k;
                            diff0 = d;
                        } else if (fabs(d - diff0) > 1e-6) {
                            ok = false;
                            break;
                        }
                    }
                    if (ref < 0) {
                        // only i and j remain active besides nothing else:
                        // shouldn't happen since cnt>2 guarantees a third
                        continue;
                    }
                    if (ok) {
                        bi = i; bj = j; bk = ref;
                    }
                }
            }
            // bi,bj,bk found (guaranteed for a valid tree metric)
            double wi = (D[bi][bj] + D[bi][bk] - D[bj][bk]) / 2.0;
            double wj = D[bi][bj] - wi;
            ans += wi + wj;
            for (int k = 1; k <= n; k++) {
                if (k == bi || k == bj || !active[k]) continue;
                double nd = (D[bi][k] + D[bj][k] - D[bi][bj]) / 2.0;
                D[bi][k] = D[k][bi] = nd;
            }
            active[bj] = false;
            cnt--;
        }

        // two nodes remain
        int last1 = -1, last2 = -1;
        for (int i = 1; i <= n; i++) {
            if (active[i]) {
                if (last1 < 0) last1 = i;
                else last2 = i;
            }
        }
        ans += D[last1][last2];

        long twice = (long)floor(ans * 2.0 + 0.5);
        if (twice % 2 == 0) {
            printf("%ld\n", twice / 2);
        } else {
            printf("%ld.5\n", twice / 2);
        }
    }
    return 0;
}
