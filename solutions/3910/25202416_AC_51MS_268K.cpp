// POJ 3910 - GCD Determinant
// Model: claude-sonnet-5
// Approach: For a factor-closed set S = {x1,...,xn} (every divisor of every
// element is also in S), the GCD matrix (gcd(xi,xj)) has determinant equal to
// the product of Euler's totient function phi(x_i) over all elements
// (Smith's determinant theorem). This holds regardless of the order chosen
// for the x_i: relabeling elements permutes rows and the same columns
// together (A' = P A P^T), which leaves det(A) unchanged since det(P)^2=1.
// So we just compute prod(phi(x_i)) mod 1e9+7, with phi computed by trial
// division against primes up to sqrt(2*10^9) (~44721), precomputed by sieve.
// Ambiguity/board notes: the discuss board confirms the product-of-phi
// conclusion directly, and separately warns that test data is weak (a set
// with a literal duplicate, e.g. {1,1}, would truly have determinant 0 but
// isn't tested) -- not a concern since S is stated to be a set of distinct
// factor-closed elements, and this matches the accepted approach.
#include <cstdio>
#include <vector>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const ll MOD = 1000000007LL;
const int LIMIT = 44722;
bool composite[LIMIT + 1];
vector<int> primes;

void sieve() {
    for (int i = 2; i <= LIMIT; ++i) {
        if (!composite[i]) {
            primes.push_back(i);
            for (ll j = (ll)i * i; j <= LIMIT; j += i)
                composite[j] = true;
        }
    }
}

ll phi(ll x) {
    ll result = x;
    ll n = x;
    for (size_t i = 0; i < primes.size(); ++i) {
        ll p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            result -= result / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

int main() {
    sieve();
    int n;
    while (scanf("%d", &n) == 1) {
        ll ans = 1;
        for (int i = 0; i < n; ++i) {
            ll x;
            scanf("%lld", &x);
            ll ph = phi(x) % MOD;
            ans = (ans * ph) % MOD;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
