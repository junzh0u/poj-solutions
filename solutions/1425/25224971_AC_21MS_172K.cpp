// POJ 1425 - Enigmatic Travel
// Model: gpt-6.1-sol
// Count journeys by length from one fixed starting vertex; symmetry cancels
// the number of starts. Walks allow repeated vertices and edges, simple paths
// do not, and simple cycles have at least three edges. Different starts on
// the same cycle are distinct journeys, as clarified by the discussion board.
#include <cstdio>
int main() {
    int n;
    while (std::scanf("%d", &n) == 1 && n) {
        double count = 1, total = 0, cost = 0;
        for (int k = 1; k <= n; ++k) {
            count *= n - 1;
            total += count;
            cost += k * count;
        }
        double f1 = cost / total;
        count = 1; total = cost = 0;
        double cycleTotal = 0, cycleCost = 0;
        for (int k = 1; k < n; ++k) {
            count *= n - k;
            total += count;
            cost += k * count;
            if (k >= 2) {
                cycleTotal += count;
                cycleCost += (k + 1) * count;
            }
        }
        std::printf("%.4f %.4f %.4f\n", f1, cost / total,
                    cycleCost / cycleTotal);
    }
    return 0;
}
