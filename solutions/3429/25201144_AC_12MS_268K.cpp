// POJ 3429 - Geometry with a ruler
// Model: claude-sonnet-5
//
// Approach: work entirely in exact projective (homogeneous) coordinates using
// arbitrary-precision integers, so no rounding/parallel-line special case is
// ever needed. Each point is a triple (X,Y,W) representing the affine point
// (X/W, Y/W). Original points start as (x,y,1). The line through two points,
// and the intersection point of two lines, are both computed with the exact
// same 3-vector cross product formula (point/line duality in P^2):
//   cross(u,v) = (u1*v2-u2*v1, u2*v0-u0*v2, u0*v1-u1*v0)
// line(a,b)  = cross(P[a], P[b])            -> (A,B,C) with Ax+By+Cw=0
// point(L1,L2) = cross(L1, L2)              -> (X,Y,W)
// A newly constructed point equals (0,0) iff X==0 and Y==0 (W != 0 is
// guaranteed by the statement: "such a point is guaranteed to exist", i.e.
// the two lines are never parallel, so we must never special-case a
// "parallel" / zero-denominator situation -- the board explicitly warns that
// doing so causes WA).
//
// Coordinates blow up multiplicatively when a construction repeatedly reuses
// the most recently built point (each new point's coordinates have degree
// equal to the sum of the degrees of its four operands, in terms of the
// original input numbers). A search over the worst possible reuse pattern
// for N>=4, M<=10 shows the theoretical maximum degree reached is about
// 17461, i.e. numbers with roughly 110000 decimal digits in the worst
// conceivable (adversarial) case -- schoolbook bignum multiplication handles
// this in well under a second (~3.5e8 word operations total), so no GCD
// reduction is needed for performance; we simply never reduce and check
// X==0 && Y==0 directly on the raw homogeneous integers.
//
// The two "Sample Input"/"Sample Output" blocks in the statement are two
// independent illustrative datasets (not one file), but since the statement
// gives no overall test-case count we read repeated datasets until EOF,
// which is safe for both a single dataset and a concatenation of several.
//
// Language: compiled as G++ (language 0) deliberately, to use unsigned
// __int128 as a safe multiply-accumulate carry buffer in the bignum
// multiplication (not available under POJ's old-MSVC language 4).

#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;

typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned __int128 u128;

static const u32 BASE = 1000000000u;

struct BigInt {
    int sign; // -1, 0, 1
    vector<u32> mag; // base 1e9, little endian, no leading zero limb

    BigInt() : sign(0) {}

    void fromLL(long long v) {
        mag.clear();
        if (v == 0) { sign = 0; return; }
        sign = (v < 0) ? -1 : 1;
        unsigned long long uv = (v < 0) ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
        while (uv > 0) {
            mag.push_back((u32)(uv % BASE));
            uv /= BASE;
        }
    }

    bool isZero() const { return sign == 0; }
};

