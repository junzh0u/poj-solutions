// POJ 3231 - FlashGet
// Model: claude-sonnet-5
//
// Simulate downloads with max-min fair bandwidth redistribution.
// Each task has (remaining size, current speed, max speed). Speeds stay
// constant until the next task finishes. At each event we jump to the
// earliest finish time among active tasks, subtract progress from every
// active task, finish all tasks whose remaining size hits zero (there can
// be ties), then redistribute the freed bandwidth (sum of the speeds of the
// tasks that just finished) among the remaining active tasks that are still
// below their max speed, using classic water-filling: split the freed
// amount equally among the still-eligible tasks, cap any task that would
// exceed its max at its max (removing it from the eligible set and folding
// its unused share back into the pool), and repeat until the pool is spent
// or no eligible task remains (any bandwidth that cannot be absorbed
// because every remaining task is already at its cap is simply left
// unused, matching the sample: task2 finishing frees 30 while task3 is
// already at its own max of 30, so that bandwidth is never used).
//
// Ambiguity: the statement never states numeric bounds beyond n<=100, and
// the discuss board (message 178593 and the "C++/G++" precision threads)
// documents heavy floating point sensitivity across compilers/eps choices
// but no algorithmic disagreement; several posted snippets implement the
// same water-filling rule. Verified against the official sample by hand
// tracing (task1 finishes at 5.000, frees 20 which fully saturates task3 to
// its max of 30 with 5 left over unused since task2 is already at max;
// task2 finishes at 6.667, frees 30 but task3 is already at its max so it
// stays unused; task3 finishes at 12.500) which matches exactly.
#include <cstdio>
#include <cmath>
using namespace std;

const double EPS = 1e-6;
const int MAXN = 105;

int main() {
    int n;
    int caseNo = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        double t;
        scanf("%lf", &t);
        double size[MAXN], speed[MAXN], maxspd[MAXN];
        bool finished[MAXN];
        double finishTime[MAXN];
        for (int i = 0; i < n; i++) {
            scanf("%lf %lf %lf", &size[i], &speed[i], &maxspd[i]);
            finished[i] = false;
            finishTime[i] = 0.0;
        }
        double curTime = 0.0;
        int remaining = n;
        while (remaining > 0) {
            double mindt = -1.0;
            for (int i = 0; i < n; i++) {
                if (!finished[i]) {
                    double dt = size[i] / speed[i];
                    if (mindt < 0 || dt < mindt) mindt = dt;
                }
            }
            if (mindt < 0) break; // safety: no active task can progress
            curTime += mindt;
            for (int i = 0; i < n; i++) {
                if (!finished[i]) {
                    size[i] -= speed[i] * mindt;
                }
            }
            double freed = 0.0;
            for (int i = 0; i < n; i++) {
                if (!finished[i] && size[i] <= EPS) {
                    finished[i] = true;
                    finishTime[i] = curTime;
                    freed += speed[i];
                    remaining--;
                }
            }
            if (remaining > 0 && freed > EPS) {
                int idx[MAXN], cnt = 0;
                for (int i = 0; i < n; i++)
                    if (!finished[i]) idx[cnt++] = i;
                bool eligible[MAXN];
                for (int k = 0; k < cnt; k++)
                    eligible[k] = (speed[idx[k]] < maxspd[idx[k]] - EPS);
                double rem = freed;
                while (rem > EPS) {
                    int ecount = 0;
                    for (int k = 0; k < cnt; k++)
                        if (eligible[k]) ecount++;
                    if (ecount == 0) break;
                    double share = rem / ecount;
                    double used = 0.0;
                    bool anycap = false;
                    for (int k = 0; k < cnt; k++) {
                        if (!eligible[k]) continue;
                        int i = idx[k];
                        if (speed[i] + share >= maxspd[i] - EPS) {
                            used += (maxspd[i] - speed[i]);
                            speed[i] = maxspd[i];
                            eligible[k] = false;
                            anycap = true;
                        }
                    }
                    if (!anycap) {
                        for (int k = 0; k < cnt; k++)
                            if (eligible[k]) speed[idx[k]] += share;
                        rem = 0.0;
                        break;
                    } else {
                        rem -= used;
                    }
                }
            }
        }
        caseNo++;
        printf("Case %d:\n", caseNo);
        for (int i = 0; i < n; i++) {
            printf("NO%d:%.3fs\n", i + 1, finishTime[i]);
        }
    }
    return 0;
}
