// POJ 3358 - Period of an Infinite Binary Expansion
// Model: claude-opus-5
//
// Reduce p/q to lowest terms and write q = 2^a * m with m odd.  The tail of
// the binary expansion from index k is determined by the remainder
// r_k = p*2^k mod q, and two tails are equal exactly when their remainders
// are, so the least preperiod is the first k whose remainder recurs and the
// least period is the cycle length.  q | p*2^i*(2^s - 1) with gcd(p,q)=1 and
// 2^s-1 odd forces i = a, so the answer's first component is a+1; what is
// left is the least s with 2^s = 1 (mod m), i.e. the multiplicative order of
// 2 mod m.  Computed as ord = phi(m) divided down by each prime factor of
// phi(m) while 2^(ord/f) is still 1 -- ord always divides phi(m).
// m = 1 (a terminating expansion) has least period "0", length 1, which also
// gives the "1,1" the board reports for 0/5 and the "3,1" for 1/4.
//
// Board: p < q < 2e9, so q, m and the order all fit in 32 bits but products
// do not; everything is long long here (hence G++ rather than POJ's C++).
// Trial division needs primes to sqrt(2e9) only, both for phi(m) and for
// factoring phi(m).
//
// Statement note: input gives no case count and no terminator, so it is read
// to EOF; p >= 0 is allowed and p = 0 reduces to 0/1, handled by the m = 1
// branch rather than specially.

#include <iostream>
#include <vector>

using namespace std;

static const int SIEVE_LIMIT = 46400; /* > sqrt(2^31) */

static vector<int> primes;

static void buildSieve() {
    vector<char> composite(SIEVE_LIMIT + 1, 0);
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= SIEVE_LIMIT; j += i)
                composite[(size_t)j] = 1;
        }
    }
}

static long long gcdLL(long long a, long long b) {
    while (b) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static long long powMod(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

static long long eulerPhi(long long n) {
    long long phi = 1;
    for (size_t i = 0; i < primes.size(); ++i) {
        long long p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            n /= p;
            phi *= p - 1;
            while (n % p == 0) {
                n /= p;
                phi *= p;
            }
        }
    }
    if (n > 1) phi *= n - 1; /* remaining factor is prime */
    return phi;
}

/* least s > 0 with 2^s == 1 (mod m), for odd m > 1 */
static long long orderOfTwo(long long m) {
    long long order = eulerPhi(m);
    long long rest = order;
    for (size_t i = 0; i < primes.size(); ++i) {
        long long p = primes[i];
        if (p * p > rest) break;
        if (rest % p == 0) {
            while (rest % p == 0) rest /= p;
            while (order % p == 0 && powMod(2, order / p, m) == 1) order /= p;
        }
    }
    if (rest > 1) {
        long long p = rest;
        while (order % p == 0 && powMod(2, order / p, m) == 1) order /= p;
    }
    return order;
}

int main() {
    ios::sync_with_stdio(false);
    buildSieve();

    long long p, q;
    char slash;
    int caseNo = 0;
    while (cin >> p >> slash >> q) {
        long long g = gcdLL(p, q);
        long long d = q / g;
        long long a = 0;
        while (d % 2 == 0) {
            d /= 2;
            ++a;
        }
        long long period = (d == 1) ? 1 : orderOfTwo(d);
        ++caseNo;
        cout << "Case #" << caseNo << ": " << (a + 1) << "," << period << "\n";
    }
    return 0;
}
