// POJ 3868 - Garbling Game
// Model: gpt-6.1-sol
// Simulate one sweep, then sum its write counts along permutation cycles.
// Decimal division handles n < 10^100 without a big-integer library.
// A number is recorded before rotating its 2 by 2 block.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const int MOD=100000;
string divide(const string &s,int d,int &rem) {
    string q; rem=0;
    for (int i=0;i<(int)s.size();++i) {
        rem=rem*10+s[i]-'0';
        int z=rem/d; rem%=d;
        if (z || !q.empty()) q+=char('0'+z);
    }
    return q.empty()?"0":q;
}
int main() {
    int r,c; string n;
    if (!(cin>>r>>c>>n)) return 0;
    vector<string> g(r-1);
    for(int i=0;i<r-1;++i) cin>>g[i];
    int tail; string q=divide(n,(r-1)*(c-1),tail);
    int sz=r*c;
    vector<int> a(sz),w(sz,0),v(sz,0),p(sz),ans(sz),seen(sz,0);
    for(int i=0;i<sz;++i) a[i]=i;
    int turn=0;
    for(int i=0;i<r-1;++i) for(int j=0;j<c-1;++j,++turn) {
        int x=i*c+j,y=x+1,z=x+c+1,t=x+c;
        ++w[a[x]]; if(turn<tail) ++v[a[x]];
        int h=a[x];
        if(g[i][j]=='R') {a[x]=a[t];a[t]=a[z];a[z]=a[y];a[y]=h;}
        if(g[i][j]=='L') {a[x]=a[y];a[y]=a[z];a[z]=a[t];a[t]=h;}
    }
    for(int i=0;i<sz;++i) p[a[i]]=i;
    for(int start=0;start<sz;++start) if(!seen[start]) {
        vector<int> cyc; int x=start;
        do {seen[x]=1;cyc.push_back(x);x=p[x];} while(x!=start);
        int len=cyc.size(),k; string laps=divide(q,len,k);
        int lapmod=0;
        for(int j=0;j<(int)laps.size();++j) lapmod=(lapmod*10+laps[j]-'0')%MOD;
        int total=0,window=0;
        for(int j=0;j<len;++j) total+=w[cyc[j]];
        for(int j=0;j<k;++j) window+=w[cyc[j]];
        int base=((lapmod/1000*total%MOD)*1000+lapmod%1000*total)%MOD;
        for(int j=0;j<len;++j) {
            ans[cyc[j]]=(base+window+v[cyc[(j+k)%len]])%MOD;
            window+=w[cyc[(j+k)%len]]-w[cyc[j]];
        }
    }
    for(int i=0;i<sz;++i) cout<<ans[i]<<'\n';
    return 0;
}
