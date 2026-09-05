// POJ 1211 - Traffic Lights
// Model: claude-sonnet-5
// Approach: each signal with cycle time C is green on [0, C-5) mod 2C, orange
// on [C-5, C), red on [C, 2C). All start green at t=0. We must find the first
// time t>0, AFTER the initial all-green stretch ends (i.e. after the earliest
// signal first turns orange, at t0 = min(C_i - 5)), at which every signal is
// simultaneously green again. Brute force t from t0 up to 5 hours (18000s);
// that bound is small enough (<=100 signals) to just scan every second.
// Ambiguity: "excluded from the reckoning" initial portion means any t before
// t0 trivially satisfies the all-green predicate (since t < C_i-5 for all i)
// but must not be reported as a real recurrence, hence starting the scan at
// t0 rather than t=1.
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int> cyc;
    int x;
    while (true) {
        cyc.clear();
        while (cin >> x && x != 0) cyc.push_back(x);
        if (cyc.empty()) break;
        int t0 = 1000000;
        for (size_t i = 0; i < cyc.size(); ++i) {
            if (cyc[i] - 5 < t0) t0 = cyc[i] - 5;
        }
        const int LIMIT = 5 * 3600;
        int ans = -1;
        for (int t = t0; t <= LIMIT; ++t) {
            bool allGreen = true;
            for (size_t i = 0; i < cyc.size(); ++i) {
                int period = 2 * cyc[i];
                int r = t % period;
                if (!(r < cyc[i] - 5)) { allGreen = false; break; }
            }
            if (allGreen) { ans = t; break; }
        }
        if (ans == -1) {
            printf("Signals fail to synchronise in 5 hours\n");
        } else {
            int h = ans / 3600;
            int m = (ans % 3600) / 60;
            int s = ans % 60;
            printf("%02d:%02d:%02d\n", h, m, s);
        }
    }
    return 0;
}
