// POJ 3059 - Rummikub
// Model: gpt-6.1-sol
// Sweep numbers, remembering each color's unfinished run length (capped at 3).
// Choose run tiles first; at least three remaining colors form a group.
// The malformed example '6g, 61g, ...' means consecutive values, as stated.
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;
int main() {
    int trans[256][16], bits[16];
    for (int m=0;m<16;++m) {
        bits[m]=0;
        for(int c=0;c<4;++c) if(m&(1<<c)) ++bits[m];
    }
    for(int s=0;s<256;++s) for(int m=0;m<16;++m) {
        int ns=0; bool ok=true;
        for(int c=0;c<4;++c) {
            int len=(s>>(2*c))&3;
            if(m&(1<<c)) ns|=min(3,len+1)<<(2*c);
            else if(len==1 || len==2) ok=false;
        }
        trans[s][m]=ok?ns:-1;
    }
    int cases; cin>>cases;
    while(cases--) {
        int n, have[102]={0}; cin>>n;
        for(int i=0;i<n;++i) {
            string tile; cin>>tile;
            int value=0;
            for(size_t j=0;j+1<tile.size();++j) value=value*10+tile[j]-'0';
            string colors="yrgb";
            int color=(int)colors.find(tile[tile.size()-1]);
            have[value]|=1<<color;
        }
        int dp[256], next[256];
        fill(dp,dp+256,-1000000); dp[0]=0;
        for(int v=1;v<=101;++v) {
            fill(next,next+256,-1000000);
            for(int s=0;s<256;++s) if(dp[s]>=0) {
                for(int m=have[v];;m=(m-1)&have[v]) {
                    int ns=trans[s][m];
                    if(ns>=0) {
                        int remaining=bits[have[v]^m];
                        int count=bits[m]+(remaining>=3?remaining:0);
                        next[ns]=max(next[ns],dp[s]+v*count);
                    }
                    if(m==0) break;
                }
            }
            copy(next,next+256,dp);
        }
        cout<<dp[0]<<'\n';
    }
    return 0;
}
