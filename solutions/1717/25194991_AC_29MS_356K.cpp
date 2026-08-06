// POJ 1717 - Dominoes
// Model: claude-sonnet-5
// Approach: Let diff = topSum - bottomSum. Flipping domino i (a,b) changes
// diff by 2*(b-a). Let d_i = b_i - a_i (range -6..6). We need to choose a
// subset of dominoes to flip so that |diff + 2*sum(d_i over flipped)| is
// minimized, and among all subsets achieving that minimum, minimize the
// subset size (number of flips). This is a 0/1 knapsack over achievable
// sums S = sum of d_i (range -6000..6000 since n<=1000, |d_i|<=6), tracking
// the minimum number of flips to realize each S. Two rolling arrays are
// used to keep the 0/1 property (each domino contributes at most once).
// Final answer: min over S of flips[S] such that |diff+2S| is minimal.
// No ambiguity found in the statement; sample matches straightforwardly.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXD = 6000;
const int SIZE = 2 * MAXD + 1; // offset by MAXD
const int INF = 1 << 29;

int cur[SIZE];
int nxt[SIZE];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int diff = 0;
    int d[1005];
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        diff += (a - b);
        d[i] = b - a;
    }

    for (int i = 0; i < SIZE; ++i) cur[i] = INF;
    cur[MAXD] = 0; // S=0, 0 flips

    for (int i = 0; i < n; ++i) {
        int di = d[i];
        if (di == 0) continue; // flipping doesn't change S, never helps, skip
        memcpy(nxt, cur, sizeof(int) * SIZE);
        // try flipping this domino: S -> S + di
        if (di > 0) {
            for (int s = SIZE - 1; s >= di; --s) {
                if (cur[s - di] + 1 < nxt[s]) nxt[s] = cur[s - di] + 1;
            }
        } else {
            for (int s = 0; s < SIZE + di; ++s) {
                if (cur[s - di] + 1 < nxt[s]) nxt[s] = cur[s - di] + 1;
            }
        }
        memcpy(cur, nxt, sizeof(int) * SIZE);
    }

    int bestGap = -1;
    int bestFlips = INF;
    for (int s = 0; s < SIZE; ++s) {
        if (cur[s] >= INF) continue;
        int S = s - MAXD;
        int gap = diff + 2 * S;
        if (gap < 0) gap = -gap;
        if (bestGap < 0 || gap < bestGap || (gap == bestGap && cur[s] < bestFlips)) {
            if (bestGap < 0 || gap < bestGap) {
                bestGap = gap;
                bestFlips = cur[s];
            } else if (gap == bestGap && cur[s] < bestFlips) {
                bestFlips = cur[s];
            }
        }
    }

    printf("%d\n", bestFlips);
    return 0;
}
