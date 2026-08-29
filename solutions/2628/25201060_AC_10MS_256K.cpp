// POJ 2628 - Table Legs
// Model: claude-sonnet-5
//
// t legs equally spaced around a circle. After cutting, all legs keep the
// same tabletop height H (level table), so each leg's remaining length is
// min(a[i], H) (cutting can only shorten). The amount cut is
// max(0, a[i] - H), and the table only actually rests (without rocking) on
// the legs whose tip touches the floor, i.e. those i with a[i] >= H. That
// contact set must not fit within a closed half-circle: the discuss board
// (message 33041) confirms the rule directly - the gap between two
// (cyclically) adjacent contact legs must be strictly less than 180
// degrees, i.e. the center of the regular polygon must be strictly inside
// the hull of contact points, not merely on its boundary. Verified against
// all three samples, including sample 2 where the 3-of-4 contact set with
// two antipodal legs (gap exactly 180 degrees) is REJECTED (must fall back
// to using all 4 legs), which is what pins down the strict inequality.
//
// Since cut(H) = sum(max(0, a[i]-H)) is convex, piecewise-linear and
// non-increasing in H, with breakpoints exactly at the a[i] values, the
// optimal H within any interval where the contact set is fixed is that
// interval's right endpoint - one of the a[i]. So it suffices to try every
// distinct a[i] as a candidate H, keep only those whose contact set is
// stable (max cyclic gap in index-steps g satisfies 2*g < t), and take the
// minimum cut among them. The full-contact candidate (H = min a[i]) is
// always stable, so a valid answer always exists.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int t;
    bool first = true;
    while (scanf("%d", &t) == 1 && t != 0) {
        vector<long long> a(t);
        for (int i = 0; i < t; ++i) scanf("%lld", &a[i]);

        vector<long long> cand(a);
        sort(cand.begin(), cand.end());
        cand.erase(unique(cand.begin(), cand.end()), cand.end());

        long long best = -1;
        for (size_t c = 0; c < cand.size(); ++c) {
            long long H = cand[c];
            vector<int> S;
            for (int i = 0; i < t; ++i) if (a[i] >= H) S.push_back(i);
            if (S.empty()) continue;

            int maxGap = 0;
            int n = (int)S.size();
            for (int k = 0; k < n; ++k) {
                int nxt = (k + 1) % n;
                int gap = (nxt == 0) ? (t - S[k] + S[0]) : (S[nxt] - S[k]);
                if (gap > maxGap) maxGap = gap;
            }
            if (2 * maxGap < t) {
                long long cut = 0;
                for (int i = 0; i < t; ++i) if (a[i] > H) cut += a[i] - H;
                if (best < 0 || cut < best) best = cut;
            }
        }

        if (!first) printf("\n");
        first = false;
        printf("%lld\n", best);
    }
    return 0;
}
