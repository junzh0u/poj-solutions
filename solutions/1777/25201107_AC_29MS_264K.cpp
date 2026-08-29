// POJ 1777 - Vivian's Problem
// Model: claude-sonnet-5
//
// N = prod p_i^{e_i} (0<=e_i<=10, at least one e_i>=1). M = sigma(N) (sum of
// divisors of N). We want to know if some choice of exponents makes M a
// power of two, and if so the largest such exponent x (M = 2^x).
//
// sigma is multiplicative over the *actual* prime factorization of N: if
// N = prod q_j^{f_j} over distinct primes q_j, then sigma(N) = prod
// sigma(q_j^{f_j}), and these factors are pairwise coprime, so the product
// is a power of two iff every single factor sigma(q_j^{f_j}) is itself a
// power of two (an odd prime factor in any one factor survives the whole
// product, since multiplication of positive integers can't cancel it).
//
// For a fixed odd prime q, sigma(q^f) = 1+q+...+q^f. A short argument (used
// to derive this solution, and double checked against community-posted test
// data below) shows this can only be a power of two when f = 1 and q+1 is
// itself a power of two:
//   - if f is even, the sum has an odd number of odd terms, so it's odd;
//     being a power of two forces it to equal 1, impossible for q>=3,f>=2.
//   - if f+1 has an odd prime factor d, then sigma(q^{(f+1)/d - 1}) is a
//     factor of sigma(q^f) with an even exponent argument (>=2), so by the
//     previous point it can't be a power of two either -> f+1 must be a
//     pure power of two.
//   - if f+1 = 2^c with c>=2, splitting q^{f+1}-1 = (q^{s}-1)(q^{s}+1) with
//     s=(f+1)/2 (s even since c>=2) shows q^s+1 = 2 mod 8 (because any odd
//     square is 1 mod 8), so v2(q^s+1)=1, forcing q^s+1=2, impossible.
//   So f+1 must equal 2 exactly, i.e. f=1, and then sigma(q)=q+1=2^a.
//
// So for the prime factor q of N to be usable, its *total* exponent across
// every p_i that contains it must be exactly 1, and q must be one of the
// primes with q+1 a power of two, i.e. a Mersenne prime 2^a-1. Since
// p_i < 2^31, the only candidates are a in {2,3,5,7,13,17,19,31} (the
// Mersenne prime exponents <= 31), giving q in
// {3,7,31,127,8191,131071,524287,2147483647}.
//
// If prime 2 itself divides any used p_i, its exponent in N is >=1 and
// sigma(2^f) = 2^{f+1}-1 is always odd (>1), never a power of two, so any
// even p_i can never be used at all.
//
// A given p_i can only ever be used with e_i = 1 (any larger e_i multiplies
// every one of its prime multiplicities, forcing some exponent >= 2), and
// only if p_i is squarefree and every prime factor of p_i is one of the 8
// good primes above (any other factor, or a repeated one, means an
// unreachable/invalid total exponent). Using such a p_i claims all of its
// prime factors; two chosen p_i's must not share a prime factor, or that
// prime's total exponent would become >= 2.
//
// This reduces to: build, for every p_i, the bitmask (over the 8 good
// primes) of which ones divide it and the corresponding weight (sum of the
// a-exponents), when p_i factors exactly into a subset of them. Then run a
// 0/1 "set packing" DP over the 8-bit mask space (256 states) picking a
// maximum-weight collection of pairwise-disjoint items. If no p_i is usable
// at all, answer is NO; otherwise the DP's best (always > 0, since every
// usable item has positive weight) is the answer.
//
// This was checked against the sample and against five independent data
// sets posted on the discuss board (message ids 112793 and 116697,
// including a case where two big inputs share prime 3 and only the larger
// of the two conflicting options should be kept) -- all matched exactly.

#include <cstdio>
#include <algorithm>
using namespace std;

static const long long GOOD[8] = {3LL,7LL,31LL,127LL,8191LL,131071LL,524287LL,2147483647LL};
static const int AVAL[8] = {2,3,5,7,13,17,19,31};

int main(){
    int k;
    while (scanf("%d", &k) == 1) {
        int masks[105];
        int weights[105];
        int cnt = 0;
        for (int i = 0; i < k; ++i) {
            long long p;
            scanf("%lld", &p);
            long long rem = p;
            int mask = 0;
            int w = 0;
            for (int j = 0; j < 8; ++j) {
                if (rem % GOOD[j] == 0) {
                    rem /= GOOD[j];
                    mask |= (1 << j);
                    w += AVAL[j];
                }
            }
            if (rem == 1 && mask != 0) {
                masks[cnt] = mask;
                weights[cnt] = w;
                ++cnt;
            }
        }

        if (cnt == 0) {
            printf("NO\n");
            continue;
        }

        static int dp[256];
        for (int m = 0; m < 256; ++m) dp[m] = -1;
        dp[0] = 0;
        for (int i = 0; i < cnt; ++i) {
            static int ndp[256];
            for (int m = 0; m < 256; ++m) ndp[m] = dp[m];
            int mi = masks[i], wi = weights[i];
            for (int s = 0; s < 256; ++s) {
                if (dp[s] < 0) continue;
                if (s & mi) continue;
                int ns = s | mi;
                int val = dp[s] + wi;
                if (val > ndp[ns]) ndp[ns] = val;
            }
            for (int m = 0; m < 256; ++m) dp[m] = ndp[m];
        }

        int best = 0;
        for (int m = 0; m < 256; ++m) best = max(best, dp[m]);
        printf("%d\n", best);
    }
    return 0;
}
