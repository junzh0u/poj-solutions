// POJ 4043 - Remoteland
// Model: gpt-6.1-sol
// The largest square product is n! with each odd-exponent prime removed.
// Start with the product of composites, then restore primes whose factorial
// exponent is even. The original SWERC statement supplies the missing POJ text.
#include <cstdio>
#include <vector>
using namespace std;
const int MOD = 1000000007;
int main() {
    vector<int> queries;
    int n, limit = 1;
    while (scanf("%d", &n) == 1 && n) {
        queries.push_back(n);
        if (n > limit) limit = n;
    }
    vector<bool> composite(limit + 1, false);
    for (int i = 2; i <= limit / i; ++i)
        if (!composite[i])
            for (int j = i * i; j <= limit; j += i) composite[j] = true;
    vector<int> primes;
    vector<int> product(limit + 1, 1);
    for (int i = 2; i <= limit; ++i) {
        product[i] = product[i - 1];
        if (composite[i]) product[i] = (long long)product[i] * i % MOD;
        else primes.push_back(i);
    }
    for (size_t k = 0; k < queries.size(); ++k) {
        n = queries[k];
        long long answer = product[n];
        for (size_t i = 0; i < primes.size() && primes[i] <= n / 2; ++i) {
            int p = primes[i], t = n, parity = 0;
            while (t) { t /= p; parity ^= (t & 1); }
            if (!parity) answer = answer * p % MOD;
        }
        printf("%d\n", (int)answer);
    }
    return 0;
}
