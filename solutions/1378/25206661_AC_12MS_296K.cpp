// POJ 1378 - Power Cable Problem
// Model: claude-sonnet-5
// Approach: For each test case, a power cable is disabled if it contains at
// least one flooded building (per the statement, a cable touching any
// damaged converter cannot power any converter attached to it). A building
// has electricity iff it belongs to at least one cable that is NOT
// disabled; buildings on no cable, or only on disabled cables, are counted
// as out of electricity. A flooded building is always counted as out,
// since every cable it touches becomes disabled by that same rule.
// Input framing: test cases are separated by a "0 0 0" line, which can
// never collide with a real header since the statement gives N, M, K all
// >= 1; the file ends with a "-1 -1 -1" line.

#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int N, M, K;
    while (scanf("%d %d %d", &N, &M, &K) == 3) {
        if (N == -1 && M == -1 && K == -1) break;
        if (N == 0 && M == 0 && K == 0) continue;

        vector<vector<int> > cables(M);
        for (int i = 0; i < M; i++) {
            int c;
            scanf("%d", &c);
            cables[i].resize(c);
            for (int j = 0; j < c; j++) scanf("%d", &cables[i][j]);
        }

        vector<char> flooded(N, 0);
        for (int i = 0; i < K; i++) {
            int b;
            scanf("%d", &b);
            flooded[b] = 1;
        }

        vector<char> hasPower(N, 0);
        for (int i = 0; i < M; i++) {
            bool disabled = false;
            for (size_t j = 0; j < cables[i].size(); j++) {
                if (flooded[cables[i][j]]) { disabled = true; break; }
            }
            if (!disabled) {
                for (size_t j = 0; j < cables[i].size(); j++) {
                    hasPower[cables[i][j]] = 1;
                }
            }
        }

        int outCount = 0;
        for (int i = 0; i < N; i++) if (!hasPower[i]) outCount++;
        printf("%d\n", outCount);
    }
    return 0;
}
