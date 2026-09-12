// POJ 1538 - Extrapolation Using a Difference Table
// Model: claude-sonnet-5
// Approach: The n-1st (last) difference column is assumed constant, which is
// exactly the statement that the original n points lie on a degree (n-1)
// polynomial in the index. Newton's forward-difference formula then gives the
// value at index n+k directly:
//   a(n+k) = sum_{i=0}^{n-1} C(n+k-1, i) * D_i
// where D_i is the i-th forward difference taken at the first element of the
// sequence (the top entry of column i in the difference table), and
// s = n+k-1 is the 0-based offset of term n+k from term 1. This was checked
// against all four sample cases by hand and matches exactly (including the
// last one, which requires signed intermediate differences).
//
// k has no stated upper bound ("no upper limit is given for k"), and the
// board shows a case with n=10, k=10000: building the difference table
// row-by-row for k extra rows is unnecessary and the resulting numbers can
// be far larger than 64 bits (C(n+k-1,9) grows like k^9), so C(N,i) is
// computed with a small arbitrary-precision integer (base 1e9, sign+magnitude)
// via the standard incremental identity C(N,i) = C(N,i-1)*(N-i+1)/i, which is
// always an exact integer division at each step. The forward differences
// themselves stay tiny (bounded by 2^9 times the input magnitude) and are
// kept in plain long long.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
const ll BASE = 1000000000LL;

struct Big {
    int sign; // -1, 0, or 1
    vector<ll> d; // magnitude, base BASE, little endian, no trailing zero chunk unless value is 0
    Big() : sign(0), d(1, 0) {}
};

static void trimVec(vector<ll> &d) {
    while (d.size() > 1 && d.back() == 0) d.pop_back();
}

static Big fromLL(ll x) {
    Big b;
    if (x == 0) { b.sign = 0; b.d.assign(1, 0); return b; }
    b.sign = x < 0 ? -1 : 1;
    ll ux = x < 0 ? -x : x;
    b.d.clear();
    while (ux > 0) { b.d.push_back(ux % BASE); ux /= BASE; }
    if (b.d.empty()) b.d.push_back(0);
    return b;
}

static vector<ll> mulSmallMag(const vector<ll> &a, ll m) {
    // m >= 0
    if (m == 0) { vector<ll> r(1, 0); return r; }
    vector<ll> res;
    ll carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        ll cur = a[i] * m + carry;
        res.push_back(cur % BASE);
        carry = cur / BASE;
    }
    while (carry > 0) { res.push_back(carry % BASE); carry /= BASE; }
    if (res.empty()) res.push_back(0);
    trimVec(res);
    return res;
}

static vector<ll> divSmallMag(const vector<ll> &a, ll dvs) {
    // exact division, dvs > 0
    vector<ll> res(a.size());
    ll rem = 0;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        ll cur = rem * BASE + a[i];
        res[i] = cur / dvs;
        rem = cur % dvs;
    }
    trimVec(res);
    return res;
}

static int cmpMag(const vector<ll> &a, const vector<ll> &b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = (int)a.size() - 1; i >= 0; --i)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static vector<ll> addMag(const vector<ll> &a, const vector<ll> &b) {
    vector<ll> res;
    size_t n = max(a.size(), b.size());
    ll carry = 0;
    for (size_t i = 0; i < n; ++i) {
        ll cur = carry;
        if (i < a.size()) cur += a[i];
        if (i < b.size()) cur += b[i];
        res.push_back(cur % BASE);
        carry = cur / BASE;
    }
    if (carry) res.push_back(carry);
    trimVec(res);
    return res;
}

static vector<ll> subMag(const vector<ll> &a, const vector<ll> &b) {
    // assumes a >= b
    vector<ll> res;
    ll borrow = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        ll cur = a[i] - borrow - (i < b.size() ? b[i] : 0);
        if (cur < 0) { cur += BASE; borrow = 1; } else borrow = 0;
        res.push_back(cur);
    }
    trimVec(res);
    return res;
}

static Big add(const Big &x, const Big &y) {
    if (x.sign == 0) return y;
    if (y.sign == 0) return x;
    Big r;
    if (x.sign == y.sign) {
        r.sign = x.sign;
        r.d = addMag(x.d, y.d);
    } else {
        int c = cmpMag(x.d, y.d);
        if (c == 0) { r.sign = 0; r.d.assign(1, 0); }
        else if (c > 0) { r.sign = x.sign; r.d = subMag(x.d, y.d); }
        else { r.sign = y.sign; r.d = subMag(y.d, x.d); }
    }
    if (r.d.size() == 1 && r.d[0] == 0) r.sign = 0;
    return r;
}

static Big mulSmallSigned(const Big &x, ll m) {
    Big r;
    if (x.sign == 0 || m == 0) { r.sign = 0; r.d.assign(1, 0); return r; }
    int sgn = x.sign * (m < 0 ? -1 : 1);
    r.d = mulSmallMag(x.d, m < 0 ? -m : m);
    r.sign = sgn;
    if (r.d.size() == 1 && r.d[0] == 0) r.sign = 0;
    return r;
}

static Big divSmallNonNeg(const Big &x, ll dvs) {
    // x must be nonnegative
    Big r;
    r.d = divSmallMag(x.d, dvs);
    r.sign = (r.d.size() == 1 && r.d[0] == 0) ? 0 : 1;
    return r;
}

static void printBig(const Big &x) {
    if (x.sign == 0) { printf("0"); return; }
    if (x.sign < 0) printf("-");
    printf("%lld", x.d.back());
    for (int i = (int)x.d.size() - 2; i >= 0; --i) printf("%09lld", x.d[i]);
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<ll> row(n);
        for (int i = 0; i < n; ++i) scanf("%lld", &row[i]);
        ll k;
        scanf("%lld", &k);

        vector<ll> diag(n);
        vector<ll> cur = row;
        for (int level = 0; level < n; ++level) {
            diag[level] = cur[0];
            if (level + 1 < n) {
                vector<ll> nxt(cur.size() - 1);
                for (size_t i = 0; i + 1 < cur.size(); ++i) nxt[i] = cur[i + 1] - cur[i];
                cur = nxt;
            }
        }

        ll N = (ll)n + k - 1; // 0-based offset of term n+k from term 1
        Big result = fromLL(0);
        Big C = fromLL(1); // C(N,0)
        for (int i = 0; i < n; ++i) {
            if (i > 0) {
                C = mulSmallSigned(C, N - i + 1); // C is nonnegative already, sign stays correct
                C = divSmallNonNeg(C, i);
            }
            Big term = mulSmallSigned(C, diag[i]);
            result = add(result, term);
        }

        printf("Term %lld of the sequence is ", (ll)n + k);
        printBig(result);
        printf("\n");
    }
    return 0;
}
