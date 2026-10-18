// POJ 3516 - Johnny Hates Math
// Model: gpt-6-astra
// Suffix dynamic programming minimizes the number of positive 1..5-digit
// summands. Every sum has the digit sum's residue modulo nine, so states
// store (sum - digit sum)/9. Exact prefix/suffix extrema bound each row.
// Zero itself and all tokens beginning with zero are forbidden.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=100000000;
struct Edge { int next, q; Edge(int n,int v):next(n),q(v){} };
int main() {
    string line; int tc=0;
    while(cin>>line && line!="0=0") {
        cout<<++tc<<". ";
        int eq=line.find('='); string s=line.substr(0,eq), rhs=line.substr(eq+1);
        int n=s.size(), target=0, digits=0;
        for(int i=0;i<n;++i) digits+=s[i]-'0';
        for(size_t i=0;i<rhs.size();++i) target=min(INF,target*10+rhs[i]-'0');
        if(target<digits || (target-digits)%9) {cout<<"IMPOSSIBLE\n";continue;}
        int Q=(target-digits)/9;
        vector< vector<Edge> > e(n);
        for(int i=0;i<n;++i) if(s[i]!='0') {
            int v=0,d=0;
            for(int j=i;j<n && j<i+5;++j) {
                v=v*10+s[j]-'0'; d+=s[j]-'0';
                e[i].push_back(Edge(j+1,(v-d)/9));
            }
        }
        vector<int> pmin(n+1,INF),pmax(n+1,-1),smin(n+1,INF),smax(n+1,-1);
        pmin[0]=pmax[0]=smin[n]=smax[n]=0;
        for(int i=0;i<n;++i) if(pmax[i]>=0)
            for(size_t k=0;k<e[i].size();++k) {
                Edge a=e[i][k];
                pmin[a.next]=min(pmin[a.next],pmin[i]+a.q);
                pmax[a.next]=max(pmax[a.next],pmax[i]+a.q);
            }
        for(int i=n-1;i>=0;--i)
            for(size_t k=0;k<e[i].size();++k) {
                Edge a=e[i][k]; if(smax[a.next]<0) continue;
                smin[i]=min(smin[i],a.q+smin[a.next]);
                smax[i]=max(smax[i],a.q+smax[a.next]);
            }
        if(Q<smin[0] || Q>smax[0]) {cout<<"IMPOSSIBLE\n";continue;}
        vector<int> lo(n+1),hi(n+1);
        vector< vector<unsigned char> > dp(n+1);
        for(int i=0;i<=n;++i) {
            lo[i]=max(smin[i],Q-pmax[i]); hi[i]=min(smax[i],Q-pmin[i]);
            if(lo[i]<=hi[i]) dp[i].assign(hi[i]-lo[i]+1,255);
        }
        dp[n][0]=0;
        for(int i=n-1;i>=0;--i) {
            for(size_t k=0;k<e[i].size();++k) {
                Edge a=e[i][k]; int j=a.next;
                int l=max(lo[i],lo[j]+a.q),h=min(hi[i],hi[j]+a.q);
                for(int q=l;q<=h;++q) {
                    int z=dp[j][q-a.q-lo[j]];
                    if(z<254 && z+1<dp[i][q-lo[i]]) dp[i][q-lo[i]]=z+1;
                }
            }
        }
        if(dp[0][Q-lo[0]]==255) {cout<<"IMPOSSIBLE\n";continue;}
        int i=0,q=Q;
        while(i<n) {
            for(size_t k=0;k<e[i].size();++k) {
                Edge a=e[i][k]; int r=q-a.q;
                if(r>=lo[a.next] && r<=hi[a.next] &&
                   dp[i][q-lo[i]]==1+dp[a.next][r-lo[a.next]]) {
                    if(i) cout<<'+';
                    cout<<s.substr(i,a.next-i); i=a.next; q=r; break;
                }
            }
        }
        cout<<'='<<rhs<<'\n';
    }
}
