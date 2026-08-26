// POJ 2596 - Dice Stacking
// Model: gpt-5.6-terra
// Dynamic programming over the chosen dice, last die, and its top face.  A
// die may have repeated labels, so orientations are represented by face index
// rather than just by the exposed number.
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

int main() {
    const int opposite[6] = {5, 3, 4, 1, 2, 0};
    int cases;
    cin >> cases;
    while (cases--) {
        int n, value[10][6], dp[1 << 10][10][6];
        cin >> n;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < 6; ++j)
                cin >> value[i][j];
        memset(dp, -1, sizeof(dp));
        for (int i = 0; i < n; ++i) {
            for (int top = 0; top < 6; ++top) {
                int side = 0;
                for (int face = 0; face < 6; ++face)
                    if (face != top && face != opposite[top])
                        side = max(side, value[i][face]);
                dp[1 << i][i][top] = side;
            }
        }
        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int last = 0; last < n; ++last) {
                for (int top = 0; top < 6; ++top) {
                    if (dp[mask][last][top] < 0) continue;
                    for (int next = 0; next < n; ++next) {
                        if (mask & (1 << next)) continue;
                        for (int nextTop = 0; nextTop < 6; ++nextTop) {
                            if (value[next][opposite[nextTop]] != value[last][top]) continue;
                            int side = 0;
                            for (int face = 0; face < 6; ++face)
                                if (face != nextTop && face != opposite[nextTop])
                                    side = max(side, value[next][face]);
                            dp[mask | (1 << next)][next][nextTop] = max(
                                dp[mask | (1 << next)][next][nextTop],
                                dp[mask][last][top] + side);
                        }
                    }
                }
            }
        }
        int answer = 0, all = (1 << n) - 1;
        for (int last = 0; last < n; ++last)
            for (int top = 0; top < 6; ++top)
                answer = max(answer, dp[all][last][top]);
        cout << answer << '\n';
    }
    return 0;
}
