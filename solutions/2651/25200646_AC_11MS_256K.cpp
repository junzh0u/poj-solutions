// POJ 2651 - So you want to be a 2^n-aire?
// Model: gpt-5.6-terra
// Let value[r] be the expected final prize divided by the current prize when
// r questions remain.  After observing p, quitting is worth 1 and answering
// is worth 2*p*value[r-1], so integrate their maximum over p in [t, 1].
// The threshold 1/(2*value[r-1]) splits the two choices exactly.
#include <cstdio>
#include <iomanip>
#include <iostream>

int main() {
    int n;
    double t;
    while (std::cin >> n >> t && n != 0) {
        double value = 1.0;
        int r;
        for (r = 1; r <= n; ++r) {
            if (t == 1.0) {
                value *= 2.0;
            } else {
                double threshold = 1.0 / (2.0 * value);
                if (threshold <= t) {
                    value *= (1.0 + t);
                } else {
                    value = ((threshold - t) + value * (1.0 - threshold * threshold)) / (1.0 - t);
                }
            }
        }
        std::cout << std::fixed << std::setprecision(3) << value + 1e-10 << '\n';
    }
    return 0;
}
