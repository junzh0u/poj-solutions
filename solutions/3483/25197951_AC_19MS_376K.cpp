// POJ 3483 - Loan Scheduling
// Model: claude-sonnet-5
//
// Each application has a profit p and a deadline d, meaning it must be
// scheduled at some integer time t in [0,d]. At most L applications may be
// scheduled at the same time slot. Choose a subset and an assignment of
// time slots to maximize total profit.
//
// This is the classic job-sequencing-with-deadlines greedy generalized to
// capacity L per slot: process applications in decreasing order of profit,
// and for each one assign it to the latest available time slot <= its
// deadline (a slot is "available" while its remaining capacity > 0).
// Placing at the latest possible slot (rather than earliest, or by
// deadline order) is what keeps earlier slots free for applications with
// tighter deadlines that are considered later; the discuss board's own
// counter-examples confirm greedy-by-deadline and front-to-back insertion
// both fail, while latest-available-slot-by-profit passes them.
//
// "Latest available slot <= d" with per-slot capacity L is implemented
// with a union-find over time slots 0..maxD, shifted by one so index 0 is
// a permanent sentinel "no slot available" (used only when L==0 or all
// slots up to d are exhausted). find(x) returns the root slot; when a
// slot's capacity reaches zero it is unioned to the previous slot so later
// finds skip over it, giving amortized-near-O(1) queries.
//
// Verified against the statement's worked example (answer 9) by hand and
// against all four sample cases (9, 2050, 0, 0), including the L=0 case
// which must still consume the N application pairs from input but
// contributes zero profit.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXD = 10005;
int parentArr[MAXD];
int cap[MAXD];

int find(int x) {
    while (parentArr[x] != x) {
        parentArr[x] = parentArr[parentArr[x]];
        x = parentArr[x];
    }
    return x;
}

struct App {
    int p, d;
};

bool cmp(const App &a, const App &b) {
    return a.p > b.p;
}

App apps[10005];

int main() {
    int n, l;
    while (scanf("%d %d", &n, &l) == 2) {
        int maxd = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d %d", &apps[i].p, &apps[i].d);
            if (apps[i].d > maxd) maxd = apps[i].d;
        }

        int ans = 0;

        if (l > 0 && n > 0) {
            sort(apps, apps + n, cmp);

            // index 0 = sentinel "no slot", index t+1 = real time t
            int sz = maxd + 2;
            for (int i = 0; i < sz; i++) {
                parentArr[i] = i;
                cap[i] = l;
            }
            cap[0] = 0; // sentinel always has zero capacity

            for (int i = 0; i < n; i++) {
                int idx = apps[i].d + 1;
                int s = find(idx);
                if (s == 0) continue; // no slot available
                ans += apps[i].p;
                cap[s]--;
                if (cap[s] == 0 && s > 0) {
                    parentArr[s] = find(s - 1);
                }
            }
        }

        printf("%d\n", ans);
    }
    return 0;
}
