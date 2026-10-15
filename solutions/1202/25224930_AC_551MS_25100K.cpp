// POJ 1202 - Family
// Model: gpt-6.1-sol
// Process monsters in topological order. For an earlier monster x and a
// later child y, relationship(x,y) is the mean of its parents' relationships.
// A monster is always identical to itself; unrelated founders have value zero.
// Exact percentages use scale 10^600: each recurrence path has fewer than
// 2*n halvings, so no precision is lost. Arbitrary input numbering is allowed.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
const int L=68, BASE=1000000000;
struct Big { int d[L]; };
Big dp[301][301];
int pa[301], pb[301], seen[301];
vector<int> order;
void visit(int x) {
    if(seen[x]) return;
    seen[x]=1;
    if(pa[x]) { visit(pa[x]); visit(pb[x]); }
    order.push_back(x);
}
void average(Big &r, const Big &a, const Big &b) {
    int carry=0;
    for(int i=0;i<L;++i) {
        int t=a.d[i]+b.d[i]+carry;
        r.d[i]=t%BASE; carry=t/BASE;
    }
    carry=0;
    for(int i=L-1;i>=0;--i) {
        int t=r.d[i]+carry*BASE;
        r.d[i]=t/2; carry=t%2;
    }
}
string output(const Big &v) {
    int h=L-1; while(h>0 && v.d[h]==0) --h;
    char buf[20]; sprintf(buf,"%d",v.d[h]); string s=buf;
    for(int i=h-1;i>=0;--i) { sprintf(buf,"%09d",v.d[i]); s+=buf; }
    if(s.size()<=600) s=string(601-s.size(),'0')+s;
    s.insert(s.size()-600,".");
    while(s[s.size()-1]=='0') s.erase(s.size()-1);
    if(s[s.size()-1]=='.') s.erase(s.size()-1);
    return s+"%";
}
int main() {
    int n,k; if(scanf("%d%d",&n,&k)!=2) return 0;
    for(int i=0;i<k;++i) { int x,a,b; scanf("%d%d%d",&x,&a,&b); pa[x]=a; pb[x]=b; }
    for(int i=1;i<=n;++i) visit(i);
    for(int i=0;i<n;++i) {
        int y=order[i]; dp[y][y].d[66]=100000000;
        if(!pa[y]) continue;
        for(int j=0;j<i;++j) {
            int x=order[j];
            average(dp[x][y],dp[x][pa[y]],dp[x][pb[y]]);
            dp[y][x]=dp[x][y];
        }
    }
    int m; scanf("%d",&m);
    while(m--) { int a,b; scanf("%d%d",&a,&b); puts(output(dp[a][b]).c_str()); }
    return 0;
}
