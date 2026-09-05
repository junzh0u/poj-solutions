// POJ 1662 - CoIns
// Model: claude-sonnet-5
//
// Process: a stack of M coins, all heads up. Repeatedly, for k = 1, 2, ..., M
// (then wrapping back to k=1), take the top k coins as a block, flip the
// whole block over (this both reverses their internal order and inverts
// every face in it) and put it back on top. Count operations until every
// coin again shows heads (position order does not matter, only the face
// shown at each position, since the coins are indistinguishable).
//
// The statement is a literal translation of the classic "Penny Flipping"
// puzzle (OEIS A089645); the discuss board's early threads (message ids
// 4178/4186/4192/4223/4283) argue over exactly this "flip == reverse the
// block and invert every coin in it" reading, which is what makes odd
// answer counts (11 for M=4) possible. Verified locally with a boolean
// array simulation (reverse+negate a prefix each step) against all four
// samples and against the first ~300 terms of A089645, plus the closed
// form below, before trusting the O(sqrt(N)) formula for large M.
//
// Let N = 2*M+1. Let ord = multiplicative order of 2 mod N (2 and N are
// coprime since N is odd), and let sub = the least k>0 with 2^k == +-1
// (mod N) (the "suborder"; sub == ord or sub == ord/2). Empirically
// (matches OEIS A089645's stated formula, corrected against the actual
// sequence values -- the else branch uses the *suborder*, not the order,
// which the OEIS %F line's literal wording does not make clear):
//   M == 1            -> answer = 2   (special case)
//   ord == sub         -> answer = M * ord
//   ord != sub (ord=2*sub) -> answer = M * sub - 1
// ord and sub are computed by factoring N (trial division, N < 2*10^8+1,
// so sqrt(N) < 15000), computing phi(N) from that factorization, then
// factoring phi(N) the same way and repeatedly dividing out prime factors
// p of phi(N) while 2^(e/p) == 1 (mod N) -- the standard O(sqrt(N)) way to
// get the exact multiplicative order without an O(N) doubling loop, which
// the discuss board (message 4300 "still TLE with O(N)") reports as too
// slow, and which board message 7303 reports as extra slow once __int64
// enters a tight per-iteration loop. The magnitude (M * ord can reach
// ~2*10^16) needs 64-bit arithmetic throughout, so this is submitted as
// G++ rather than POJ's old-MSVC C++ (no long long there); using
// iostream sidesteps the %lld/%I64d question entirely.

#include <iostream>
using namespace std;

typedef long long ll;

ll modpow(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// factor n into (prime, exponent) pairs, trial division up to sqrt(n)
int factorize(ll n, ll primes[], int exps[]) {
    int cnt = 0;
    for (ll d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) { n /= d; ++e; }
            primes[cnt] = d;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

ll solve(ll M) {
    if (M == 1) return 2;
    ll N = 2 * M + 1;

    ll primesN[64];
    int expsN[64];
    int cntN = factorize(N, primesN, expsN);

    ll phi = N;
    for (int i = 0; i < cntN; ++i) {
        ll p = primesN[i];
        phi = phi / p * (p - 1);
    }

    ll primesPhi[64];
    int expsPhi[64];
    int cntPhi = factorize(phi, primesPhi, expsPhi);

    ll order = phi;
    for (int i = 0; i < cntPhi; ++i) {
        ll p = primesPhi[i];
        while (order % p == 0 && modpow(2, order / p, N) == 1) {
            order /= p;
        }
    }

    ll sub;
    if (order % 2 == 1) {
        sub = order;
    } else if (modpow(2, order / 2, N) == N - 1) {
        sub = order / 2;
    } else {
        sub = order;
    }

    if (order == sub) {
        return M * order;
    } else {
        return M * sub - 1;
    }
}

int main() {
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        ll M;
        cin >> M;
        cout << solve(M) << "\n";
    }
    return 0;
}
