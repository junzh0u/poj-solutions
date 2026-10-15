// POJ 2557 - Fold
// Model: gpt-6.1-sol
// Interval DP on stripes. A first fold can remove the shorter side only
// when overlapping future creases are opposite under reflection.
// All layers are folded together; the longer side retains its pattern.
// Precomputed opposite-palindrome radii make transitions constant time.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int dp[201][201], radius_[200];
int main() {
    string s;
    while (cin >> s) {
        int n = s.size();
        for (int c=0;c<n;++c) {
            radius_[c]=0;
            for (int d=1;c-d>=0 && c+d<n && s[c-d]!=s[c+d];++d)
                radius_[c]=d;
        }
        for (int l=0;l<=n;++l) dp[l][l]=0;
        for (int len=2;len<=n+1;++len) {
            for (int l=0;l+len<=n+1;++l) {
                int r=l+len-1;
                dp[l][r]=len-1;
                for (int k=l+1;k<=r;++k) {
                    int a=k-l,b=r-k+1;
                    if (radius_[k-1] < min(a,b)-1) continue;
                    int rest=a<=b ? dp[k][r] : dp[l][k-1];
                    dp[l][r]=min(dp[l][r],rest+1);
                }
            }
        }
        cout << dp[0][n] << '\n';
    }
    return 0;
}
