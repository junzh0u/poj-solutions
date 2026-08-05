// POJ 2689 - Prime Distance
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static const int LIMIT = 46341; // > sqrt(2^31-1)

int main() {
    // sieve base primes up to LIMIT
    vector<bool> comp(LIMIT + 1, false);
    vector<int> primes;
    for (int i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= LIMIT; j += i)
                comp[(int)j] = true;
        }
    }

    long long L, U;
    while (scanf("%lld %lld", &L, &U) == 2) {
        long long len = U - L + 1;
        vector<bool> segComp(len, false);
        if (L == 1) segComp[0] = true; // 1 is not prime

        for (size_t k = 0; k < primes.size(); ++k) {
            long long p = primes[k];
            if (p * p > U) break;
            long long start = ((L + p - 1) / p) * p;
            if (start < p * p) start = p * p;
            for (long long j = start; j <= U; j += p) {
                segComp[j - L] = true;
            }
        }

        // collect primes in range
        long long c1 = -1, c2 = -1, d1 = -1, d2 = -1;
        long long minDist = -1, maxDist = -1;
        long long prev = -1;
        for (long long v = L; v <= U; ++v) {
            if (!segComp[v - L]) {
                if (prev != -1) {
                    long long dist = v - prev;
                    if (minDist == -1 || dist < minDist) {
                        minDist = dist;
                        c1 = prev; c2 = v;
                    }
                    if (maxDist == -1 || dist > maxDist) {
                        maxDist = dist;
                        d1 = prev; d2 = v;
                    }
                }
                prev = v;
            }
        }

        if (minDist == -1) {
            printf("There are no adjacent primes.\n");
        } else {
            printf("%lld,%lld are closest, %lld,%lld are most distant.\n", c1, c2, d1, d2);
        }
    }

    return 0;
}
