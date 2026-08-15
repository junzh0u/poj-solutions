// POJ 3112 - Digital Biochemist Circuit
// Model: claude-sonnet-5
// Approach: For each test case, stream N rows of P bits. Maintain a
// per-column current run length (consecutive 1s down that column). Whenever
// a run ends (a 0 is read, or we reach the last row while still in a run),
// check whether its length is >= C and count it. Runs are counted
// column-wise; a single column can contribute multiple qualifying runs, and
// rows do not matter for run detection (confirmed against the discuss board
// and against the sample's second case, which needs multiple counts from
// different columns and a run that terminates only at the last row).
#include <cstdio>

int curRun[1005];

int main() {
    int P, N, C;
    while (std::scanf("%d %d %d", &P, &N, &C) == 3) {
        if (P == 0 && N == 0 && C == 0) break;
        for (int j = 0; j < P; ++j) curRun[j] = 0;
        long count = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < P; ++j) {
                int bit;
                std::scanf("%d", &bit);
                if (bit == 1) {
                    curRun[j]++;
                } else {
                    if (curRun[j] >= C) count++;
                    curRun[j] = 0;
                }
            }
        }
        // flush runs still open at the end of the last row
        for (int j = 0; j < P; ++j) {
            if (curRun[j] >= C) count++;
        }
        std::printf("%ld\n", count);
    }
    return 0;
}
