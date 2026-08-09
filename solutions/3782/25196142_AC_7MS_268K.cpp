// POJ 3782 - Equal Sum Partitions
// Model: gpt-5.6-terra
// Test the possible first-group sums in increasing prefix order; a candidate
// works exactly when a left-to-right scan reaches it at every group boundary.

#include <cstdio>
#include <vector>

int main() {
    int datasets;
    if (std::scanf("%d", &datasets) != 1) return 0;
    while (datasets--) {
        int id, m;
        std::scanf("%d%d", &id, &m);
        std::vector<long long> values(m);
        long long total = 0;
        for (int i = 0; i < m; ++i) {
            std::scanf("%lld", &values[i]);
            total += values[i];
        }
        long long prefix = 0;
        long long answer = total;
        for (int first_end = 0; first_end < m; ++first_end) {
            prefix += values[first_end];
            if (total % prefix != 0) continue;
            long long group = 0;
            bool valid = true;
            for (int i = first_end + 1; i < m; ++i) {
                group += values[i];
                if (group == prefix) group = 0;
                else if (group > prefix) {
                    valid = false;
                    break;
                }
            }
            if (valid && group == 0) {
                answer = prefix;
                break;
            }
        }
        std::printf("%d %lld\n", id, answer);
    }
    return 0;
}
