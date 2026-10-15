// POJ 3899 - The Lucky Numbers
// Model: gpt-6.1-sol
// Count the union of the lucky numbers in [A,B] and their reversals.
// Inclusion-exclusion uses a digit DP for x <= U and reverse(x) <= V.
// Appending a digit preserves a decided forward comparison; in reverse
// it prepends a digit, which overrides the old comparison when unequal.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
typedef long long Int;
int cmp(char a, char b) { return a < b ? 0 : (a == b ? 1 : 2); }
string before(string s) {
    int i = (int)s.size()-1;
    while (s[i]=='0') { s[i]='9'; --i; }
    --s[i];
    if (s.size()>1 && s[0]=='0') s.erase(0,1);
    return s;
}
Int both(string u, string v) {
    Int result=0;
    int limit=(int)(u.size()<v.size()?u.size():v.size());
    for (int n=1;n<=limit;++n) {
        string a=u.size()>(unsigned)n?string(n,'9'):u;
        string b=v.size()>(unsigned)n?string(n,'9'):v;
        Int dp[3][3]={0}; dp[1][1]=1;
        for(int i=0;i<n;++i) {
            Int next[3][3]={0};
            for(int x=0;x<3;++x) for(int y=0;y<3;++y)
                for(int d=0;d<2;++d) {
                    char c=d?'7':'4';
                    int p=x==1?cmp(c,a[i]):x;
                    int q=cmp(c,b[n-1-i]);
                    if(q==1) q=y;
                    next[p][q]+=dp[x][y];
                }
            memcpy(dp,next,sizeof dp);
        }
        for(int x=0;x<=1;++x) for(int y=0;y<=1;++y) result+=dp[x][y];
    }
    return result;
}
Int count(string s) { return both(s,string(s.size(),'9')); }
int main() {
    int t; cin>>t;
    while(t--) {
        string a,b; cin>>a>>b; a=before(a);
        Int inside=count(b)-count(a);
        Int overlap=both(b,b)-2*both(a,b)+both(a,a);
        cout<<2*inside-overlap<<'\n';
    }
    return 0;
}
