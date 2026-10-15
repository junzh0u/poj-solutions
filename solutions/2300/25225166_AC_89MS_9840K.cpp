// POJ 2300 - Up the Ante
// Model: gpt-6.1-sol
// Propagate probabilities by net winnings and the next bet exponent.
// Absorb a path as soon as its winnings are positive in rounds k through m.
// The wheel gives payouts -1, 1, 2, 3 with weights 10, 2, 1, 1 out of 14;
// the prose symmetry, rather than details of the photograph, defines these.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int OFFSET = 15360, WIDTH = 61441;
static double dp[2][10][WIDTH];
int main() {
    int cases;
    scanf("%d", &cases);
    while (cases--) {
        int k, m, limit;
        scanf("%d%d%d", &k, &m, &limit);
        int levels = 1;
        while ((1 << levels) <= limit) ++levels;
        memset(dp, 0, sizeof(dp));
        dp[0][0][OFFSET] = 1.0;
        int low = 0, high = 0;
        double answer = 0.0;
        for (int round = 1; round <= m; ++round) {
            int cur = (round-1)&1, next = round&1;
            memset(dp[next], 0, sizeof(dp[next]));
            int nlow = 0, nhigh = 0;
            for (int e = 0; e < levels; ++e) {
                int bet = 1 << e;
                for (int money = low; money <= high; ++money) {
                    double p = dp[cur][e][money+OFFSET];
                    if (p == 0.0) continue;
                    for (int outcome = 0; outcome < 4; ++outcome) {
                        int gain = outcome == 0 ? -bet : outcome*bet;
                        int value = money + gain;
                        double q = p * (outcome == 0 ? 10 : outcome == 1 ? 2 : 1) / 14.0;
                        if (round >= k && value > 0) answer += q;
                        else {
                            int ne = outcome == 0 && e+1 < levels ? e+1 : 0;
                            dp[next][ne][value+OFFSET] += q;
                            nlow = min(nlow, value);
                            nhigh = max(nhigh, value);
                        }
                    }
                }
            }
            low = nlow; high = nhigh;
        }
        printf("%.4f\n", answer);
    }
    return 0;
}
