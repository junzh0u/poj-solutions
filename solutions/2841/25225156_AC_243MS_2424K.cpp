// POJ 2841 - Navigation Game
// Model: gpt-6.1-sol
// Row DP with two monotone convex hull sweeps, separated by wheel parity.
// Horizontal moves cannot reverse without revisiting a cell. Costs depend
// on the destination; blessing and storm cells still extend the move streak.
// Process from bottom to top; boundary dots are impassable land.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
typedef long long LL;
const LL INF = 1000000000000000LL;
struct Line { LL m,b; Line(LL x=0,LL y=0):m(x),b(y){} LL at(LL x)const{return m*x+b;} };
struct Hull {
    vector<Line> q; int h;
    Hull():h(0){}
    void clear(){q.clear();h=0;}
    void add(Line c){
        while((int)q.size()-h>=2){
            Line a=q[q.size()-2],b=q.back();
            if((b.b-a.b)*(b.m-c.m)>=(c.b-b.b)*(a.m-b.m))q.pop_back();
            else break;
        }
        q.push_back(c);
    }
    LL get(LL x){
        if(h==(int)q.size())return INF;
        while(h+1<(int)q.size() && q[h+1].at(x)<=q[h].at(x))++h;
        return q[h].at(x);
    }
};
int weight(char c){return c=='B'?0:c=='S'?2:1;}
int main(){
    int n,m;
    if(!(cin>>n>>m))return 0;
    vector<string> g(n);for(int i=0;i<n;++i)cin>>g[i];
    vector<vector<LL> > prev(m,vector<LL>(2,INF));
    for(int j=0;j<m;++j)if(g[n-1][j]=='H')prev[j][0]=0;
    for(int r=n-2;r>=0;--r){
        vector<vector<LL> > entry(m,vector<LL>(2,INF)),cur=entry;
        for(int j=0;j<m;++j){
            char c=g[r][j];if(c=='O'||(r==0&&c!='H'))continue;
            for(int p=0;p<2;++p)if(prev[j][p]<INF)
                entry[j][p^(c=='F')]=prev[j][p]+weight(c);
        }
        for(int dir=0;dir<2;++dir){
            Hull hull[2];LL a=0,b=0;int parity=0;
            for(int t=0;t<m;++t){
                int j=dir?m-1-t:t;char c=g[r][j];
                if(c=='O'||(r==0&&c!='H')){hull[0].clear();hull[1].clear();a=b=0;parity=0;continue;}
                a+=weight(c);b+=(LL)t*weight(c);parity^=(c=='F');
                for(int p=0;p<2;++p)if(entry[j][p]<INF)
                    hull[p^parity].add(Line(-t,entry[j][p]-b+(LL)t*a));
                for(int p=0;p<2;++p){LL v=hull[p^parity].get(a);if(v<INF)cur[j][p]=min(cur[j][p],v+b);}
            }
        }
        prev.swap(cur);
    }
    LL ans=INF;for(int j=0;j<m;++j)if(g[0][j]=='H')ans=min(ans,prev[j][1]);
    if(ans==INF)cout<<"No solution\n";else cout<<ans<<'\n';
    return 0;
}
