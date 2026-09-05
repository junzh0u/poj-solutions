// POJ 2736 - Square Count
// Model: claude-sonnet-5
//
// Approach: rooms are non-overlapping axis-aligned rectangles that may share
// a side. When the shared side has length m > 2 a door of length m-2,
// centered on the shared segment, connects the two rooms; otherwise the
// shared boundary is a solid wall. The statement guarantees no square
// overlaps more than two rooms, so the total count splits into:
//   (a) squares lying entirely inside a single room, and
//   (b) squares straddling exactly one shared-boundary door between two
//       adjacent rooms.
//
// (a) For a w x h room, sum_{k=1}^{min(w,h)} (w-k+1)(h-k+1) is computed in
// closed form via sum_i, sum_i^2 formulas (O(1) per room).
//
// (b) For two rooms sharing a boundary of length m (door length D = m-2,
// centered), let H1, H2 be the two rooms' depths perpendicular to the
// boundary. A straddling square of size k splits into a part of height a in
// room 1 and k-a in room 2, with 1<=a<=k-1, a<=H1, k-a<=H2. The count of
// valid a for a given k, f(k) = max(0, min(k-1,H1) - max(1,k-H2) + 1), is a
// piecewise-linear trapezoid in k (rises, plateaus, falls), and the
// footprint along the boundary direction must fit inside the door, giving
// (D-k+1) positions. The cross count is sum_k f(k)*(D-k+1); since f is
// piecewise linear in k this is a sum of quadratics evaluated in O(1) per
// piece (at most 3 pieces), so each room pair costs O(1) rather than O(D).
//
// Adjacency between two rooms is recognized only when one room's edge
// exactly meets the other's opposite edge (e.g. A.xmax == B.xmin), with a
// positive-length overlap on the perpendicular axis - never by matching two
// rooms' near edges (e.g. both xmin equal), which the discuss board reports
// as a real bug: some accepted solutions had to remove exactly that check
// because looser adjacency matching mis-fires on data containing incidental
// coordinate coincidences. The board also confirms shared sides of length
// <= 2 contribute zero crossing squares, which falls out of the formula
// automatically (D <= 0).
//
// Verified against both sample cases (86, 152) and a from-scratch
// grid/connectivity brute force (independently modeling walls and doors at
// the unit-cell level) over randomized small room configurations, plus
// mutation testing on the formula's edge handling.
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

struct Room {
    ll x1, y1, x2, y2; // normalized: x1<=x2, y1<=y2
};

ll singleCount(ll w, ll h) {
    ll m = min(w, h);
    if (m <= 0) return 0;
    ll a = w + 1, b = h + 1;
    ll sum_i = m * (m + 1) / 2;
    ll sum_i2 = m * (m + 1) * (2 * m + 1) / 6;
    return m * a * b - (a + b) * sum_i + sum_i2;
}

// sum_{k=lo}^{hi} (A*k+B)*(C*k+E), inclusive; 0 if lo>hi
ll sumLinearProduct(ll lo, ll hi, ll A, ll B, ll C, ll E) {
    if (lo > hi) return 0;
    ll n = hi - lo + 1;
    // sum k from lo..hi
    ll sumK = (lo + hi) * n / 2;
    // sum k^2 from lo..hi = sum_{1}^{hi} k^2 - sum_{1}^{lo-1} k^2
    ll sumK2hi = hi * (hi + 1) * (2 * hi + 1) / 6;
    ll loMinus1 = lo - 1;
    ll sumK2lo = loMinus1 * (loMinus1 + 1) * (2 * loMinus1 + 1) / 6;
    ll sumK2 = sumK2hi - sumK2lo;
    return A * C * sumK2 + (A * E + B * C) * sumK + B * E * n;
}

// Cross-room square count for a shared boundary of door length D,
// with perpendicular depths H1 (room1 side) and H2 (room2 side).
ll crossCount(ll D, ll H1, ll H2) {
    if (D <= 0) return 0;
    ll K = min(D, H1 + H2);
    if (K < 2) return 0;
    ll P = H1 + 1, Q = H2 + 1;
    ll bp1 = min(P, Q), bp2 = max(P, Q);
    ll total = 0;
    // g(k) = D - k + 1  -> C=-1, E=D+1
    ll C = -1, E = D + 1;
    // piece 1: k in [2, min(bp1,K)], f(k) = k - 1  -> A=1, B=-1
    {
        ll lo = 2, hi = min(bp1, K);
        total += sumLinearProduct(lo, hi, 1, -1, C, E);
    }
    // piece 2: k in [bp1+1, min(bp2,K)], f(k) = min(H1,H2) constant -> A=0,B=min(H1,H2)
    if (bp1 < bp2) {
        ll lo = bp1 + 1, hi = min(bp2, K);
        total += sumLinearProduct(lo, hi, 0, min(H1, H2), C, E);
    }
    // piece 3: k in [bp2+1, K], f(k) = H1+H2+1-k -> A=-1, B=H1+H2+1
    {
        ll lo = bp2 + 1, hi = K;
        total += sumLinearProduct(lo, hi, -1, H1 + H2 + 1, C, E);
    }
    return total;
}

int main() {
    int n;
    int caseNo = 1;
    while (scanf("%d", &n) == 1 && n != 0) {
        static Room rooms[1005];
        for (int i = 0; i < n; i++) {
            ll x1, y1, x2, y2;
            scanf("%lld %lld %lld %lld", &x1, &y1, &x2, &y2);
            rooms[i].x1 = min(x1, x2);
            rooms[i].x2 = max(x1, x2);
            rooms[i].y1 = min(y1, y2);
            rooms[i].y2 = max(y1, y2);
        }
        ll total = 0;
        for (int i = 0; i < n; i++) {
            ll w = rooms[i].x2 - rooms[i].x1;
            ll h = rooms[i].y2 - rooms[i].y1;
            total += singleCount(w, h);
        }
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // vertical boundary: A.xmax == B.xmin (A left of B) or B.xmax == A.xmin
                if (rooms[i].x2 == rooms[j].x1 || rooms[j].x2 == rooms[i].x1) {
                    ll ov = min(rooms[i].y2, rooms[j].y2) - max(rooms[i].y1, rooms[j].y1);
                    if (ov > 2) {
                        ll D = ov - 2;
                        ll H1 = rooms[i].x2 - rooms[i].x1; // width of room i
                        ll H2 = rooms[j].x2 - rooms[j].x1; // width of room j
                        total += crossCount(D, H1, H2);
                    }
                }
                // horizontal boundary: A.ymax == B.ymin or B.ymax == A.ymin
                if (rooms[i].y2 == rooms[j].y1 || rooms[j].y2 == rooms[i].y1) {
                    ll ov = min(rooms[i].x2, rooms[j].x2) - max(rooms[i].x1, rooms[j].x1);
                    if (ov > 2) {
                        ll D = ov - 2;
                        ll H1 = rooms[i].y2 - rooms[i].y1; // height of room i
                        ll H2 = rooms[j].y2 - rooms[j].y1; // height of room j
                        total += crossCount(D, H1, H2);
                    }
                }
            }
        }
        printf("Case %d: %lld\n", caseNo++, total);
    }
    return 0;
}
