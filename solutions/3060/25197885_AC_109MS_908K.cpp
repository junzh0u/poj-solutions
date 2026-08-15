// POJ 3060 - Make it Manhattan
// Model: claude-sonnet-5
//
// Roads: avenues at x = a (mod D) for chosen offset a in [0,D), streets at
// y = b (mod D) for chosen offset b. A building is demolished iff its x
// residue equals a OR its y residue equals b. Let cntX[r] = #buildings with
// x mod D == r, cntY[r] similarly for y, and joint(a,b) = #buildings with
// both residues simultaneously. By inclusion-exclusion the number
// demolished for a given (a,b) is cntX[a]+cntY[b]-joint(a,b), and we must
// minimize this over all D*D choices of (a,b).
//
// For any (a,b) with joint(a,b)==0 the value is simply cntX[a]+cntY[b],
// whose minimum over all such pairs equals minCntX+minCntY (achieved by
// picking the row/column achieving each individual minimum - that combo's
// true value can only be <= this bound, since a positive joint count there
// would only reduce it further). So the overall minimum equals
// min(minCntX+minCntY, min over pairs (a,b) that actually co-occur among
// the input points of cntX[a]+cntY[b]-joint(a,b)). The co-occurring pairs
// are found by sorting the N (rx,ry) residue pairs, giving an O(N log N)
// algorithm independent of D (avoids the D*D matrix scan some accepted
// solutions on the board use, which relies on D*D being small enough per
// test case - safer here since the number of test cases isn't bounded by
// the statement).
//
// Verified against the sample (D=3, 10 points -> answer 1): residue counts
// cntX = {0:3,1:6,2:1}, cntY = {0:5,1:0,2:5}; minCntX+minCntY = 1+0 = 1,
// matching the expected output.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXD = 1005;
int cntX[MAXD], cntY[MAXD];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int D, N;
        scanf("%d %d", &D, &N);
        for (int i = 0; i < D; i++) { cntX[i] = 0; cntY[i] = 0; }

        static pair<int,int> pts[100005];
        for (int i = 0; i < N; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            int rx = ((x % D) + D) % D;
            int ry = ((y % D) + D) % D;
            cntX[rx]++;
            cntY[ry]++;
            pts[i] = make_pair(rx, ry);
        }

        int minCntX = cntX[0], minCntY = cntY[0];
        for (int i = 1; i < D; i++) {
            if (cntX[i] < minCntX) minCntX = cntX[i];
            if (cntY[i] < minCntY) minCntY = cntY[i];
        }
        int best = minCntX + minCntY;

        sort(pts, pts + N);
        int i = 0;
        while (i < N) {
            int j = i;
            while (j < N && pts[j] == pts[i]) j++;
            int joint = j - i;
            int rx = pts[i].first, ry = pts[i].second;
            int val = cntX[rx] + cntY[ry] - joint;
            if (val < best) best = val;
            i = j;
        }

        printf("%d\n", best);
    }
    return 0;
}
