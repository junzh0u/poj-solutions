// POJ 1549 - Bright Bracelet
// Model: gpt-6.1-sol
// Fix the first octagon, then extend chains by subset and exposed color.
// Rotations select opposite edges; turning by four edges reverses the pair
// without flipping the octagon. Equal brightness never merges colors.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
const int INF=1000000000;
int dp[2048][8], w[8], a[11][8];
int main() {
    int n;
    while(cin>>n && n) {
        for(int c=0;c<8;++c) cin>>w[c];
        for(int i=0;i<n;++i) {
            string s; cin>>s;
            for(int j=0;j<8;++j) a[i][j]=s[j]-'A';
        }
        int answer=INF, full=(1<<n)-1;
        for(int r=0;r<8;++r) {
            int start=a[0][r], end=a[0][(r+4)%8];
            for(int mask=0;mask<=full;++mask)
                for(int c=0;c<8;++c) dp[mask][c]=INF;
            dp[1][end]=0;
            for(int mask=1;mask<=full;mask+=2)
                for(int c=0;c<8;++c) if(dp[mask][c]<INF)
                    for(int i=1;i<n;++i) if(!(mask&(1<<i)))
                        for(int j=0;j<8;++j) if(a[i][j]==c) {
                            int &v=dp[mask|(1<<i)][a[i][(j+4)%8]];
                            v=min(v,dp[mask][c]+w[c]);
                        }
            answer=min(answer,dp[full][start]+w[start]);
        }
        if(answer>=INF) cout<<"impossible\n";
        else cout<<answer<<'\n';
    }
}
