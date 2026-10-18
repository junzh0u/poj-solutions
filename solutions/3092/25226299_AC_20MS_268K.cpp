// POJ 3092 - Non-divisible 2-3 Power Sums
// Model: gpt-6-astra
// Remove factors of two, then subtract the largest power of three.
// For odd n and 3^b <= n < 3^(b+1), (n-3^b)/2 < 3^b.
// Thus successive terms have strictly increasing 2-exponents and strictly
// decreasing 3-exponents: neither can divide the other. Any valid sum is
// allowed; the order and number of terms need not match the sample.
#include <cstdio>
#include <vector>
#include <utility>
using namespace std;

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc = 1; tc <= cases; ++tc) {
        int n;
        scanf("%d", &n);
        vector< pair<int, int> > terms;
        int a = 0;
        while (n > 0) {
            while (n % 2 == 0) {
                n /= 2;
                ++a;
            }
            int power = 1, b = 0;
            while (power <= n / 3) {
                power *= 3;
                ++b;
            }
            terms.push_back(make_pair(a, b));
            n -= power;
        }
        printf("%d %d", tc, static_cast<int>(terms.size()));
        for (unsigned i = 0; i < terms.size(); ++i)
            printf(" [%d,%d]", terms[i].first, terms[i].second);
        printf("\n");
    }
    return 0;
}
