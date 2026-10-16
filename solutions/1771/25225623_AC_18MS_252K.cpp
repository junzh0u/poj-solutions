// POJ 1771 - Elevator Stopping Plan
// Model: gpt-6-astra
// Binary search the finishing time. Each stop covers an interval of floors;
// place it as high as possible while serving the lowest uncovered request.
// Stops need not be requested floors. Walking from floor 1 is also allowed.
#include <cstdio>
#include <vector>
using namespace std;
int n, f[32];
vector<int> stops;
bool feasible(int limit) {
    stops.clear();
    int next = 0, last = 1;
    while (next < n && 20 * (f[next] - 1) <= limit) ++next;
    while (next < n) {
        int best = -1;
        for (int s = last + 1; s <= 31; ++s) {
            int arrival = 4 * (s - 1) + 10 * (int)stops.size();
            int d = s - f[next];
            if (d < 0) d = -d;
            if (arrival + 20 * d <= limit) best = s;
        }
        if (best < 0) return false;
        int arrival = 4 * (best - 1) + 10 * (int)stops.size();
        int high = best + (limit - arrival) / 20;
        int end = next;
        while (end < n && f[end] <= high) ++end;
        // Prefer the lowest stop with the same covered request prefix.
        for (int s = last + 1; s < best; ++s) {
            int a = 4 * (s - 1) + 10 * (int)stops.size();
            int d1 = s - f[next], d2 = s - f[end - 1];
            if (d1 < 0) d1 = -d1;
            if (d2 < 0) d2 = -d2;
            if (a + 20 * d1 <= limit && a + 20 * d2 <= limit) {
                best = s;
                break;
            }
        }
        stops.push_back(best);
        last = best;
        while (next < n && f[next] <= high) ++next;
    }
    return true;
}
int main() {
    while (scanf("%d", &n) == 1 && n) {
        for (int i = 0; i < n; ++i) scanf("%d", &f[i]);
        int low = 0, high = 410;
        while (low < high) {
            int mid = (low + high) / 2;
            if (feasible(mid)) high = mid;
            else low = mid + 1;
        }
        feasible(low);
        printf("%d\n%d", low, (int)stops.size());
        for (int i = 0; i < (int)stops.size(); ++i) printf(" %d", stops[i]);
        printf("\n");
    }
    return 0;
}
