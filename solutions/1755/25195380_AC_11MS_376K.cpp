// POJ 1755 - Triathlon
// Model: claude-opus-5
//
// The judge picks section lengths x, y, z, each strictly > 0; contestant k's
// time is x/v_k + y/u_k + z/w_k, and i "wins" only by being the *unique*
// fastest.  So i is a Yes iff some x,y,z > 0 make time_i < time_j for all j.
//
// Substitute p = x/v_i, q = y/u_i, r = z/w_i (a bijection of the open positive
// octant onto itself).  Then time_i = p+q+r and time_j = p*v_i/v_j + q*u_i/u_j
// + r*w_i/w_j, so "i beats j", multiplied by v_j*u_j*w_j > 0, becomes an
// exactly-integral half-space through the origin:
//
//     (v_i-v_j)*u_j*w_j * p + (u_i-u_j)*v_j*w_j * q + (w_i-w_j)*v_j*u_j * r > 0
//
// The system is homogeneous, so normalise p+q+r = 1 and substitute r = 1-p-q:
// a bounded 2-D problem, "is this intersection of open half-planes non-empty?".
//
// Everything here is exact - no floating point anywhere, which is what the
// discuss board's long argument about eps (1e-8? 1e-16? 1e-17?) is really
// about.  Two devices make that affordable:
//
//   * Strictness by symbolic perturbation.  Each strict constraint becomes
//     a*p + b*q + (c - eps) >= 0 with eps an infinitesimal carried as a second
//     component.  For infinitesimal eps the feasible set is empty or
//     full-dimensional (a point of F(2eps) is interior to F(eps)), so the test
//     collapses to "is the clipped polygon non-empty?" - no area comparison,
//     hence no epsilon.
//   * Bounded operand growth.  Sutherland-Hodgman clipping of a convex polygon,
//     where each vertex is stored as the *pair of input lines* that define it
//     and recomputed from them.  Vertices therefore never compound, and every
//     predicate is the sign of a 3x3 determinant of the input coefficients.
//     Those coefficients are < 2e12, so the determinant is < 2.4e37 - beyond
//     64 bits, so signs are evaluated in a small sign+magnitude 128-bit type.
//
// The unit square is used as the starting polygon; it never binds, because the
// real region needs p > 0, q > 0 and p+q < 1.

#include <cstdio>

typedef long long ll;
typedef unsigned long long ull;

/* ---- signed 128-bit, sign + magnitude ---- */
struct I128 { int sg; ull hi, lo; };

static void umul64(ull a, ull b, ull &hi, ull &lo) {
    ull a0 = a & 0xffffffffULL, a1 = a >> 32;
    ull b0 = b & 0xffffffffULL, b1 = b >> 32;
    ull p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    ull mid = (p00 >> 32) + (p01 & 0xffffffffULL) + (p10 & 0xffffffffULL);
    lo = (p00 & 0xffffffffULL) | (mid << 32);
    hi = p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32);
}

static int ucmp(ull ah, ull al, ull bh, ull bl) {
    if (ah != bh) return ah < bh ? -1 : 1;
    if (al != bl) return al < bl ? -1 : 1;
    return 0;
}

static I128 mk(int sg, ull hi, ull lo) {
    I128 r;
    if (hi == 0 && lo == 0) { r.sg = 0; r.hi = 0; r.lo = 0; }
    else { r.sg = sg; r.hi = hi; r.lo = lo; }
    return r;
}

static I128 mulLL(ll a, ll b) {
    int sg = 1; ull ua, ub;
    if (a < 0) { sg = -sg; ua = (ull)(-a); } else ua = (ull)a;
    if (b < 0) { sg = -sg; ub = (ull)(-b); } else ub = (ull)b;
    ull hi, lo; umul64(ua, ub, hi, lo);
    return mk(sg, hi, lo);
}

static I128 mulI(const I128 &x, ll b) {
    if (x.sg == 0 || b == 0) return mk(0, 0, 0);
    int sg = x.sg; ull ub;
    if (b < 0) { sg = -sg; ub = (ull)(-b); } else ub = (ull)b;
    ull h1, l1; umul64(x.lo, ub, h1, l1);
    return mk(sg, h1 + x.hi * ub, l1);
}

static I128 iadd(const I128 &x, const I128 &y) {
    if (x.sg == 0) return y;
    if (y.sg == 0) return x;
    if (x.sg == y.sg) {
        ull lo = x.lo + y.lo;
        ull hi = x.hi + y.hi + (lo < x.lo ? 1ULL : 0ULL);
        return mk(x.sg, hi, lo);
    }
    int c = ucmp(x.hi, x.lo, y.hi, y.lo);
    if (c == 0) return mk(0, 0, 0);
    if (c > 0) {
        ull lo = x.lo - y.lo;
        ull hi = x.hi - y.hi - (x.lo < y.lo ? 1ULL : 0ULL);
        return mk(x.sg, hi, lo);
    }
    ull lo = y.lo - x.lo;
    ull hi = y.hi - x.hi - (y.lo < x.lo ? 1ULL : 0ULL);
    return mk(y.sg, hi, lo);
}

static I128 ineg(I128 x) { x.sg = -x.sg; return x; }

/* ---- half-planes  a*p + b*q + (c - e*eps) >= 0,  eps infinitesimal ---- */
struct Line { ll a, b, c; int e; };

