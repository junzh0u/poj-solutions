// POJ 2431 - Expedition
//
// Classic greedy-with-max-heap (same idea as LeetCode "Minimum Number of
// Refueling Stops"). Stop distances are given from the town, but the truck
// drives from its own position, so first convert: distFromTruck = L - a.
// Sort stops by distance from the truck ascending and sweep outward while
// tracking `reach`, the farthest point reachable without another stop
// (starts at P). Any stop already passed but not yet used for fuel sits in
// a max-heap; whenever `reach` falls short of the next stop (or the town),
// repeatedly take the single largest available fuel amount from the heap
// until `reach` catches up or the heap runs dry (-> -1). This is optimal
// because delaying the choice of *which* passed stop to use, and always
// picking the biggest one when a refuel is actually forced, never does
// worse than committing earlier.
//
// distFromTruck is only kept in [0, L] as a safety filter; the statement
// guarantees stops lie strictly between the town and the truck, so this
// never rejects real input. Reaching the town with exactly 0 fuel left
// counts as success, and if P already covers L the answer is 0 stops.
#include <cstdio>
#include <algorithm>
#include <queue>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    // distFromTruck, fuel
    static pair<int,int> stops[10005];
    int m = 0;
    for (int i = 0; i < n; i++) {
        int a, f;
        scanf("%d %d", &a, &f);
        stops[m].first = -1; // placeholder, fill below
        stops[m].second = f;
        stops[m].first = a; // temporarily store a, convert after reading L
        m++;
    }
    int L, P;
    scanf("%d %d", &L, &P);

    // convert distance from town -> distance from truck, filter valid range
    int cnt = 0;
    for (int i = 0; i < m; i++) {
        int distFromTruck = L - stops[i].first;
        if (distFromTruck >= 0 && distFromTruck <= L) {
            stops[cnt].first = distFromTruck;
            stops[cnt].second = stops[i].second;
            cnt++;
        }
    }
    sort(stops, stops + cnt);

    priority_queue<int> pq;
    int reach = P;
    int stopsUsed = 0;
    bool failed = false;

    for (int i = 0; i < cnt; i++) {
        while (reach < stops[i].first) {
            if (pq.empty()) { failed = true; break; }
            reach += pq.top(); pq.pop();
            stopsUsed++;
        }
        if (failed) break;
        pq.push(stops[i].second);
    }

    if (!failed) {
        while (reach < L) {
            if (pq.empty()) { failed = true; break; }
            reach += pq.top(); pq.pop();
            stopsUsed++;
        }
    }

    if (failed) {
        printf("-1\n");
    } else {
        printf("%d\n", stopsUsed);
    }
    return 0;
}
