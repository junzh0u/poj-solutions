// POJ 2815 - Context-Free Clock
// Model: claude-sonnet-5
//
// Relative angular speed of minute hand vs hour hand (clockwise, minute - hour)
// is 11/120 degree per second. If S is the start time in seconds since
// midnight and t (real seconds since midnight) is a solution time, then the
// angle from hour to minute hand at time t equals A degrees exactly when
// 11*t == 120*A + 43200*k for some integer k >= 0 (43200 = period*11, where
// the base period between consecutive occurrences of angle A is 43200/11
// seconds). We want the smallest such t that is >= S, then floor it to the
// nearest whole second for output (problem says round down), then reduce
// modulo one day since only a time-of-day, not a date, is printed.
//
// All arithmetic is done in exact integers (multiples of 1/11 second) to
// avoid floating point error at exact-second boundaries (e.g. case "0
// 12:00:00" must print exactly 12:00:00, not 11:59:59 or off by epsilon).
#include <cstdio>

int main() {
    int A, HH, MM, SS;
    while (scanf("%d %d:%d:%d", &A, &HH, &MM, &SS) == 4) {
        if (A == -1) break;
        long S = (long)HH * 3600 + MM * 60 + SS;
        long num = 11 * S - 120 * (long)A; // = 11*(S - t0)
        long k;
        if (num <= 0) k = 0;
        else k = (num + 43200 - 1) / 43200;
        long tnum = 120 * (long)A + 43200 * k; // = 11 * t
        long tfloor = tnum / 11;                     // floor(t)
        while (tfloor >= 86400) tfloor -= 86400;
        int oh = (int)(tfloor / 3600);
        int om = (int)((tfloor % 3600) / 60);
        int os = (int)(tfloor % 60);
        printf("%02d:%02d:%02d\n", oh, om, os);
    }
    return 0;
}
