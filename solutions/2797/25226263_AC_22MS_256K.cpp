// POJ 2797 - Guards
// Model: gpt-6-astra
// Regular schedules use equal populations in each cyclic phase, hence
// bundles of 3, 1, 4 and 5 guards. Enumerate the first and third bundles.
// For fixed counts, the fewest fourth bundles meeting nighttime demand
// is optimal: each extra bundle costs 5 and saves at most 3 weekday guards.
// The jury checker confirms that regularity is required per schedule.
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int n1, n2, n3;
    if (scanf("%d%d%d", &n1, &n2, &n3) != 3) return 0;
    int limit = max(n1, max(n2, n3));
    int best = 3 * limit, aa = limit, bb = 0, cc = 0, dd = 0;
    for (int a = 0; a <= limit; ++a) {
        for (int c = 0; c <= limit; ++c) {
            if (a + 2*c < n2) continue;
            int d = max(0, (n3-a-c+1)/2);
            int b = max(0, n1-a-2*c-3*d);
            int cost = 3*a+b+4*c+5*d;
            if (cost < best) {
                best = cost;
                aa = a; bb = b; cc = c; dd = d;
            }
        }
    }
    printf("%d %d %d %d\n", 3*aa, bb, 4*cc, 5*dd);
    return 0;
}
