// POJ 3499 - Containers
// Model: claude-sonnet-5
// Approach: number of stacks needed is N = ceil(n/5) (each stack holds up to
// 5 containers). Stacks are arranged in a P x Q rectangular grid. Along one
// axis, P stacks are placed end to end sharing their 8ft short faces,
// extending along the 40ft length; a 4ft crane gap is needed next to a stack
// and between two stacks along that axis, giving dim1 = 40*P + 4*(P+1) =
// 44*P + 4. Along the other axis, Q stacks are placed side by side sharing
// their 40ft long faces, extending along the 8ft width; a 2ft catching gap
// is needed next to a stack and between two stacks along that axis, giving
// dim2 = 8*Q + 2*(Q+1) = 10*Q + 2. This formula was reverse-engineered from
// the statement's own worked figure (2 rows of 4 stacks = 8 stacks, 92 X 42
// = 3864: 44*2+4=92, 10*4+2=42) and verified against every row of the
// sample I/O (n=1,15,22,29,36,43 all reproduce exactly, including the
// ceil(n/5) rule confirmed on the discuss board: (n+4)/5).
//
// For a fixed P the minimal valid Q is ceil(N/P) (larger Q only grows the
// area), so we need min over P>=1 of dim1(P) * dim2(ceil(N/P)). Since for
// any P, either P or ceil(N/P) is <= isqrt(N)+1, enumerating d = 1..S+1 in
// both roles (d as P, and d as Q) covers the global optimum. Among minimal
// area candidates, keep the one with minimal length-width difference (the
// discuss board flags this as the point people misread, picking min width
// instead of min difference).
#include <cstdio>
#include <cmath>
using namespace std;

typedef long long ll;

ll isqrt_ll(ll x) {
    ll r = (ll)sqrtl((long double)x);
    while (r > 0 && r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        ll n;
        scanf("%lld", &n);
        ll N = (n + 4) / 5; // ceil(n/5)
        ll S = isqrt_ll(N);
        ll bestArea = -1, bestDiff = -1, bestLen = 0, bestWid = 0;
        for (ll d = 1; d <= S + 1; d++) {
            for (int role = 0; role < 2; role++) {
                ll P, Q;
                if (role == 0) { P = d; Q = (N + P - 1) / P; }
                else { Q = d; P = (N + Q - 1) / Q; }
                ll dim1 = 44 * P + 4;
                ll dim2 = 10 * Q + 2;
                ll area = dim1 * dim2;
                ll len = dim1 > dim2 ? dim1 : dim2;
                ll wid = dim1 > dim2 ? dim2 : dim1;
                ll diff = len - wid;
                if (bestArea == -1 || area < bestArea ||
                    (area == bestArea && diff < bestDiff)) {
                    bestArea = area; bestDiff = diff; bestLen = len; bestWid = wid;
                }
            }
        }
        printf("%lld X %lld = %lld\n", bestLen, bestWid, bestArea);
    }
    return 0;
}
