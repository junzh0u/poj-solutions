// POJ 1212 - Eeny Meeny
// Model: gpt-6.1-sol
// Precompute the zero-based Josephus survivor for step 15. A distance d
// is unsafe if either d or n-d survives for any possible population n.
// Mxgobgwq occupies position zero; distances start at one, and cannot
// exceed half the smallest possible circle. Accept either endpoint order.
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int survivor[501];
    survivor[1] = 0;
    for (int n = 2; n <= 500; ++n)
        survivor[n] = (survivor[n-1] + 15) % n;
    int low, high;
    while (scanf("%d%d", &low, &high) == 2 && (low || high)) {
        if (low > high) swap(low, high);
        int answer = 0;
        for (int d = 1; d <= low/2; ++d) {
            bool safe = true;
            for (int n = low; n <= high; ++n)
                if (survivor[n] == d || survivor[n] == n-d) {
                    safe = false;
                    break;
                }
            if (safe) { answer = d; break; }
        }
        if (answer) printf("%d\n", answer);
        else puts("Better estimate needed");
    }
    return 0;
}
