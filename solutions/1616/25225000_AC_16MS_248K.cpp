// POJ 1616 - Harmonic Periods
// Model: gpt-6.1-sol
// Sort harmonic periods and count idle slots in each repeating period.
// A task consumes the first C available slots; invert those removals from
// longest to shortest period to locate the last task's completion time.
// Jobs are released together at time zero, as in the statement's schedule.
#include <iostream>
#include <algorithm>
using namespace std;
struct Task { int c, p; bool operator<(const Task& b) const { return p < b.p; } };
int main() {
    int cases; cin >> cases;
    while (cases--) {
        int n; cin >> n;
        Task a[100]; int freeSlots[100];
        for (int i=0;i<n;++i) cin >> a[i].c >> a[i].p;
        sort(a,a+n);
        bool ok=true;
        for (int i=0;i<n;++i) {
            int available = i ? freeSlots[i-1]*(a[i].p/a[i-1].p) : a[i].p;
            freeSlots[i]=available-a[i].c;
            if (freeSlots[i]<0) { ok=false; break; }
        }
        if (!ok) { cout << -1 << '\n'; continue; }
        int rank=a[n-1].c;
        for (int i=n-2;i>=0;--i) {
            int blocks=(rank-1)/freeSlots[i];
            int rest=(rank-1)%freeSlots[i]+1;
            rank=blocks*(freeSlots[i]+a[i].c)+a[i].c+rest;
        }
        cout << rank << '\n';
    }
    return 0;
}
