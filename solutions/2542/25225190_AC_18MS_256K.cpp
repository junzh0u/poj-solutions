// POJ 2542 - Fool Game
// Model: gpt-6.1-sol
// Enumerate the ranks that may appear on the table. A matching covering every
// attacking card of those ranks, using defending cards of the same rank set,
// gives a fixed defense against every attack order. Conversely, a completed
// defense supplies exactly such a matching for its final table ranks.
// Both players choose optimally; ties use rank, then h, s, d, c.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int a[18], b[18], n, trump, mask, mate[18];
bool edge[18][18], seen[18];
bool augment(int u) {
    for (int v=0;v<n;++v) if ((mask&(1<<(b[v]/4))) && edge[u][v] && !seen[v]) {
        seen[v]=true;
        if (mate[v]<0 || augment(mate[v])) { mate[v]=u; return true; }
    }
    return false;
}
int main() {
    string t,x,y, ranks="67890JQKA", suits="hsdc";
    if (!(cin>>t>>x>>y)) return 0;
    trump=(int)suits.find(t[0]); n=(int)x.size()/2;
    for(int i=0;i<n;++i) {
        a[i]=(int)ranks.find(x[2*i])*4+(int)suits.find(x[2*i+1]);
        b[i]=(int)ranks.find(y[2*i])*4+(int)suits.find(y[2*i+1]);
    }
    for(int i=0;i<n;++i) for(int j=0;j<n;++j)
        edge[i][j]=(a[i]%4==b[j]%4 && a[i]/4<b[j]/4) || (b[j]%4==trump && a[i]%4!=trump);
    int safe=0;
    for(mask=1;mask<512;++mask) {
        memset(mate,-1,sizeof(mate)); bool ok=true;
        for(int i=0;i<n && ok;++i) if(mask&(1<<(a[i]/4))) {
            memset(seen,0,sizeof(seen)); ok=augment(i);
        }
        if(ok) safe|=mask;
    }
    int best=36;
    for(int i=0;i<n;++i) if(!(safe&(1<<(a[i]/4))) && a[i]<best) best=a[i];
    if(best==36) cout<<"NO\n";
    else cout<<ranks[best/4]<<suits[best%4]<<'\n';
    return 0;
}
