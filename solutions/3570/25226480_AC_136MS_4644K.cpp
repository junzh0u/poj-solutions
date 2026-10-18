// POJ 3570 - Fund Management
// Model: gpt-6-astra
// Enumerate bounded portfolios and maximize cash after each day.
// One transition is HOLD or one whole-lot trade; retain actions to reconstruct.
// Decimal amounts are parsed as integer cents, represented exactly by double.
// All reachable balances are below 2^53, so comparisons need no epsilon.
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
struct State { int a[8], total, code; };
vector<State> states;
map<int,int> ids;
int n,k,cap[8],power9[8];
void generate(int i, State s) {
    if(i==n) { ids[s.code]=(int)states.size(); states.push_back(s); return; }
    for(int q=0;q<=cap[i] && s.total+q<=k;++q) {
        State t=s; t.a[i]=q; t.total+=q; t.code+=q*power9[i]; generate(i+1,t);
    }
}
double cents(const string &s) {
    double v=0; size_t p=s.find('.');
    size_t end=(p==string::npos?s.size():p);
    for(size_t i=0;i<end;++i) v=v*10+s[i]-'0';
    v*=100;
    if(p!=string::npos) {
        if(p+1<s.size()) v+=(s[p+1]-'0')*10;
        if(p+2<s.size()) v+=s[p+2]-'0';
    }
    return v;
}
int main() {
    string s,name[8]; int days;
    if(!(cin>>s>>days>>n>>k)) return 0;
    double initial=cents(s),price[100][8];
    for(int i=0;i<n;++i) {
        int lot; cin>>name[i]>>lot>>cap[i];
        for(int d=0;d<days;++d) { cin>>s; price[d][i]=cents(s)*lot; }
        power9[i]=(i?power9[i-1]*9:1);
    }
    State zero={ {0},0,0 }; generate(0,zero);
    int count=(int)states.size();
    vector< vector<int> > buy(count,vector<int>(n,-1)),sell=buy;
    for(int j=0;j<count;++j) for(int i=0;i<n;++i) {
        if(states[j].a[i]<cap[i] && states[j].total<k) buy[j][i]=ids[states[j].code+power9[i]];
        if(states[j].a[i]) sell[j][i]=ids[states[j].code-power9[i]];
    }
    vector<double> dp(count,-1),next(count,-1); dp[0]=initial;
    vector< vector<signed char> > action(days,vector<signed char>(count,0));
    for(int d=0;d<days;++d) {
        next=dp;
        for(int j=0;j<count;++j) if(dp[j]>=0) for(int i=0;i<n;++i) {
            int dest=buy[j][i]; double v=dp[j]-price[d][i];
            if(dest>=0 && v>=0 && v>next[dest]) { next[dest]=v; action[d][dest]=(signed char)(i+1); }
            dest=sell[j][i]; v=dp[j]+price[d][i];
            if(dest>=0 && v>next[dest]) { next[dest]=v; action[d][dest]=(signed char)(-i-1); }
        }
        dp.swap(next);
    }
    printf("%.2f\n",dp[0]/100);
    vector<int> plan(days); int current=0;
    for(int d=days-1;d>=0;--d) {
        int a=action[d][current]; plan[d]=a;
        if(a>0) current=sell[current][a-1];
        if(a<0) current=buy[current][-a-1];
    }
    for(int d=0;d<days;++d) {
        if(plan[d]>0) cout<<"BUY "<<name[plan[d]-1]<<'\n';
        else if(plan[d]<0) cout<<"SELL "<<name[-plan[d]-1]<<'\n';
        else cout<<"HOLD\n";
    }
    return 0;
}
