// POJ 3779 - Single CPU, multi-tasking
// Model: gpt-6.1-sol
// A task completes on its ceil(duration)-th visit. Sum each task's
// CPU use up to that visit, including this round only through its index.
// Exact integer durations finish in that round, without an extra visit.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
int main() {
    int t, n;
    if (scanf("%d", &t) != 1) return 0;
    for (int cs = 1; cs <= t; ++cs) {
        scanf("%d", &n);
        double a[100];
        for (int i = 0; i < n; ++i) scanf("%lf", &a[i]);
        printf("Case %d:\n", cs);
        for (int i = 0; i < n; ++i) {
            double round = ceil(a[i]), finish = 0;
            for (int j = 0; j < n; ++j)
                finish += min(a[j], round - (j > i ? 1 : 0));
            printf("%.2f\n", finish);
        }
    }
    return 0;
}
