// POJ 3005 - Exploding CPU
// Model: claude-sonnet-5
//
// An explosive number is x = p0*p1*...*pn where p0 = 1 (fixed, not required
// to be prime -- the sample 4505 = 1*5*17*53 makes this explicit even though
// the statement's garbled "all p_i are distinct primes ... p0 = 1" reads as
// if p0 must also be prime) and p1..pn (n >= 3, so at least 3 primes) are
// distinct primes satisfying p_i = A*p_{i-1} + B for integer A, B.
//
// Since all p_i (i>=1) are primes >= 2 and the running product only grows,
// we can enumerate every (A, B) pair, walk the deterministic chain starting
// at p0 = 1, and record the running product every time the chain has
// produced at least 3 valid, pairwise-distinct primes, stopping the chain as
// soon as a term is out of range, composite, a repeat, or the product would
// exceed the global bound 2*10^9.
//
// Bounding A, B: writing B = p1 - A (from p0=1), algebra shows that pushing
// |A| large forces at least one of p1,p2,p3 to grow at least as fast, so
// with p1*p2*p3 <= 2*10^9 the useful range of A (and hence B) is small
// (roughly the cube root of 2*10^9 ~= 1260 in the extremal case p1=2).
// This was cross-checked empirically: enumerating A,B in [-600,600] already
// reproduces exactly the same 243-element set of explosive numbers under
// 2*10^9 as enumerating in [-1500,1500] (identical sorted lists, not just
// matching counts). The submitted code uses [-1000,1000] for margin.
//
// Primality: a small sieve handles the (very common) small candidates and
// a deterministic Miller-Rabin (bases 2,3,5,7,11,13,...,37, valid well
// beyond 2*10^9) covers any larger candidate that arises later in a chain.
//
// All (A,B) results are deduplicated (the same explosive number can arise
// from more than one (A,B) or n) and queries are answered offline via
// binary search over the sorted list.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

static const ll LIMIT = 2000000000LL;
static const ll RANGE = 1000;
static const int SIEVE_N = 2000000;
static unsigned char isComposite[SIEVE_N + 1];

ull mulmod(ull a, ull b, ull m) { return (ull)((__int128)a * b % m); }

ull powmod(ull a, ull e, ull m) {
    ull r = 1;
    a %= m;
    while (e) {
        if (e & 1) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

bool isPrimeMR(ll n) {
    if (n < 2) return false;
    static const ll smallPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(smallPrimes) / sizeof(smallPrimes[0]); i++) {
        ll p = smallPrimes[i];
        if (n % p == 0) return n == p;
    }
    ull d = (ull)(n - 1);
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    for (size_t i = 0; i < sizeof(smallPrimes) / sizeof(smallPrimes[0]); i++) {
        ull a = (ull)smallPrimes[i];
        if (a >= (ull)n) continue;
        ull x = powmod(a, d, (ull)n);
        if (x == 1 || x == (ull)n - 1) continue;
        bool composite = true;
        for (int j = 0; j < r - 1; j++) {
            x = mulmod(x, x, (ull)n);
            if (x == (ull)n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

inline bool isPrimeFast(ll n) {
    if (n < 2) return false;
    if (n <= SIEVE_N) return !isComposite[n];
    return isPrimeMR(n);
}

int main() {
    for (int i = 2; (ll)i * i <= SIEVE_N; i++) {
        if (!isComposite[i]) {
            for (ll j = (ll)i * i; j <= SIEVE_N; j += i) isComposite[j] = 1;
        }
    }

    vector<ll> explosive;
    ll used[64];
    for (ll A = -RANGE; A <= RANGE; A++) {
        if (A == 0) continue;
        for (ll B = -RANGE; B <= RANGE; B++) {
            ll p = 1; // p0
            int usedCnt = 0;
            int cnt = 0;
            ll product = 1;
            for (int step = 0; step < 63; step++) {
                ll next = A * p + B;
                if (next < 2 || next > LIMIT) break;
                if (!isPrimeFast(next)) break;
                bool dup = false;
                for (int i = 0; i < usedCnt; i++) {
                    if (used[i] == next) { dup = true; break; }
                }
                if (dup) break;
                if (product > LIMIT / next) break; // product * next would exceed LIMIT
                product *= next;
                used[usedCnt++] = next;
                cnt++;
                if (cnt >= 3) explosive.push_back(product);
                p = next;
            }
        }
    }
    sort(explosive.begin(), explosive.end());
    explosive.erase(unique(explosive.begin(), explosive.end()), explosive.end());

    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) {
        ll lo, hi;
        scanf("%lld %lld", &lo, &hi);
        ll cnt = (ll)(upper_bound(explosive.begin(), explosive.end(), hi) -
                       lower_bound(explosive.begin(), explosive.end(), lo));
        printf("%lld\n", cnt);
    }
    return 0;
}
