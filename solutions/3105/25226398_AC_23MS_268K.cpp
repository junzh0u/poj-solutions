// POJ 3105 - Expectation
// Model: gpt-6-astra
// Count ones in each bit among [0,n). Independence gives XOR probability
// 2*p*(1-p); linearity of expectation sums the weighted bit probabilities.
// Both generators use the same n. All integer counts and doubled bit weights
// fit signed 32 bits for n <= 10^9; convert to double before products.
#include <cstdio>
#include <algorithm>
int main() {
    int cases, n;
    if (std::scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        std::scanf("%d", &n);
        double answer = 0.0;
        for (int bit = 1; bit < n; bit *= 2) {
            int period = 2 * bit;
            int ones = (n / period) * bit + std::max(0, n % period - bit);
            double p = double(ones) / n;
            answer += bit * 2.0 * p * (1.0 - p);
        }
        std::printf("%.2f\n", answer);
    }
    return 0;
}
