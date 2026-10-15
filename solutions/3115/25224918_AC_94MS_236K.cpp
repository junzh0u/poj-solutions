// POJ 3115 - Energy x Delay
// Model: gpt-6.1-sol
// Dynamic programming by the frequency after each program, O(P*F) time.
// Total EDP is the sum of the individual program products and E*A for
// each frequency change, including a possible change from initial level 1.
// The sample distinguishes this from multiplying total energy and time.
#include <iostream>
#include <algorithm>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int f,p,e,a;
    const long long INF = 1000000000000LL;
    while (cin >> f >> p >> e >> a && f) {
        long long dp[20], next[20];
        for (int j=0;j<f;++j) dp[j]=(j==0 ? 0 : INF);
        for (int i=0;i<p;++i) {
            long long best=*min_element(dp,dp+f);
            for (int j=0;j<f;++j) {
                int energy,delay;
                cin >> energy >> delay;
                next[j]=min(dp[j],best+e*a)+energy*delay;
            }
            copy(next,next+f,dp);
        }
        cout << *min_element(dp,dp+f) << '\n';
    }
    return 0;
}
