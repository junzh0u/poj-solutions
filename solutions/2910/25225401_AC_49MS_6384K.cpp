// POJ 2910 - Downpayment
// Model: gpt-6.1-sol
// Keep the least debt for each alternative and remaining binding time.
// Rates are the actual monthly rates, even inside a binding period.
// All monetary arithmetic uses cents; interest is truncated before payment.
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long Int;
Int cents(const string &s) {
    Int v=0; size_t p=s.find('.');
    string a=s.substr(0,p); for(size_t i=0;i<a.size();++i) v=v*10+a[i]-'0';
    v*=100;
    if(p!=string::npos) { if(p+1<s.size()) v+=(s[p+1]-'0')*10; if(p+2<s.size()) v+=s[p+2]-'0'; }
    return v;
}
struct Rate { Int n,d; Rate():n(0),d(100){} };
Rate rate(const string &s) {
    Rate r; bool dot=false;
    for(size_t i=0;i<s.size();++i) { if(s[i]=='.') dot=true; else {r.n=r.n*10+s[i]-'0'; if(dot) r.d*=10;} }
    return r;
}
int main() {
    int cases; cin>>cases;
    for(int tc=1;tc<=cases;++tc) {
        int m; string s; cin>>m>>s; Int x=cents(s); cin>>s; Int pay=cents(s);
        vector<int> len(m); for(int i=0;i<m;++i) cin>>len[i];
        vector< vector<Int> > cost(m,vector<Int>(m));
        for(int i=0;i<m;++i) for(int j=0;j<m;++j) {cin>>s; cost[i][j]=cents(s);}
        int t; cin>>t; vector< vector<Rate> > rates(t,vector<Rate>(m));
        for(int i=0;i<t;++i) for(int j=0;j<m;++j) {cin>>s; rates[i][j]=rate(s);}
        int states=m*60, horizon=min(t,1200); const Int INF=2000000000000000000LL;
        vector<Int> old(states,INF), next(states,INF);
        vector< vector<int> > prev(horizon+1,vector<int>(states,-2));
        int finish=0, finalAlt=-1, finalPrev=-1; Int finalPay=INF;
        for(int month=1;month<=horizon && !finish;++month) {
            fill(next.begin(),next.end(),INF);
            for(int u=-1;u<states;++u) {
                if(u==-1 && month!=1) continue;
                Int debt=(u==-1?x:old[u]); if(debt==INF) continue;
                int a=(u==-1?-1:u/60), rem=(u==-1?0:u%60);
                for(int b=0;b<m;++b) {
                    if(rem>0 && b!=a) continue;
                    int nr=rem>0?rem-1:len[b]-1;
                    Int z=debt+(u!=-1 && rem==0?cost[a][b]:0);
                    Rate r=rates[month-1][b];
                    // A debt above all remaining payments cannot be repaid.
                    if(z>(1201-month)*pay) continue;
                    if(r.n && z>(INF-z)/r.n) continue;
                    z+=z*r.n/r.d;
                    if(z<=pay) {
                        if(z<finalPay) {finish=month; finalPay=z; finalAlt=b; finalPrev=u;}
                    } else {
                        z-=pay; int v=b*60+nr;
                        if(z<next[v]) {next[v]=z; prev[month][v]=u;}
                    }
                }
            }
            old.swap(next);
        }
        vector<int> plan(finish); plan[finish-1]=finalAlt;
        int u=finalPrev;
        for(int month=finish-1;month>=1;--month) {plan[month-1]=u/60; u=prev[month][u];}
        cout<<"Test case "<<tc<<'\n';
        for(int i=0;i<finish;++i) cout<<"Month "<<i+1<<": Alternative "<<plan[i]+1<<'\n';
        Int total=(finish-1)*pay+finalPay;
        cout<<"Total: "<<total/100<<'.'; if(total%100<10) cout<<'0'; cout<<total%100<<'\n';
    }
}
