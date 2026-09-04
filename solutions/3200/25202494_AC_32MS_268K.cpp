// POJ 3200 - Babel Towers
// Model: claude-sonnet-5
//
// Each block k (radius r_k, center (x_k,y_k), mass proportional to r_k^2) rests
// directly on block k-1 (block 0 rests on the ground). Blocks are added bottom to
// top; after each addition the whole partial tower must be stable, i.e. at every
// joint the combined center of mass of everything resting above that joint must
// lie STRICTLY inside the support region there. For the ground (below block 0)
// the support region is simply disc(block 0). For the joint between block j-1 and
// block j (j>=1) the support region is the intersection of disc(block j-1) and
// disc(block j) (two discs touching in the same horizontal plane); a point lies
// in that intersection iff it lies strictly inside BOTH discs.
//
// Since blocks are only ever appended on top, once a prefix of m-1 blocks is
// known stable, adding block m-1 can only break a joint's stability by changing
// the suffix center of mass that includes the new block (every suffix not
// touching the new block was already fine). So for each new total count m we
// recheck every joint j=0..m-1, using prefix sums of mass=r^2, r^2*x, r^2*y to
// get each suffix's center of mass in O(1). Overall O(n^2) per test case.
//
// The board's discuss thread confirms the reported index is the newest block's
// 0-based index (a poster was confused expecting 1-based "4", answer being "3"
// for a 4-block case; someone clarified "it's 0-indexed"), and confirms a center
// of mass exactly on the boundary counts as Unfeasible (strict inequality).
//
// All comparisons are done with exact integer arithmetic (no floating point) to
// avoid any epsilon issue with the boundary case: comparing
// (X - mass*xr)^2 + (Y - mass*yr)^2  vs  r^2 * mass^2
// where X,Y,mass are exact suffix sums (fit in a 64-bit integer given the input
// bounds), so the squared terms need 128-bit integers, hence G++ (language 0)
// rather than the 32-bit-only "C++" compiler.
//
// Verified against the sample (including the exact indices) and stress tested
// against an independent floating point O(n^3) reference on random small cases.

#include <cstdio>

typedef long long ll;
typedef __int128 lll;

const int MAXN = 1005;

int X[MAXN], Y[MAXN], R[MAXN];
ll PM[MAXN], PX[MAXN], PY[MAXN];

// returns true if the point (xr,yr) with radius rr does NOT strictly contain
// the given suffix center of mass (i.e. the joint fails)
bool violates(ll massX2, ll sumX, ll sumY, int xr, int yr, int rr) {
    // center of mass = (sumX/massX2, sumY/massX2); condition strictly inside disc:
    // (sumX - massX2*xr)^2 + (sumY - massX2*yr)^2 < rr^2 * massX2^2
    lll A = (lll)sumX - (lll)massX2 * xr;
    lll B = (lll)sumY - (lll)massX2 * yr;
    lll lhs = A * A + B * B;
    lll rhs = (lll)rr * rr * (lll)massX2 * massX2;
    return !(lhs < rhs);
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d", &X[i], &Y[i], &R[i]);
        }
        PM[0] = PX[0] = PY[0] = 0;
        for (int i = 0; i < n; i++) {
            ll m = (ll)R[i] * R[i];
            PM[i + 1] = PM[i] + m;
            PX[i + 1] = PX[i] + m * X[i];
            PY[i + 1] = PY[i] + m * Y[i];
        }
        int failAt = -1;
        for (int m = 1; m <= n && failAt < 0; m++) {
            for (int j = 0; j < m; j++) {
                ll mass = PM[m] - PM[j];
                if (mass == 0) continue; // degenerate zero-mass suffix, cannot tip anything
                ll sx = PX[m] - PX[j];
                ll sy = PY[m] - PY[j];
                if (j == 0) {
                    if (violates(mass, sx, sy, X[0], Y[0], R[0])) {
                        failAt = m - 1;
                        break;
                    }
                } else {
                    if (violates(mass, sx, sy, X[j - 1], Y[j - 1], R[j - 1])) {
                        failAt = m - 1;
                        break;
                    }
                    if (violates(mass, sx, sy, X[j], Y[j], R[j])) {
                        failAt = m - 1;
                        break;
                    }
                }
            }
        }
        if (failAt < 0) {
            printf("Feasible\n");
        } else {
            printf("Unfeasible %d\n", failAt);
        }
    }
    return 0;
}
