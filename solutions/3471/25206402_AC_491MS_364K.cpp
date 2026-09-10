// POJ 3471 - Integral Roots
// Model: claude-sonnet-5
//
// The polynomial is monic with integer coefficients (all coefficients
// fit in a 32-bit signed int per the statement). The board's own test
// case "1 / -2147483648" produces the single root 2147483648, which
// overflows a 32-bit int, so every root and every intermediate value
// below is kept in a 64-bit (or wider) type (and the source is
// submitted under G++, since POJ's language 4 has no long long).
//
// By the rational root theorem for a monic integer polynomial, every
// integer root divides the constant term exactly. So repeatedly:
//   - if the constant term is 0, x = 0 is a root; drop it (a shift,
//     no arithmetic) and recurse on the remaining lower-degree poly.
//   - otherwise enumerate the (positive and negative) divisors of the
//     constant term and test each one by synthetic division (Horner);
//     whichever divides evenly is a root -- deflate by it and repeat.
//   - if no divisor of a nonzero constant term works, there are no
//     more integer roots; stop.
//
// Overflow-safe root test: while evaluating a candidate r via Horner
// against the CURRENT (possibly already deflated) polynomial, once
// the running value's magnitude exceeds M = the current polynomial's
// own max |coefficient| (recomputed fresh for each deflated state --
// dividing out a root of magnitude 1 does not shrink coefficients the
// way dividing by |r|>=2 does, so a later quotient's own bound can
// exceed the original problem's 32-bit bound; a *fixed* 2^31 cutoff
// here was tried first and is provably too tight, silently discarding
// genuine larger roots -- confirmed on a constructed 90-degree case
// with true roots {1,2} where it reported only {1}) with |r| >= 2,
// the value only grows in magnitude from there on: |v*r + c| >=
// |v|*|r| - M > |v| once |v| > M/(|r|-1), and M/(|r|-1) <= M for
// |r| >= 2 -- so it can never return to exactly zero, and testing
// aborts immediately as "not a root". This also proves a genuine
// root's intermediate synthetic-division values never exceed
// M/(|r|-1) <= M, so no real root is ever pruned. The multiply v*r
// itself is done in __int128 (v can be up to ~1e13 in adversarial
// deflated states, r up to ~2^31) to stay exact before the check.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef __int128 lll;

static ll absll(ll x) { return x < 0 ? -x : x; }
static lll absl128(lll x) { return x < 0 ? -x : x; }

// Divide coef (coef[0]=1 leading, size d+1) by (x-r); on success fill
// quot (size d) with the quotient's coefficients and return true.
bool tryRoot(const vector<ll>& coef, ll r, ll M, vector<ll>& quot) {
    int d = (int)coef.size() - 1;
    quot.resize(d);
    lll v = coef[0];
    if (d > 0) quot[0] = (ll)v;
    lll boundM = M;
    for (int i = 1; i <= d; i++) {
        v = v * (lll)r + (lll)coef[i];
        if (absll(r) >= 2 && absl128(v) > boundM) return false;
        if (i < d) quot[i] = (ll)v;
    }
    return v == 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;
    while (cin >> n) {
        vector<ll> coef(n + 1);
        coef[0] = 1;
        for (int i = 1; i <= n; i++) cin >> coef[i];

        vector<ll> roots;
        while ((int)coef.size() - 1 > 0) {
            ll c0 = coef.back();
            if (c0 == 0) {
                roots.push_back(0);
                coef.pop_back();
                continue;
            }
            ll a = absll(c0);
            vector<ll> divisors;
            for (ll i = 1; i * i <= a; i++) {
                if (a % i == 0) {
                    divisors.push_back(i);
                    if (i != a / i) divisors.push_back(a / i);
                }
            }
            ll M = 0;
            for (size_t i = 0; i < coef.size(); i++) M = max(M, absll(coef[i]));
            bool found = false;
            vector<ll> quot;
            for (size_t k = 0; k < divisors.size() && !found; k++) {
                for (int s = 0; s < 2 && !found; s++) {
                    ll r = (s == 0) ? divisors[k] : -divisors[k];
                    if (tryRoot(coef, r, M, quot)) {
                        roots.push_back(r);
                        coef = quot;
                        found = true;
                    }
                }
            }
            if (!found) break;
        }
        sort(roots.begin(), roots.end());
        cout << roots.size() << "\n";
        for (size_t i = 0; i < roots.size(); i++) cout << roots[i] << "\n";
    }
    return 0;
}
