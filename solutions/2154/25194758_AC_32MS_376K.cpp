// POJ 2154 - Color
// Model: gpt-5.6-terra
// Burnside's lemma groups rotations by their order: the number of necklaces is
// sum_{d | N} phi(d) * N^(N/d-1).  This form avoids division modulo P.
// P is not necessarily prime (and may be 1), so every power is reduced directly.
// Only rotations are identified; reflections remain distinct as stated.
#include <cstdio>
#include <map>
#include <utility>
#include <vector>

using namespace std;

static vector<int> primes;
static int nValue;
static int modValue;
static int answer;

static int powerMod(int base, int exponent) {
    int result = 1 % modValue;
    base %= modValue;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % modValue;
        base = base * base % modValue;
        exponent >>= 1;
    }
    return result;
}

static void addDivisors(const vector<pair<int, int> >& factors, int at, int divisor, int phi) {
    if (at == (int)factors.size()) {
        int term = (phi % modValue) * powerMod(nValue, nValue / divisor - 1) % modValue;
        answer += term;
        if (answer >= modValue) answer -= modValue;
        return;
    }

    addDivisors(factors, at + 1, divisor, phi);
    int prime = factors[at].first;
    int value = 1;
    int phiPower = prime - 1;
    int exponent;
    for (exponent = 1; exponent <= factors[at].second; ++exponent) {
        value *= prime;
        addDivisors(factors, at + 1, divisor * value, phi * phiPower);
        phiPower *= prime;
    }
}

static int solve(int n, int p) {
    nValue = n;
    modValue = p;
    answer = 0;

    vector<pair<int, int> > factors;
    int left = n;
    int i;
    for (i = 0; i < (int)primes.size() && primes[i] * primes[i] <= left; ++i) {
        if (left % primes[i] == 0) {
            int count = 0;
            do {
                left /= primes[i];
                ++count;
            } while (left % primes[i] == 0);
            factors.push_back(make_pair(primes[i], count));
        }
    }
    if (left > 1) factors.push_back(make_pair(left, 1));
    addDivisors(factors, 0, 1, 1);
    return answer;
}

int main() {
    const int limit = 31623;
    vector<bool> composite(limit + 1, false);
    int i;
    for (i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            primes.push_back(i);
            if (i * i <= limit) {
                int j;
                for (j = i * i; j <= limit; j += i) composite[j] = true;
            }
        }
    }

    int tests;
    scanf("%d", &tests);
    map<pair<int, int>, int> cache;
    while (tests--) {
        int n, p;
        scanf("%d%d", &n, &p);
        pair<int, int> key = make_pair(n, p);
        map<pair<int, int>, int>::iterator found = cache.find(key);
        if (found == cache.end()) {
            int value = solve(n, p);
            cache[key] = value;
            printf("%d\n", value);
        } else {
            printf("%d\n", found->second);
        }
    }
    return 0;
}
