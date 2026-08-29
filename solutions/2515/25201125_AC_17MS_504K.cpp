// POJ 2515 - Birthday Cake
// Model: claude-sonnet-5
//
// Volume = sum_{k=1}^{N} k^M. N (up to 10^41) is read as a decimal string
// and M (3..100) is small. Because N can be astronomically larger than M,
// the sum cannot be accumulated term by term; instead sum_{k=1}^{n} k^M is
// a fixed polynomial P(n) of degree d = M+1 in n. We recover P at N via
// Lagrange interpolation from the d+1 exact sample values P(0..d), which
// are cheap to compute directly (k, M both small there). Multiplying the
// Lagrange formula through by d! keeps every intermediate an integer, so
// only big-integer (+ - * and one exact /) arithmetic is needed -- no
// rational/Bernoulli-number machinery.
//
// Board note (message 24820 et al.): the statement's "... is exactly k!"
// is a plain sentence ending in an exclamation mark, NOT k-factorial; the
// sample (N=3,M=3 -> 36 = 1+8+27) confirms length-of-cube = k, matching a
// reading many first-time solvers reportedly got wrong.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
const int BASE = 1000000000;
const int BASE_DIGITS = 9;

struct BigInt {
    int sign; // 1 or -1; zero uses sign 1
    vector<int> a; // little-endian base BASE

    BigInt() : sign(1) {}
    BigInt(ll v) { assign(v); }

    void assign(ll v) {
        sign = 1;
        if (v < 0) { sign = -1; v = -v; }
        a.clear();
        while (v > 0) { a.push_back((int)(v % BASE)); v /= BASE; }
    }

    void trim() {
        while (!a.empty() && a.back() == 0) a.pop_back();
        if (a.empty()) sign = 1;
    }

    bool isZero() const { return a.empty(); }

    static BigInt fromString(const string &s) {
        BigInt res;
        int pos = 0;
        res.sign = 1;
        if (!s.empty() && s[0] == '-') { res.sign = -1; pos = 1; }
        for (int i = (int)s.size(); i > pos; i -= BASE_DIGITS) {
            int start = i - BASE_DIGITS;
            if (start < pos) start = pos;
            res.a.push_back(atoi(s.substr(start, i - start).c_str()));
        }
        res.trim();
        return res;
    }

    string toString() const {
        if (a.empty()) return "0";
        string res = (sign < 0) ? "-" : "";
        char buf[16];
        sprintf(buf, "%d", a.back());
        res += buf;
        for (int i = (int)a.size() - 2; i >= 0; --i) {
            sprintf(buf, "%09d", a[i]);
            res += buf;
        }
        return res;
    }

    static int cmpAbs(const BigInt &x, const BigInt &y) {
        if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
        for (int i = (int)x.a.size() - 1; i >= 0; --i)
            if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
        return 0;
    }

    static BigInt addAbs(const BigInt &x, const BigInt &y) {
        BigInt res;
        res.a.assign(max(x.a.size(), y.a.size()) + 1, 0);
        for (size_t i = 0; i < x.a.size(); ++i) res.a[i] += x.a[i];
        for (size_t i = 0; i < y.a.size(); ++i) res.a[i] += y.a[i];
        for (size_t i = 0; i + 1 < res.a.size(); ++i)
            if (res.a[i] >= BASE) { res.a[i] -= BASE; res.a[i + 1]++; }
        res.trim();
        return res;
    }

    // x - y assuming |x| >= |y|
    static BigInt subAbs(const BigInt &x, const BigInt &y) {
        BigInt res;
        res.a = x.a;
        int borrow = 0;
        for (size_t i = 0; i < y.a.size() || borrow; ++i) {
            int sub = borrow + (i < y.a.size() ? y.a[i] : 0);
            res.a[i] -= sub;
            borrow = 0;
            if (res.a[i] < 0) { res.a[i] += BASE; borrow = 1; }
        }
        res.trim();
        return res;
    }

    friend BigInt operator+(const BigInt &x, const BigInt &y) {
        BigInt res;
        if (x.sign == y.sign) {
            res = addAbs(x, y);
            res.sign = x.isZero() && y.isZero() ? 1 : x.sign;
        } else if (cmpAbs(x, y) >= 0) {
            res = subAbs(x, y);
            res.sign = x.sign;
        } else {
            res = subAbs(y, x);
            res.sign = y.sign;
        }
        res.trim();
        return res;
    }

