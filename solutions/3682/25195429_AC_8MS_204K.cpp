// POJ 3682 - King Arthur's Birthday Celebration
// Model: claude-sonnet-5
// Let N be the day on which the K-th head appears (i.e. the number of
// coin tosses until K heads occur). N is the sum of K iid Geometric(p)
// waiting times X_1..X_K (trials to get one head), since the process
// restarts fresh after every head.
//   E[N]  = K/p
//   Var(N) = K*(1-p)/p^2      (sum of K iid geometrics' variances)
//   E[N^2] = Var(N) + E[N]^2 = K*(K+1-p)/p^2
// The cost through day N is 1+3+5+...+(2N-1) = N^2 (thousand coins),
// so the expected cost is exactly E[N^2].
// Verified against both samples: K=1,p=1 -> 1.000 1.000;
// K=1,p=0.5 -> days=1/0.5=2, coins=1*(1+1-0.5)/0.25=6 -> 2.000 6.000.
#include <cstdio>

int main() {
    int k;
    double p;
    while (scanf("%d", &k) == 1 && k != 0) {
        scanf("%lf", &p);
        double days = k / p;
        double coins = k * (k + 1 - p) / (p * p);
        printf("%.3f %.3f\n", days, coins);
    }
    return 0;
}