static int cmpMag(const vector<u32>& a, const vector<u32>& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (size_t i = a.size(); i-- > 0; ) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static vector<u32> addMag(const vector<u32>& a, const vector<u32>& b) {
    vector<u32> res;
    res.reserve(max(a.size(), b.size()) + 1);
    u64 carry = 0;
    size_t n = max(a.size(), b.size());
    for (size_t i = 0; i < n || carry; i++) {
        u64 cur = carry;
        if (i < a.size()) cur += a[i];
        if (i < b.size()) cur += b[i];
        res.push_back((u32)(cur % BASE));
        carry = cur / BASE;
    }
    while (!res.empty() && res.back() == 0) res.pop_back();
    return res;
}

// requires a >= b
static vector<u32> subMag(const vector<u32>& a, const vector<u32>& b) {
    vector<u32> res;
    res.reserve(a.size());
    long long borrow = 0;
    for (size_t i = 0; i < a.size(); i++) {
        long long cur = (long long)a[i] - borrow - (i < b.size() ? (long long)b[i] : 0);
        if (cur < 0) { cur += BASE; borrow = 1; } else borrow = 0;
        res.push_back((u32)cur);
    }
    while (!res.empty() && res.back() == 0) res.pop_back();
    return res;
}

static BigInt add(const BigInt& a, const BigInt& b) {
    BigInt r;
    if (a.sign == 0) return b;
    if (b.sign == 0) return a;
    if (a.sign == b.sign) {
        r.mag = addMag(a.mag, b.mag);
        r.sign = a.sign;
    } else {
        int c = cmpMag(a.mag, b.mag);
        if (c == 0) { r.sign = 0; r.mag.clear(); }
        else if (c > 0) { r.mag = subMag(a.mag, b.mag); r.sign = a.sign; }
        else { r.mag = subMag(b.mag, a.mag); r.sign = b.sign; }
    }
    if (r.mag.empty()) r.sign = 0;
    return r;
}

static BigInt negateBig(const BigInt& a) {
    BigInt r = a;
    r.sign = -a.sign;
    return r;
}

static BigInt sub(const BigInt& a, const BigInt& b) {
    return add(a, negateBig(b));
}

static BigInt mul(const BigInt& a, const BigInt& b) {
    BigInt r;
    if (a.sign == 0 || b.sign == 0) { r.sign = 0; return r; }
    size_t la = a.mag.size(), lb = b.mag.size();
    vector<u128> tmp(la + lb, 0);
    for (size_t i = 0; i < la; i++) {
        u64 ai = a.mag[i];
        if (ai == 0) continue;
        for (size_t j = 0; j < lb; j++) {
            tmp[i + j] += (u128)ai * (u64)b.mag[j];
        }
    }
    vector<u32> res(la + lb + 2, 0);
    u128 carry = 0;
    size_t k = 0;
    for (; k < tmp.size(); k++) {
        u128 cur = tmp[k] + carry;
        res[k] = (u32)(cur % BASE);
        carry = cur / BASE;
    }
    while (carry > 0) {
        res[k++] = (u32)(carry % BASE);
        carry /= BASE;
    }
    while (!res.empty() && res.back() == 0) res.pop_back();
    r.mag = res;
    r.sign = (r.mag.empty()) ? 0 : (a.sign * b.sign);
    return r;
}

struct Point3 {
    BigInt x, y, w;
};

// generic cross product of two homogeneous 3-vectors; used both for
// "line through two points" and "point of intersection of two lines"
static void cross3(const BigInt& u0, const BigInt& u1, const BigInt& u2,
                    const BigInt& v0, const BigInt& v1, const BigInt& v2,
                    BigInt& r0, BigInt& r1, BigInt& r2) {
    r0 = sub(mul(u1, v2), mul(u2, v1));
    r1 = sub(mul(u2, v0), mul(u0, v2));
    r2 = sub(mul(u0, v1), mul(u1, v0));
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        vector<Point3> pts(n + 1); // 1-indexed
        for (int i = 1; i <= n; i++) {
            long long xi, yi;
            scanf("%lld %lld", &xi, &yi);
            pts[i].x.fromLL(xi);
            pts[i].y.fromLL(yi);
            pts[i].w.fromLL(1);
        }
        int m;
        scanf("%d", &m);
        int answer = 0;
        for (int k = 1; k <= m; k++) {
            int a, b, c, d;
            scanf("%d %d %d %d", &a, &b, &c, &d);
            // Even after the answer is found we must keep constructing:
            // later operations may reference this point by index, and we
            // still need to consume all of this dataset's input tokens
            // before the next EOF-loop iteration reads the next dataset.
            BigInt A1, B1, C1, A2, B2, C2;
            cross3(pts[a].x, pts[a].y, pts[a].w, pts[b].x, pts[b].y, pts[b].w, A1, B1, C1);
            cross3(pts[c].x, pts[c].y, pts[c].w, pts[d].x, pts[d].y, pts[d].w, A2, B2, C2);
            Point3 np;
            cross3(A1, B1, C1, A2, B2, C2, np.x, np.y, np.w);
            pts.push_back(np);
            if (answer == 0 && np.x.isZero() && np.y.isZero()) {
                answer = k;
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
