// POJ 2058 - Word Encoding
// Model: gpt-6.1-sol
// Keep the last two letters: forbidden substrings have length at most three.
// Count valid suffixes by remaining length, then rank or unrank in shortlex order.
// Counts saturate at 2000000000, the largest possible input or output rank.
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;
const int CAP=2000000000;
int tr[703][26], dp[21][703];
bool one[26],two[26][26],three[26][26][26];
int add(int a,int b) { return b>=CAP-a?CAP:a+b; }
int main() {
    int T; cin>>T;
    while(T--) {
        int n,m; cin>>n>>m;
        memset(one,0,sizeof(one)); memset(two,0,sizeof(two));
        memset(three,0,sizeof(three));
        string s;
        for(int i=0;i<n;++i) {
            cin>>s;
            int a=s[0]-'a';
            if(s.size()==1) one[a]=true;
            else if(s.size()==2) two[a][s[1]-'a']=true;
            else three[a][s[1]-'a'][s[2]-'a']=true;
        }
        for(int st=0;st<703;++st) for(int c=0;c<26;++c) {
            int a=-1,b=-1;
            if(st>=27) { a=(st-27)/26; b=(st-27)%26; }
            else if(st) b=st-1;
            if(one[c] || (b>=0 && two[b][c]) || (a>=0 && three[a][b][c])) tr[st][c]=-1;
            else tr[st][c]=(b<0?1+c:27+b*26+c);
        }
        for(int st=0;st<703;++st) dp[0][st]=1;
        for(int l=1;l<=20;++l) for(int st=0;st<703;++st) {
            dp[l][st]=0;
            for(int c=0;c<26;++c) if(tr[st][c]>=0)
                dp[l][st]=add(dp[l][st],dp[l-1][tr[st][c]]);
        }
        while(m--) {
            cin>>s;
            if(s[0]>='0' && s[0]<='9') {
                int k=atoi(s.c_str()),l=1,st=0;
                while(l<20 && k>dp[l][0]) { k-=dp[l][0]; ++l; }
                string ans;
                for(int r=l-1;r>=0;--r) for(int c=0;c<26;++c) {
                    int ns=tr[st][c];
                    if(ns<0) continue;
                    if(k>dp[r][ns]) k-=dp[r][ns];
                    else { ans+=char('a'+c); st=ns; break; }
                }
                cout<<ans<<'\n';
            } else {
                int rank=1,st=0,l=int(s.size());
                for(int i=1;i<l;++i) rank=add(rank,dp[i][0]);
                for(int i=0;i<l;++i) {
                    for(int c=0;c<s[i]-'a';++c) if(tr[st][c]>=0)
                        rank=add(rank,dp[l-i-1][tr[st][c]]);
                    st=tr[st][s[i]-'a'];
                }
                cout<<rank<<'\n';
            }
        }
    }
}
