// POJ 2464 - Brownie Points II
// Model: claude-sonnet-5
//
// Approach: Stan picks a vertical line x=x0 through some point; Ollie then
// picks a horizontal line y=y0 through some point that shares x0 (i.e. y0
// ranges over the y-coordinates of points on the vertical line).  For any
// specific point p=(x0,y0), Stan's score S(p) = #{points strictly greater in
// both x and y} + #{points strictly smaller in both} (points sharing x0 or
// y0 never count for either player, and this is exactly captured by strict
// dominance counts against ALL n points, not just the survivors, since a
// point sharing x0 automatically fails "x' > x0" / "x' < x0", and likewise
// for y0). Ollie's score O(p) is the complementary pair of quadrants
// (strictly less x & greater y, strictly greater x & less y).
//
// The statement is explicit that Stan "maximizes his smallest-possible
// score": for a fixed x0, Stan's guaranteed score is the MIN of S(p) over
// all points p on that vertical line (worst case over every legal Ollie
// reply, not just Ollie's self-optimal ones).  Stan then picks the x0 (there
// may be ties) that maximizes this minimum -- that maximum-of-minimums is
// the printed Stan score.  Among the y0 choices on an optimal x0 that attain
// that minimum (there may be several), Ollie -- who also wants to maximize
// his own score -- would pick the one with the largest O(p); the output
// lists the "(high) scores of Ollie", i.e. this per-optimal-x0 maximum,
// deduplicated and sorted increasing across every optimal x0.  Verified by
// hand against the sample (11 points): x0=2 forces Stan=7,Ollie=2; x0=0
// forces Stan=7,Ollie=3; every other x0 gives a smaller guaranteed Stan
// score (x0=-3 -> 5, x0=1 -> 6, x0=3 -> 0), so the answer is
// "Stan: 7; Ollie: 2 3;" matching the official output exactly.
//
// Dominance counts (TR/BL for S, TL/BR for O) are computed with two Fenwick
// tree sweeps over y-rank-compressed points, processing points in batches of
// equal x so that same-x points never count as dominating one another.
// O(n log n) per test case; coordinates are read as long long since the
// statement gives no explicit bound.

#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <algorithm>
using namespace std;

static inline int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return INT_MIN;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    long long v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10 + (c - '0');
        c = getchar();
    }
    return (int)(sign * v);
}

struct Fenwick {
    vector<int> t;
    int n;
    void init(int n_) { n = n_; t.assign(n + 1, 0); }
    void update(int i, int delta) {
        for (; i <= n; i += i & (-i)) t[i] += delta;
    }
    int query(int i) { // sum of [1..i]
        int s = 0;
        for (; i > 0; i -= i & (-i)) s += t[i];
        return s;
    }
};

int main() {
    int n;
    while ((n = readInt()) != 0) {
        vector<long long> X(n), Y(n);
        for (int i = 0; i < n; i++) {
            X[i] = readInt();
            Y[i] = readInt();
        }

        // coordinate-compress Y
        vector<long long> ys(Y);
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        int numY = (int)ys.size();
        vector<int> yrank(n);
        for (int i = 0; i < n; i++) {
            yrank[i] = (int)(lower_bound(ys.begin(), ys.end(), Y[i]) - ys.begin()) + 1;
        }

        // order of indices sorted by X ascending
        vector<pair<long long, int> > xi(n);
        for (int i = 0; i < n; i++) xi[i] = make_pair(X[i], i);
        sort(xi.begin(), xi.end());
        vector<int> ordAsc(n);
        for (int i = 0; i < n; i++) ordAsc[i] = xi[i].second;

        vector<int> BL(n), TL(n), TR(n), BR(n);

        // Pass 1: ascending X -> compute BL (x'<x,y'<y) and TL (x'<x,y'>y)
        {
            Fenwick bit;
            bit.init(numY);
            int inserted = 0;
            int i = 0;
            while (i < n) {
                int j = i;
                while (j < n && X[ordAsc[j]] == X[ordAsc[i]]) j++;
                for (int k = i; k < j; k++) {
                    int p = ordAsc[k];
                    int r = yrank[p];
                    BL[p] = bit.query(r - 1);
                    TL[p] = inserted - bit.query(r);
                }
                for (int k = i; k < j; k++) {
                    int p = ordAsc[k];
                    bit.update(yrank[p], 1);
                    inserted++;
                }
                i = j;
            }
        }

        // Pass 2: descending X -> compute TR (x'>x,y'>y) and BR (x'>x,y'<y)
        {
            Fenwick bit;
            bit.init(numY);
            int inserted = 0;
            int i = n - 1;
            while (i >= 0) {
                int j = i;
                while (j >= 0 && X[ordAsc[j]] == X[ordAsc[i]]) j--;
                // group is (j, i] in ordAsc, i.e. indices j+1..i
                for (int k = i; k > j; k--) {
                    int p = ordAsc[k];
                    int r = yrank[p];
                    TR[p] = inserted - bit.query(r);
                    BR[p] = bit.query(r - 1);
                }
                for (int k = i; k > j; k--) {
                    int p = ordAsc[k];
                    bit.update(yrank[p], 1);
                    inserted++;
                }
                i = j;
            }
        }

        vector<int> S(n), O(n);
        for (int i = 0; i < n; i++) {
            S[i] = TR[i] + BL[i];
            O[i] = TL[i] + BR[i];
        }

        // group by x (ordAsc groups), compute per-group min S and max O among ties at that min
        vector<int> groupMinS;
        vector<int> groupMaxO;
        {
            int i = 0;
            while (i < n) {
                int j = i;
                while (j < n && X[ordAsc[j]] == X[ordAsc[i]]) j++;
                int mn = S[ordAsc[i]];
                for (int k = i + 1; k < j; k++) mn = min(mn, S[ordAsc[k]]);
                int mx = -1;
                for (int k = i; k < j; k++) {
                    if (S[ordAsc[k]] == mn) mx = max(mx, O[ordAsc[k]]);
                }
                groupMinS.push_back(mn);
                groupMaxO.push_back(mx);
                i = j;
            }
        }

        int answer = groupMinS[0];
        for (size_t g = 1; g < groupMinS.size(); g++) answer = max(answer, groupMinS[g]);

        vector<int> ollie;
        for (size_t g = 0; g < groupMinS.size(); g++) {
            if (groupMinS[g] == answer) ollie.push_back(groupMaxO[g]);
        }
        sort(ollie.begin(), ollie.end());
        ollie.erase(unique(ollie.begin(), ollie.end()), ollie.end());

        printf("Stan: %d; Ollie:", answer);
        for (size_t k = 0; k < ollie.size(); k++) printf(" %d", ollie[k]);
        printf(";\n");
    }
    return 0;
}
