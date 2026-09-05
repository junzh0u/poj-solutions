// POJ 3530 - A Modular Arithmetic Challenge
// Model: claude-sonnet-5
//
// Find minimal non-negative x with L <= (D*x mod M) <= R.
// Approach: binary search on x (the count of x in [0,X] whose D*x mod M
// falls in [L,R] is monotone nondecreasing in X, since it's a prefix
// count), using the classic floor-sum identity to evaluate, for a given
// upper bound n = X+1, how many i in [0,n-1] satisfy (D*i mod M) < C:
//   floor((D*i)/M) - floor((D*i - C)/M) == 1  iff  (D*i mod M) < C   (0<=C<=M)
// so that count = FloorSum(n,D,0,M) - FloorSum(n,D,-C,M), where
// FloorSum(n,a,b,m) = sum_{i=0}^{n-1} floor((a*i+b)/m) is computed in
// O(log) time by the standard Euclidean-like algorithm (as in the
// AtCoder Library's floor_sum). D is reduced mod M first so 0<=a<m at
// the start, which keeps every partial sum within the bound n^2/2 (all
// values fit comfortably in a 64-bit accumulator; __int128 is used for
// intermediate products purely as a safety margin).
//
// If L > M-1 no x can ever work (a remainder is always < M), so the
// answer is -1 immediately. Otherwise R is clipped to M-1 and the
// binary search runs over X in [0, M-1] (the sequence D*x mod M is
// periodic with period M/gcd(D,M) <= M, so any solution appears within
// the first M values).
//
// The discuss board's own editorial (recursive Euclid-style reduction)
// is reported to blow the recursion stack around D == M-1 unless the
// reflection D -> M-D trick is added; this floor-sum/binary-search
// formulation needs no such reflection since floor_sum's inner loop is
// already iterative and bounded by O(log M) swaps, so it sidesteps that
// failure mode entirely. Verified against the board's own stress case
// (1035076339 631418315 87622600 721919952 -> 1) and against an O(M)
// brute force for small M.

#include <cstdio>

typedef long long ll;
typedef __int128 lll;

// sum_{i=0}^{n-1} floor((a*i+b)/m), n>=0, m>=1, a,b may be any integers.
ll floor_sum(ll n, ll m, ll a, ll b) {
    if (n <= 0) return 0;
    lll ans = 0;
    if (a < 0) {
        ll a2 = a % m;
        if (a2 < 0) a2 += m;
        ans -= (lll)n * (n - 1) / 2 * ((a2 - a) / m);
        a = a2;
    }
    if (b < 0) {
        ll b2 = b % m;
        if (b2 < 0) b2 += m;
        ans -= (lll)n * ((b2 - b) / m);
        b = b2;
    }
    while (true) {
        if (a >= m) {
            ans += (lll)n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += (lll)n * (b / m);
            b %= m;
        }
        lll y_max = (lll)a * n + b;
        if (y_max < m) break;
        n = (ll)(y_max / m);
        b = (ll)(y_max % m);
        ll tmp = m; m = a; a = tmp;
    }
    return (ll)ans;
}

// count of i in [0,n-1] with (Dm*i mod M) < C, 0<=C<=M, 0<=Dm<M, M>=1
ll countLess(ll n, ll Dm, ll M, ll C) {
    if (C <= 0) return 0;
    if (n <= 0) return 0;
    return floor_sum(n, M, Dm, 0) - floor_sum(n, M, Dm, -C);
}

// count of i in [0,n-1] with L <= (Dm*i mod M) <= R  (0<=L<=R<M)
ll countRange(ll n, ll Dm, ll M, ll L, ll R) {
    return countLess(n, Dm, M, R + 1) - countLess(n, Dm, M, L);
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;
    while (N--) {
        ll M, D, L, R;
        scanf("%lld %lld %lld %lld", &M, &D, &L, &R);
        if (L > M - 1) {
            printf("-1\n");
            continue;
        }
        ll Rc = R < M - 1 ? R : M - 1;
        ll Dm = D % M;
        if (Dm < 0) Dm += M;
        // feasibility: does any x in [0, M-1] work?
        if (countRange(M, Dm, M, L, Rc) == 0) {
            printf("-1\n");
            continue;
        }
        ll lo = 0, hi = M - 1;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (countRange(mid + 1, Dm, M, L, Rc) >= 1) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
