// POJ 2297 - Balanced Chemical Equations
// Model: gpt-6.1-sol
// Meet in the middle over coefficients 1..9, matching signed atom totals.
// Sort suffix totals with their digit codes; enumerate prefixes in lexical
// order to obtain the smallest complete coefficient sequence. Repeated atom
// occurrences in a formula are added together, not overwritten.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <cctype>
using namespace std;
int a[10][10], n, atoms, cut, prefixCode, suffixCode;
struct State { int s[10]; int code; };
vector<State> states;
bool lessState(const State& x, const State& y) {
    for(int j=0;j<atoms;++j) if(x.s[j]!=y.s[j]) return x.s[j]<y.s[j];
    return x.code<y.code;
}
void generate(int i, State cur) {
    if(i==n) { states.push_back(cur); return; }
    int oldCode=cur.code;
    for(int d=1;d<=9;++d) {
        for(int j=0;j<atoms;++j) cur.s[j]+=a[i][j];
        cur.code=oldCode*10+d;
        generate(i+1,cur);
    }
}
bool findPrefix(int i, State cur) {
    if(i==cut) {
        State target; memset(&target,0,sizeof(target));
        for(int j=0;j<atoms;++j) target.s[j]=-cur.s[j];
        vector<State>::iterator it=lower_bound(states.begin(),states.end(),target,lessState);
        if(it==states.end()) return false;
        for(int j=0;j<atoms;++j) if(it->s[j]!=target.s[j]) return false;
        prefixCode=cur.code; suffixCode=it->code; return true;
    }
    int oldCode=cur.code;
    for(int d=1;d<=9;++d) {
        for(int j=0;j<atoms;++j) cur.s[j]+=a[i][j];
        cur.code=oldCode*10+d;
        if(findPrefix(i+1,cur)) return true;
    }
    return false;
}
int main() {
    int t; cin>>t;
    while(t--) {
        string eq; cin>>eq;
        map<string,int> ids; memset(a,0,sizeof(a)); n=0; atoms=0;
        int sign=1;
        for(size_t p=0;p<eq.size();) {
            if(eq[p]=='+' || eq[p]=='=') {
                if(eq[p]=='=') sign=-1;
                ++n; ++p; continue;
            }
            string symbol(1,eq[p++]);
            if(p<eq.size() && islower(static_cast<unsigned char>(eq[p]))) symbol+=eq[p++];
            int count=1;
            if(p<eq.size() && isdigit(static_cast<unsigned char>(eq[p]))) count=eq[p++]-'0';
            if(ids.find(symbol)==ids.end()) ids[symbol]=atoms++;
            a[n][ids[symbol]]+=sign*count;
        }
        ++n; cut=n/2; states.clear();
        State zero; memset(&zero,0,sizeof(zero));
        generate(cut,zero); sort(states.begin(),states.end(),lessState);
        if(!findPrefix(0,zero)) cout<<"IMPOSSIBLE\n";
        else {
            int digits[10];
            for(int i=cut-1;i>=0;--i) { digits[i]=prefixCode%10; prefixCode/=10; }
            for(int i=n-1;i>=cut;--i) { digits[i]=suffixCode%10; suffixCode/=10; }
            for(int i=0;i<n;++i) { if(i) cout<<' '; cout<<digits[i]; }
            cout<<'\n';
        }
    }
    return 0;
}
