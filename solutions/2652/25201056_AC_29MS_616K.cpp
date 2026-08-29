// POJ 2652 - Ferry Loading III
// Model: claude-sonnet-5
// Approach: Simulate the ferry directly. Split cars into two FIFO queues (left
// bank / right bank) by original arrival order (arrival times are strictly
// increasing across the whole test case, so each per-bank subsequence is
// already sorted). Track the ferry's current bank and the time it is ready
// to load there. At each step: load up to n cars from the current bank whose
// arrival <= current time (oldest first); if any were loaded, depart
// immediately (time += t, flip bank). If none were loaded but the OTHER bank
// already has a car waiting (arrived <= current time), the ferry must still
// travel there (it moves whenever any car anywhere is waiting), so it
// crosses empty (time += t, flip bank) without advancing the clock further.
// Otherwise, if cars remain anywhere, the ferry is fully idle: jump time to
// the next arrival (whichever bank) and re-evaluate without switching bank
// yet. This reproduces both sample outputs exactly, including the
// alternating "wait, then cross empty, then load" pattern in sample 2.
//
// No statement ambiguity beyond confirming this dispatch rule against both
// sample cases by hand simulation.
#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int c;
    if (scanf("%d", &c) != 1) return 0;
    for (int tc = 0; tc < c; tc++) {
        if (tc > 0) printf("\n");
        long long n, t;
        int m;
        scanf("%lld %lld %d", &n, &t, &m);
        vector<long long> arr(m);
        vector<int> side(m); // 0 = left, 1 = right
        vector<long long> ans(m);
        char buf[16];
        for (int i = 0; i < m; i++) {
            scanf("%lld %s", &arr[i], buf);
            side[i] = (buf[0] == 'r') ? 1 : 0;
        }
        // per-bank lists of original indices, in arrival order
        vector<int> q[2];
        for (int i = 0; i < m; i++) q[side[i]].push_back(i);

        int idx[2] = {0, 0};
        int curBank = 0; // start on left
        long long curTime = 0;

        while (idx[0] < (int)q[0].size() || idx[1] < (int)q[1].size()) {
            int other = 1 - curBank;
            // load up to n cars from curBank with arrival <= curTime
            int loaded = 0;
            while (loaded < n && idx[curBank] < (int)q[curBank].size() &&
                   arr[q[curBank][idx[curBank]]] <= curTime) {
                int carIdx = q[curBank][idx[curBank]];
                ans[carIdx] = curTime + t;
                idx[curBank]++;
                loaded++;
            }
            if (loaded > 0) {
                curTime += t;
                curBank = other;
                continue;
            }
            // nothing loaded here
            bool otherWaiting = (idx[other] < (int)q[other].size() &&
                                  arr[q[other][idx[other]]] <= curTime);
            if (otherWaiting) {
                curTime += t;
                curBank = other;
                continue;
            }
            // fully idle: jump to next arrival among remaining cars
            long long nextT = -1;
            if (idx[curBank] < (int)q[curBank].size()) {
                long long v = arr[q[curBank][idx[curBank]]];
                if (nextT == -1 || v < nextT) nextT = v;
            }
            if (idx[other] < (int)q[other].size()) {
                long long v = arr[q[other][idx[other]]];
                if (nextT == -1 || v < nextT) nextT = v;
            }
            if (nextT == -1) break; // no more cars anywhere
            curTime = nextT;
        }

        for (int i = 0; i < m; i++) printf("%lld\n", ans[i]);
    }
    return 0;
}
