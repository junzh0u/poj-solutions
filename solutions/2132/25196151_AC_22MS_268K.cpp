// POJ 2132 - Cow Math
// Model: gpt-5.6-terra
// For each prime, its answer exponent is the widest-path bottleneck of that
// exponent; multiply the resulting prime powers with a decimal big integer.
#include <cstdio>
#include <vector>

using namespace std;

class BigInteger {
public:
    BigInteger() { digit.push_back(1); }

    void multiply(int value) {
        long long carry = 0;
        int i;
        for (i = 0; i < (int)digit.size(); ++i) {
            long long product = (long long)digit[i] * value + carry;
            digit[i] = (int)(product % BASE);
            carry = product / BASE;
        }
        while (carry != 0) {
            digit.push_back((int)(carry % BASE));
            carry /= BASE;
        }
    }

    void print() const {
        int i;
        printf("%d", digit.back());
        for (i = (int)digit.size() - 2; i >= 0; --i) printf("%09d", digit[i]);
        printf("\n");
    }

private:
    static const int BASE = 1000000000;
    vector<int> digit;
};

static int exponent(int value, int prime) {
    int count = 0;
    while (value % prime == 0) {
        value /= prime;
        ++count;
    }
    return count;
}

int main() {
    int n, i, j, k, value, p;
    int edge[25][25];
    vector<int> primes;
    bool composite[2001];
    BigInteger answer;

    if (scanf("%d", &n) != 1) return 0;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            scanf("%d", &edge[i][j]);

    for (i = 0; i <= 2000; ++i) composite[i] = false;
    for (i = 2; i <= 2000; ++i) {
        if (!composite[i]) {
            primes.push_back(i);
            for (j = i + i; j <= 2000; j += i) composite[j] = true;
        }
    }

    for (p = 0; p < (int)primes.size(); ++p) {
        int best[25][25];
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) {
                if (i == j) best[i][j] = 1000;
                else if (edge[i][j] != 0) best[i][j] = exponent(edge[i][j], primes[p]);
                else best[i][j] = -1;
            }
        }
        for (k = 0; k < n; ++k)
            for (i = 0; i < n; ++i)
                for (j = 0; j < n; ++j) {
                    value = best[i][k] < best[k][j] ? best[i][k] : best[k][j];
                    if (value > best[i][j]) best[i][j] = value;
                }
        for (i = 0; i < best[0][1]; ++i) answer.multiply(primes[p]);
    }

    answer.print();
    return 0;
}
