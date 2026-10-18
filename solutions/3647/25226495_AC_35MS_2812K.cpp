// POJ 3647 - Tournament
// Model: gpt-6-astra
// With P the least power of two >= n, award P-n byes and form n-P/2 pairs.
// An optimal pairing uses adjacent abilities in sorted order: uncross pairs,
// then replace an endpoint by any skipped interior knight to decrease cost.
// Prefix DP either awards the last knight a bye or pairs the last two.
// Store the choices to reconstruct the actual bye names; ties are arbitrary.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Knight { string name; double ability; };
bool lessAbility(const Knight &a, const Knight &b) { return a.ability < b.ability; }
int main() {
    ios::sync_with_stdio(false);
    int n; bool first = true;
    while (cin >> n && n) {
        vector<Knight> a(n);
        for (int i=0;i<n;++i) cin >> a[i].name >> a[i].ability;
        sort(a.begin(),a.end(),lessAbility);
        int p=1; while(p<n) p*=2;
        int pairs=n-p/2, width=pairs+1;
        vector<unsigned char> take((n+1)*width,0);
        vector<double> dp[3];
        for(int r=0;r<3;++r) dp[r].assign(width,1e300);
        dp[0][0]=0;
        for(int i=1;i<=n;++i) {
            vector<double> &cur=dp[i%3];
            fill(cur.begin(),cur.end(),1e300); cur[0]=0;
            for(int j=1;j<=pairs && 2*j<=i;++j) {
                cur[j]=dp[(i+2)%3][j];
                double d=a[i-1].ability-a[i-2].ability;
                double v=dp[(i+1)%3][j-1]+d*d;
                if(v<cur[j]) {cur[j]=v;take[i*width+j]=1;}
            }
        }
        if(!first) cout << '\n'; first=false;
        int i=n,j=pairs;
        while(i>0) {
            if(take[i*width+j]) {i-=2;--j;}
            else {cout << a[i-1].name << '\n';--i;}
        }
    }
    return 0;
}
