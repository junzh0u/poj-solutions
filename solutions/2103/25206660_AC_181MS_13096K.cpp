// POJ 2103 - Jackpot
// Model: claude-sonnet-5
// Approach: the winning probability is the classic inclusion-exclusion sum
// over nonempty subsets S of the n lucky numbers: sum (-1)^(|S|+1)/lcm(S).
// Let D = lcm(p_1..p_n); every lcm(S) divides D (up to ~150 decimal digits
// when the p_i are pairwise coprime), so multiply through by D and work
// with the exact integers term(S) = D / lcm(S). Both lcm(S) and term(S) are
// built incrementally over the standard "drop the lowest set bit" subset
// recurrence in O(2^n): dp(S) = lcm(dp(S \ {i}), p_i), and crucially
// term(S) = term(S \ {i}) / (p_i / gcd(dp(S\{i}), p_i)) -- a division of a
// bignum by a SMALL integer (<=1e9), never a full bignum/bignum division.
// The discuss board (message 10368 et al.) confirms avoiding bignum/bignum
// division is exactly the intended trick to stay inside the time limit.
// A single bignum/bignum long division is used only once at the very end,
// to reduce the final fraction by gcd(numerator, D). No statement
// ambiguity: single test case, n<=16 lucky numbers on one line after n.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef vector<int> Big; // base BASE, little endian, no leading zero limbs (except value 0 itself)
const ll BASE = 1000000; // 10^6

void trimBig(Big &a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
    if (a.empty()) a.push_back(0);
}

Big fromLL(ll x) {
    Big r;
    if (x == 0) { r.push_back(0); return r; }
    while (x > 0) { r.push_back((int)(x % BASE)); x /= BASE; }
    return r;
}

bool isZeroBig(const Big &a) { return a.size() == 1 && a[0] == 0; }

int cmpBig(const Big &a, const Big &b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = (int)a.size() - 1; i >= 0; i--)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

Big addBig(const Big &a, const Big &b) {
    Big r;
    size_t n = max(a.size(), b.size());
    ll carry = 0;
    for (size_t i = 0; i < n || carry; i++) {
        ll cur = carry;
        if (i < a.size()) cur += a[i];
        if (i < b.size()) cur += b[i];
        r.push_back((int)(cur % BASE));
        carry = cur / BASE;
    }
    if (r.empty()) r.push_back(0);
    trimBig(r);
    return r;
}

// a - b, assumes a >= b
Big subBig(const Big &a, const Big &b) {
    Big r;
    ll borrow = 0;
    for (size_t i = 0; i < a.size(); i++) {
        ll cur = a[i] - borrow - (i < b.size() ? b[i] : 0);
        if (cur < 0) { cur += BASE; borrow = 1; } else borrow = 0;
        r.push_back((int)cur);
    }
    trimBig(r);
    return r;
}

// multiply bignum by a small non-negative integer (fits comfortably, <= ~1e9)
Big mulSmall(const Big &a, ll m) {
    Big r;
    if (m == 0) { r.push_back(0); return r; }
    ll carry = 0;
    for (size_t i = 0; i < a.size() || carry; i++) {
        ll cur = carry;
        if (i < a.size()) cur += (ll)a[i] * m;
        r.push_back((int)(cur % BASE));
        carry = cur / BASE;
    }
    trimBig(r);
    return r;
}

// divide bignum by a small positive integer (<= ~1e9); sets remainder
Big divSmall(const Big &a, ll m, ll &rem) {
    Big r(a.size());
    ll cur = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        cur = cur * BASE + a[i];
        r[i] = (int)(cur / m);
        cur %= m;
    }
    trimBig(r);
    rem = cur;
    return r;
}

ll modSmall(const Big &a, ll m) {
    ll cur = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
        cur = (cur * BASE + a[i]) % m;
    return cur;
}

ll gcdLL(ll a, ll b) { while (b) { ll t = a % b; a = b; b = t; } return a; }

// full bignum long division (only used a handful of times, for the final reduction)
Big divmodBig(const Big &a, const Big &b, Big &rem) {
    Big quotient(a.size(), 0);
    Big cur; cur.push_back(0);
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        cur.insert(cur.begin(), a[i]);
        trimBig(cur);
        ll lo = 0, hi = BASE - 1, d = 0;
        while (lo <= hi) {
            ll mid = (lo + hi) / 2;
            Big t = mulSmall(b, mid);
            if (cmpBig(t, cur) <= 0) { d = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        quotient[i] = (int)d;
        cur = subBig(cur, mulSmall(b, d));
    }
    trimBig(quotient);
    rem = cur;
    return quotient;
}

Big gcdBig(Big a, Big b) {
    while (!isZeroBig(b)) {
        Big rem;
        divmodBig(a, b, rem);
        a = b;
        b = rem;
    }
    return a;
}

int n;
ll p[16];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        p[i] = x;
    }

    // D = lcm of all p_i, built via small-int gcd/multiply only
    Big D = fromLL(1);
    for (int i = 0; i < n; i++) {
        ll r = modSmall(D, p[i]);
        ll g = gcdLL(r, p[i]);
        ll mulf = p[i] / g;
        D = mulSmall(D, mulf);
    }

    int full = 1 << n;
    vector<Big> dp(full), term(full);
    dp[0] = fromLL(1);
    term[0] = D; // D / lcm(empty set) = D / 1

    Big posSum = fromLL(0), negSum = fromLL(0);

    for (int mask = 1; mask < full; mask++) {
        int i = 0;
        while (!(mask & (1 << i))) i++;
        int mask2 = mask ^ (1 << i);
        ll r = modSmall(dp[mask2], p[i]);
        ll g = gcdLL(r, p[i]);
        ll mulf = p[i] / g; // = lcm(dp[mask2],p_i)/dp[mask2], also the divisor for term
        dp[mask] = mulSmall(dp[mask2], mulf);
        ll rem;
        term[mask] = divSmall(term[mask2], mulf, rem);
        // rem is mathematically guaranteed to be 0

        int pc = __builtin_popcount((unsigned)mask);
        if (pc & 1) posSum = addBig(posSum, term[mask]);
        else negSum = addBig(negSum, term[mask]);
    }

    Big numerator = subBig(posSum, negSum); // posSum >= negSum since the true value lies in [0,1]

    Big g = gcdBig(numerator, D);
    Big rem;
    Big finalNum = divmodBig(numerator, g, rem);
    Big finalDen = divmodBig(D, g, rem);

    printf("%d", finalNum.back());
    for (int i = (int)finalNum.size() - 2; i >= 0; i--) printf("%06d", finalNum[i]);
    printf("\n");
    printf("%d", finalDen.back());
    for (int i = (int)finalDen.size() - 2; i >= 0; i--) printf("%06d", finalDen[i]);
    printf("\n");

    return 0;
}