static const int MAXL = 210;
static const int MAXV = 220;

static Line ln[MAXL];
static int nl;

/* vertex = intersection of two lines: (X/D, Y/D) with D > 0,
   X = X0 + eps*X1, Y = Y0 + eps*Y1 */
struct Vert { I128 D, X0, Y0; ll X1, Y1; };

static void mkVert(int i1, int i2, Vert &v) {
    const Line &L1 = ln[i1], &L2 = ln[i2];
    v.D  = iadd(mulLL(L1.a, L2.b), ineg(mulLL(L2.a, L1.b)));
    v.X0 = iadd(mulLL(L1.b, L2.c), ineg(mulLL(L2.b, L1.c)));
    v.Y0 = iadd(mulLL(L2.a, L1.c), ineg(mulLL(L1.a, L2.c)));
    v.X1 = -(L1.b * L2.e - L2.b * L1.e);
    v.Y1 = -(L2.a * L1.e - L1.a * L2.e);
    if (v.D.sg < 0) {
        v.D = ineg(v.D); v.X0 = ineg(v.X0); v.Y0 = ineg(v.Y0);
        v.X1 = -v.X1; v.Y1 = -v.Y1;
    }
}

static int sideOf(const Vert &v, int li) {
    const Line &L = ln[li];
    I128 n0 = iadd(iadd(mulI(v.X0, L.a), mulI(v.Y0, L.b)), mulI(v.D, L.c));
    if (n0.sg != 0) return n0.sg;
    I128 n1 = iadd(iadd(mulLL(v.X1, L.a), mulLL(v.Y1, L.b)), ineg(mulI(v.D, L.e)));
    return n1.sg;
}

static int pin[MAXV], pout[MAXV], qin[MAXV], qout[MAXV];
static int sv[MAXV];
static Vert vt[MAXV];

/* returns 1 if the intersection of every half-plane in ln[0..nl-1] with the
   unit square is non-empty */
static int feasible(int firstCut) {
    int m = 4;
    pin[0] = 0; pout[0] = 1;
    pin[1] = 1; pout[1] = 2;
    pin[2] = 2; pout[2] = 3;
    pin[3] = 3; pout[3] = 0;
    for (int li = firstCut; li < nl; li++) {
        int neg = 0, nonneg = 0;
        for (int k = 0; k < m; k++) {
            mkVert(pin[k], pout[k], vt[k]);
            sv[k] = sideOf(vt[k], li);
            if (sv[k] < 0) neg++; else nonneg++;
        }
        if (neg == 0) continue;
        if (nonneg == 0) return 0;
        int t = 0;
        for (int k = 0; k < m; k++) {
            int kn = (k + 1) % m, kp = (k + m - 1) % m;
            if (sv[k] >= 0) {
                qin[t]  = (sv[k] == 0 && sv[kp] < 0) ? li : pin[k];
                qout[t] = (sv[k] == 0 && sv[kn] < 0) ? li : pout[k];
                t++;
            }
            if (sv[k] > 0 && sv[kn] < 0) { qin[t] = pout[k]; qout[t] = li; t++; }
            if (sv[k] < 0 && sv[kn] > 0) { qin[t] = li; qout[t] = pout[k]; t++; }
        }
        if (t < 3) return 0;
        m = t;
        for (int k = 0; k < m; k++) { pin[k] = qin[k]; pout[k] = qout[k]; }
    }
    return 1;
}

static int V[105], U[105], W[105];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        for (int k = 0; k < n; k++) scanf("%d %d %d", &V[k], &U[k], &W[k]);
        for (int i = 0; i < n; i++) {
            nl = 0;
            /* bounding square [0,1]^2 (never binding: the real region needs
               p,q > 0 and p+q < 1) */
            ln[nl].a =  1; ln[nl].b =  0; ln[nl].c = 0; ln[nl].e = 0; nl++;
            ln[nl].a =  0; ln[nl].b =  1; ln[nl].c = 0; ln[nl].e = 0; nl++;
            ln[nl].a = -1; ln[nl].b =  0; ln[nl].c = 1; ln[nl].e = 0; nl++;
            ln[nl].a =  0; ln[nl].b = -1; ln[nl].c = 1; ln[nl].e = 0; nl++;
            int firstCut = nl;
            /* p > 0, q > 0, r = 1-p-q > 0 */
            ln[nl].a =  1; ln[nl].b =  0; ln[nl].c = 0; ln[nl].e = 1; nl++;
            ln[nl].a =  0; ln[nl].b =  1; ln[nl].c = 0; ln[nl].e = 1; nl++;
            ln[nl].a = -1; ln[nl].b = -1; ln[nl].c = 1; ln[nl].e = 1; nl++;
            for (int j = 0; j < n; j++) {
                if (j == i) continue;
                ll A = (ll)(V[i] - V[j]) * U[j] * W[j];
                ll B = (ll)(U[i] - U[j]) * V[j] * W[j];
                ll C = (ll)(W[i] - W[j]) * V[j] * U[j];
                ln[nl].a = A - C; ln[nl].b = B - C; ln[nl].c = C; ln[nl].e = 1;
                nl++;
            }
            puts(feasible(firstCut) ? "Yes" : "No");
        }
    }
    return 0;
}
