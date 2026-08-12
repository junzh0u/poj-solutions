// POJ 1492 - Up and Down Sequences
// Model: gpt-5.6-terra
// Classify initial equal values by the first strict change.  Thereafter each
// adjacent comparison extends the current weakly monotone run, or starts a
// run of the opposite direction; averages are measured in comparisons.
#include <cstdio>
#include <vector>

int main() {
    int first;
    while (std::scanf("%d", &first) == 1 && first != 0) {
        std::vector<int> values;
        values.push_back(first);
        int value;
        while (std::scanf("%d", &value) == 1 && value != 0)
            values.push_back(value);

        int direction = 0; // 1 for up, -1 for down
        int i;
        for (i = 1; i < (int)values.size(); ++i) {
            if (values[i] > values[i - 1]) {
                direction = 1;
                break;
            }
            if (values[i] < values[i - 1]) {
                direction = -1;
                break;
            }
        }

        int upLength = 0, downLength = 0;
        int upRuns = 0, downRuns = 0;
        if (direction != 0) {
            if (direction == 1)
                upRuns = 1;
            else
                downRuns = 1;
            for (i = 1; i < (int)values.size(); ++i) {
                int next = 0;
                if (values[i] > values[i - 1]) next = 1;
                if (values[i] < values[i - 1]) next = -1;
                if (next != 0 && next != direction) {
                    direction = next;
                    if (direction == 1)
                        ++upRuns;
                    else
                        ++downRuns;
                }
                if (direction == 1)
                    ++upLength;
                else
                    ++downLength;
            }
        }

        double upAverage = upRuns ? (double)upLength / upRuns : 0.0;
        double downAverage = downRuns ? (double)downLength / downRuns : 0.0;
        std::printf("Nr values = %d:  %.6f %.6f\n", (int)values.size(),
                    upAverage, downAverage);
    }
    return 0;
}
