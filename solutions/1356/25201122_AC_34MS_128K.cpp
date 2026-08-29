// POJ 1356 - Grandpa's Other Estate
// Model: claude-sonnet-5
// Approach: fixed-size axis-aligned square covering max points. The count of
// points inside window [X,X+r] (per axis) as a function of X is a step
// function whose breakpoints are exactly at X = x_i and X = x_i - r for each
// point i (inclusive containment: point enters window when X drops to x_i,
// exits when X exceeds x_i, i.e. relevant boundary values are x_i and
// x_i - r). So it suffices to try all such candidate left-edges for X and Y
// independently (2n each), then for every combination count points inside in
// O(n). n <= 100, t <= 10, so 10 * (200*200) * 100 = 4*10^7 worst case,
// comfortably within limits.
// No statement ambiguity found; sample and a board-suggested stress case
// (4 points forming a diamond, r=2, all 4 covered by one square) both verified.
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n, r;
        scanf("%d %d", &n, &r);
        static int x[105], y[105];
        for (int i = 0; i < n; i++) scanf("%d %d", &x[i], &y[i]);

        static int cx[205], cy[205];
        int ncx = 0, ncy = 0;
        for (int i = 0; i < n; i++) {
            cx[ncx++] = x[i];
            cx[ncx++] = x[i] - r;
            cy[ncy++] = y[i];
            cy[ncy++] = y[i] - r;
        }

        int best = 0;
        for (int a = 0; a < ncx; a++) {
            int X = cx[a];
            for (int b = 0; b < ncy; b++) {
                int Y = cy[b];
                int cnt = 0;
                for (int i = 0; i < n; i++) {
                    if (x[i] >= X && x[i] <= X + r && y[i] >= Y && y[i] <= Y + r)
                        cnt++;
                }
                if (cnt > best) best = cnt;
            }
        }
        printf("%d\n", best);
    }
    return 0;
}
