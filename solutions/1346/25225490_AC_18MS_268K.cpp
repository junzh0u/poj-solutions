// POJ 1346 - Board of Directors Meeting
// Model: gpt-6.1-sol
// Keep a longest common circular subsequence of non-speakers, excluding
// at least one initially red member who can deliver the first address.
// Later speakers can be made red by vacancy circulation; circulation also
// permits either final chair-color phase once an address has occurred.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int c,n,a[24],b[24],pos[24],dp[24],v[24];
    string s,t;
    while (cin>>c && c) {
        cin>>n>>s; a[0]=0;
        for(int i=1;i<n;++i){cin>>a[i];--a[i];}
        cin>>t; b[0]=0;
        for(int i=1;i<n;++i){cin>>b[i];--b[i];}
        bool same=s[0]==t[0];
        for(int i=0;i<n;++i) if(a[i]!=b[i])same=false;
        int best=0;
        if(same){cout<<c<<" 0\n";continue;}
        for(int removed=0;removed<n;++removed) {
            if((removed+(s[0]=='R'))%2!=1)continue;
            for(int start=0;start<n;++start) {
                if(start==removed)continue;
                int target=0;
                while(b[target]!=a[start])++target;
                for(int i=0;i<n;++i)pos[b[(target+i)%n]]=i;
                int len=0;
                for(int i=0;i<n;++i)
                    if((start+i)%n!=removed)v[len++]=pos[a[(start+i)%n]];
                for(int i=0;i<len;++i) {
                    dp[i]=1;
                    for(int j=0;j<i;++j)if(v[j]<v[i])dp[i]=max(dp[i],dp[j]+1);
                    best=max(best,dp[i]);
                }
            }
        }
        cout<<c<<' '<<n-best<<'\n';
    }
    return 0;
}
