// POJ 2158 - A Difficult Mathematics Problem
// Model: gpt-6.1-sol
// dp[x] sums products for prefixes ending in x. Appending y multiplies
// each product by min(x,y)+1. Weighted prefixes and unweighted suffixes
// evaluate all transitions in O(N*K) time and O(K) space.
// All arithmetic stays integral; the statement guarantees a result < 2^63.
#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<long long> dp(k, 1), next(k);
    for (int len = 1; len < n; ++len) {
        long long suffix = 0, prefix = 0;
        for (int x = 0; x < k; ++x) suffix += dp[x];
        for (int y = 0; y < k; ++y) {
            suffix -= dp[y];
            prefix += dp[y] * (y + 1);
            next[y] = prefix + suffix * (y + 1);
        }
        dp.swap(next);
    }
    long long answer = 0;
    for (int x = 0; x < k; ++x) answer += dp[x];
    cout << answer << '\n';
    return 0;
}