    friend BigInt operator-(const BigInt &x, const BigInt &y) {
        BigInt ny = y;
        if (!ny.isZero()) ny.sign = -ny.sign;
        return x + ny;
    }

    BigInt operator-() const {
        BigInt r = *this;
        if (!r.isZero()) r.sign = -r.sign;
        return r;
    }

    friend BigInt operator*(const BigInt &x, const BigInt &y) {
        BigInt res;
        if (x.isZero() || y.isZero()) return res;
        res.a.assign(x.a.size() + y.a.size(), 0);
        for (size_t i = 0; i < x.a.size(); ++i) {
            ll carry = 0;
            for (size_t j = 0; j < y.a.size() || carry; ++j) {
                ll cur = res.a[i + j] + carry;
                if (j < y.a.size()) cur += (ll)x.a[i] * y.a[j];
                res.a[i + j] = (int)(cur % BASE);
                carry = cur / BASE;
            }
        }
        res.sign = x.sign * y.sign;
        res.trim();
        return res;
    }

    // exact/floor magnitude division; sets q and r (r has sign of x, like C++ trunc division)
    static void divmod(const BigInt &x, const BigInt &y, BigInt &q, BigInt &r) {
        q.a.assign(x.a.size(), 0);
        r = BigInt(0);
        BigInt yAbs = y;
        yAbs.sign = 1;
        for (int i = (int)x.a.size() - 1; i >= 0; --i) {
            r.a.insert(r.a.begin(), x.a[i]);
            r.trim();
            int lo = 0, hi = BASE - 1, ans = 0;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                BigInt t = yAbs * BigInt((ll)mid);
                if (cmpAbs(t, r) <= 0) { ans = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            q.a[i] = ans;
            BigInt t = yAbs * BigInt((ll)ans);
            r = subAbs(r, t);
        }
        q.sign = (x.sign * y.sign);
        q.trim();
        r.sign = x.sign;
        r.trim();
    }
};

BigInt bigPow(BigInt base, int exp) {
    BigInt result(1);
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

int main() {
    int Z;
    if (scanf("%d", &Z) != 1) return 0;
    while (Z--) {
        char nbuf[64];
        int M;
        scanf("%s %d", nbuf, &M);
        string nstr(nbuf);
        int d = M + 1; // need d+1 = M+2 sample points: i = 0..d

        // y[i] = sum_{k=1}^{i} k^M, i = 0..d
        vector<BigInt> y(d + 1);
        y[0] = BigInt(0);
        for (int i = 1; i <= d; ++i) {
            y[i] = y[i - 1] + bigPow(BigInt((ll)i), M);
        }

        // Pascal's triangle up to row d, we only need row d: C[d][i]
        vector<vector<BigInt> > C(d + 1, vector<BigInt>());
        C[0].push_back(BigInt(1));
        for (int r = 1; r <= d; ++r) {
            C[r].assign(r + 1, BigInt(0));
            C[r][0] = BigInt(1);
            C[r][r] = BigInt(1);
            for (int c = 1; c < r; ++c) {
                C[r][c] = C[r - 1][c - 1] + C[r - 1][c];
            }
        }

        BigInt N = BigInt::fromString(nstr);

        // x_j = N - j, j = 0..d
        vector<BigInt> x(d + 1);
        for (int j = 0; j <= d; ++j) x[j] = N - BigInt((ll)j);

        // prefix[i] = prod_{j=0}^{i-1} x_j ; suffix[i] = prod_{j=i+1}^{d} x_j
        vector<BigInt> prefix(d + 2), suffix(d + 2);
        prefix[0] = BigInt(1);
        for (int i = 1; i <= d; ++i) prefix[i] = prefix[i - 1] * x[i - 1];
        suffix[d] = BigInt(1);
        for (int i = d - 1; i >= 0; --i) suffix[i] = suffix[i + 1] * x[i + 1];

        BigInt total(0);
        for (int i = 0; i <= d; ++i) {
            BigInt prod = prefix[i] * suffix[i];
            BigInt term = y[i] * C[d][i] * prod;
            if ((d - i) % 2 == 1) term = -term;
            total = total + term;
        }

        BigInt dFact(1);
        for (int i = 1; i <= d; ++i) dFact = dFact * BigInt((ll)i);

        BigInt q, r;
        BigInt::divmod(total, dFact, q, r);
        // r should be zero (exact division); q should be non-negative
        if (q.sign < 0) q.sign = 1; // safety: numeric identity guarantees non-negativity
        printf("%s\n", q.toString().c_str());
    }
    return 0;
}
