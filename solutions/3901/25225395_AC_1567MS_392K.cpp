// POJ 3901 - The Computer Game
// Model: gpt-6.1-sol
// Sweep diagonals x+y, retaining the connectivity partition of at most
// seven frontier points. A component may never leave the frontier while
// another point is still to be chosen. Count each connected set at its
// last nonempty diagonal. Points outside the strict diamond are irrelevant.
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstdlib>
using namespace std;
struct Big {
    int d[3];
    Big(int x=0) { d[0]=x; d[1]=d[2]=0; }
    void operator+=(const Big& b) {
        int carry=0;
        for(int i=0;i<3;++i) { int x=d[i]+b.d[i]+carry; carry=x>=1000000000; d[i]=x-carry*1000000000; }
    }
};
ostream& operator<<(ostream& o,const Big& b) {
    int i=2; while(i>0 && !b.d[i]) --i;
    o<<b.d[i]; while(i--) o<<setw(9)<<setfill('0')<<b.d[i];
    return o;
}
int p[16];
int root(int x) { return p[x]==x?x:p[x]=root(p[x]); }
void join(int a,int b) { p[root(a)]=root(b); }
int main() {
    int T; cin>>T;
    while(T--) {
        int n,m; cin>>n>>m;
        bool blocked[15][15]={{false}};
        for(int i=0;i<m;++i) { int x,y; cin>>x>>y; blocked[x+7][y+7]=true; }
        map<int,Big> dp; dp[0]=Big(1);
        vector<int> old;
        Big answer;
        for(int u=1-n;u<n;++u) {
            vector<int> now;
            int freeMask=0;
            for(int v=1-n;v<n;++v) if((u-v)%2==0) {
                int x=(u+v)/2, y=(u-v)/2;
                if(!blocked[x+7][y+7]) freeMask|=1<<now.size();
                now.push_back(v);
            }
            map<int,Big> next;
            for(map<int,Big>::iterator it=dp.begin();it!=dp.end();++it) {
                int labels[7]={0}, comps=0;
                for(int i=0;i<(int)old.size();++i) { labels[i]=(it->first>>(3*i))&7; if(labels[i]>comps) comps=labels[i]; }
                for(int mask=freeMask;;mask=(mask-1)&freeMask) {
                    for(int i=0;i<16;++i) p[i]=i;
                    bool touched[8]={false};
                    for(int j=0;j<(int)now.size();++j) if(mask&(1<<j))
                        for(int i=0;i<(int)old.size();++i) if(labels[i] && abs(now[j]-old[i])==1) {
                            join(labels[i]-1,7+j); touched[labels[i]]=true;
                        }
                    bool ok=true;
                    for(int i=1;i<=comps;++i) if(!touched[i]) ok=false;
                    if(ok) {
                        int names[16]={0}, count=0, code=0;
                        for(int j=0;j<(int)now.size();++j) if(mask&(1<<j)) {
                            int r=root(7+j);
                            if(!names[r]) names[r]=++count;
                            code|=names[r]<<(3*j);
                        }
                        next[code]+=it->second;
                        if(count==1) answer+=it->second;
                    }
                    if(mask==0) break;
                }
            }
            dp.swap(next); old=now;
        }
        cout<<answer<<'\n';
    }
}
