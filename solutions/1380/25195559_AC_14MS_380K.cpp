// POJ 1380 - Equipment Box
// Model: claude-opus-5
//
// Can an X*Y box be laid strictly inside an A*B tile (not touching the border),
// at any rotation?  A rectangle placed at angle t fits inside an axis-aligned
// rectangle iff its bounding box does, so with a = max(A,B), b = min(A,B),
// p = max(X,Y), q = min(X,Y) we need some t in [0, pi/2] with
//     f(t) = p*cos t + q*sin t < a   and   g(t) = p*sin t + q*cos t < b.
// Writing R = sqrt(p*p+q*q) and phi = atan(q/p): f(t) = R*cos(t-phi) and
// g(t) = R*sin(t+phi), both concave on the range, so each attains its minimum
// at an endpoint of any subinterval.
//   * min over t of g is min(q, p) = q, so q < b is necessary.
//   * f(0) = p, so p < a settles it axis-aligned.
//   * otherwise p >= a and {f < a} = (ta, pi/2], where ta = phi + acos(a/R).
//     On that interval min g = min(g(ta), g(pi/2)) = min(g(ta), p) and p >= b,
//     so the box fits iff g(ta) < b, i.e. iff
//         2*p*q*a + (p*p-q*q)*sqrt(p*p+q*q-a*a) < b*(p*p+q*q).
// Let S = p*p+q*q, D = p*p-q*q, L = b*S - 2*p*q*a; the test is L > 0 and
// D*D*(S-a*a) < L*L.  Both sides reach ~3e28, so they are compared exactly as
// 128-bit products built from 32-bit halves -- doubles cannot separate them,
// and 64-bit multiplication silently wraps (the discuss board's overflow trap).
//
// Ambiguity: "without touching its border" is strict, so exact tangency is a
// refusal.  Tangency cases exist over the integers (e.g. tile 35x28, box 40x5,
// where 4*p*q*a*b = 4*p*p*q*q + S*(a*a+b*b-S) holds exactly); the judges' own
// CERC-1999 reference answers some of them "possible" purely from double
// rounding, but its official 10000-case input contains none, so the strict
// reading is both correct and safe.  This program agrees with that reference
// on all 10000 official cases and on every a,b,p,q <= 40 except exact ties.

#include <cstdio>

typedef long long ll;
typedef unsigned long long ull;

static void mul128(ull a, ull b, ull &hi, ull &lo) {
    ull a0 = a & 0xFFFFFFFFULL, a1 = a >> 32;
    ull b0 = b & 0xFFFFFFFFULL, b1 = b >> 32;
    ull p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    ull mid = p01 + p10;
    ull c1 = (mid < p01) ? (1ULL << 32) : 0ULL;
    ull low = p00 + (mid << 32);
    ull c2 = (low < p00) ? 1ULL : 0ULL;
    lo = low;
    hi = p11 + (mid >> 32) + c1 + c2;
}

/* true iff x*y < z*w, exact for non-negative 64-bit factors */
static bool prodLess(ull x, ull y, ull z, ull w) {
    ull h1, l1, h2, l2;
    mul128(x, y, h1, l1);
    mul128(z, w, h2, l2);
    if (h1 != h2) return h1 < h2;
    return l1 < l2;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int iA, iB, iX, iY;
        if (scanf("%d %d %d %d", &iA, &iB, &iX, &iY) != 4) break;
        ll a = iA, b = iB, p = iX, q = iY, t;
        if (a < b) { t = a; a = b; b = t; }
        if (p < q) { t = p; p = q; q = t; }
        bool ok;
        if (q >= b) {
            ok = false;             /* shorter box side never clears shorter tile side */
        } else if (p < a) {
            ok = true;              /* fits axis-aligned */
        } else {
            ll S = p * p + q * q;
            ll D = p * p - q * q;
            ll L = b * S - 2 * p * q * a;
            if (L <= 0) ok = false;
            else ok = prodLess((ull)(D * D), (ull)(S - a * a), (ull)L, (ull)L);
        }
        puts(ok ? "Escape is possible." : "Box cannot be dropped.");
    }
    return 0;
}
