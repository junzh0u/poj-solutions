// POJ 3558 - Map Generator returns (MG-II)
// Model: gpt-6-astra
// Explore the component of vertex 1. dp[t][d] is the probability that
// t vertices have been processed and d have been discovered. An active
// vertex discovers a binomial number of the n-d unseen vertices.
// All transitions are nonnegative, avoiding cancellation at small P.
// FAMI uses independent undirected edges, as defined in problem 3557.
#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    int n;
    double p;
    if (!(cin >> n >> p)) return 0;
    double binom[21][21] = {{0}}, dp[21][21] = {{0}};
    binom[0][0] = 1;
    for (int m = 1; m <= n; ++m)
        for (int j = 0; j <= m; ++j) {
            binom[m][j] = (1-p)*binom[m-1][j];
            if (j) binom[m][j] += p*binom[m-1][j-1];
        }
    dp[0][1] = 1;
    for (int t = 0; t < n; ++t)
        for (int d = t+1; d <= n; ++d)
            for (int j = 0; j <= n-d; ++j)
                dp[t+1][d+j] += dp[t][d]*binom[n-d][j];
    cout << fixed << setprecision(12) << dp[n][n] << '\n';
    return 0;
}
