// POJ 1377 - Good Approximation Problem
// Model: claude-sonnet-5
//
// alpha = p/q (reduced by gcd first, since the input pair need not be
// coprime). |G(alpha)| is the count of "best approximations of the second
// kind": a fraction y/x is such a best approximation iff no other y1/x1
// with 1<=x1<=x beats it strictly in |y - x*alpha| (equivalently, using
// the given identity, in |y*q - x*p|).
//
// Classical continued-fraction theory: writing alpha's continued fraction
// as [a0; a1, a2, ..., a_n], every convergent p_k/q_k for k=1..n is always
// a best approximation of the second kind. The k=0 convergent a0/1 is the
// only special case: it competes at x=1 against the alternative integer
// a0+1, and a0/1 wins strictly iff its fractional part f = alpha-a0 is
// < 1/2. Since f = 1/(a1+tail), f<1/2 fails when a1==1 (always dominated
// by a0+1, which equals p1/q1 at the SAME denominator 1 -- so it merely
// gets replaced, no net change to the count) and also when a1==2 with no
// further terms (n==1), which makes f exactly 1/2 -- an exact tie, so
// NEITHER a0/1 nor a0+1/1 is a good approximation there (net -1, with no
// replacement). Verified against the discuss board's posted data (5/2 ->
// 1, catching exactly this a1==2 tie case; the official samples 37/13 and
// 237/113 only exercise the a1==1 replacement case) and against an
// exhaustive brute force (direct check of the definition via exact
// cross-multiplied integer comparisons) for all p,q in [1,150]^2 plus
// random/boundary samples up to 10000.
#include <cstdio>

typedef long ll;

static ll mygcd(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        ll p, q;
        scanf("%ld %ld", &p, &q);
        ll g = mygcd(p, q);
        p /= g; q /= g;
        if (q == 1) { printf("1\n"); continue; }

        ll x = p, y = q;
        ll terms[40];
        int m = 0;
        while (y != 0) {
            terms[m++] = x / y;
            ll r = x % y;
            x = y; y = r;
        }
        int lastIdx = m - 1;
        ll a1 = terms[1];
        bool exclude = (a1 == 1) || (a1 == 2 && lastIdx == 1);
        ll answer = m - (exclude ? 1 : 0);
        printf("%ld\n", answer);
    }
    return 0;
}
