// POJ 1527 - Making the Grade
// Model: gpt-6.1-sol
// Compute class cutoffs before bonuses, then adjust letter grades for attendance.
// Round averages, mean and standard deviation to tenths as the statement requires.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double tenth(double x) { return floor(x * 10.0 + 0.5) / 10.0; }
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    puts("MAKING THE GRADE OUTPUT");
    while (cases--) {
        int s, t, bonus[31], absent[31];
        double avg[31], mean = 0, sd = 0;
        scanf("%d%d", &s, &t);
        for (int i = 0; i < s; ++i) {
            int sum = 0, low = 101;
            for (int j = 0; j < t; ++j) {
                int x; scanf("%d", &x);
                sum += x; low = min(low, x);
            }
            if (t > 2) sum -= low;
            avg[i] = tenth(double(sum) / (t > 2 ? t - 1 : t));
            mean += avg[i];
            scanf("%d%d", &bonus[i], &absent[i]);
        }
        mean = tenth(mean / s);
        for (int i = 0; i < s; ++i) sd += (avg[i]-mean)*(avg[i]-mean);
        sd = max(1.0, tenth(sqrt(sd / s)));
        int points = 0;
        for (int i = 0; i < s; ++i) {
            double x = avg[i] + 3 * (bonus[i] / 2);
            int grade = x + 1e-8 >= mean + sd ? 4 :
                        x + 1e-8 >= mean ? 3 :
                        x + 1e-8 >= mean - sd ? 2 : 1;
            grade -= absent[i] / 4;
            if (!absent[i]) ++grade;
            points += max(0, min(4, grade));
        }
        printf("%.1f\n", double(points) / s);
    }
    puts("END OF OUTPUT");
    return 0;
}
