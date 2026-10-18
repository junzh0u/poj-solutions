// POJ 2794 - Double Patience
// Model: gpt-6-astra
// Memoize the win probability for each base-five vector of pile heights.
// Average over individual legal pairs, not over ranks; input is bottom to top.
// At most 5^9 states, 36 candidate pairs per state, and depth 18.
#include <cstdio>
#include <algorithm>
static double memo[1953125];
static char cards[9][5];
static int power5[9];
double solve(int state) {
    double &ans = memo[state];
    if (ans >= 0) return ans;
    int h[9], s = state;
    for (int i=0;i<9;++i) { h[i]=s%5; s/=5; }
    double sum=0;
    int count=0;
    for (int i=0;i<9;++i) if (h[i])
        for (int j=0;j<i;++j) if (h[j] && cards[i][h[i]]==cards[j][h[j]]) {
            sum += solve(state-power5[i]-power5[j]);
            ++count;
        }
    return ans = count ? sum/count : 0;
}
int main() {
    char card[8];
    for (int i=0;i<9;++i) {
        power5[i]= i ? power5[i-1]*5 : 1;
        for (int j=1;j<=4;++j) {
            if (std::scanf("%7s",card)!=1) return 0;
            cards[i][j]=card[0];
        }
    }
    std::fill(memo,memo+1953125,-1.0);
    memo[0]=1;
    std::printf("%.6f\n",solve(1953124));
    return 0;
}
