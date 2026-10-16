// POJ 1534 - Terrorist Attack
// Model: gpt-6.1-sol
// Sort all plane-to-tower crash times and slide a window whose edges
// admit a matching of d distinct planes and towers. Maintain the matching
// with augmenting paths after each edge change. Flights are redirected
// immediately from their departure airport; destination is irrelevant.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;
struct Point { double x,y; };
struct Edge { int a,b; double t; bool operator<(const Edge& e)const{return t<e.t;} };
bool active[50][90],seen[50];
int mt[50],mp[90],K,P;
bool dfs(int a) {
    if(seen[a]) return false;
    seen[a]=true;
    for(int b=0;b<P;++b) if(active[a][b]) {
        if(mp[b]<0 || dfs(mp[b])) { mt[a]=b; mp[b]=a; return true; }
    }
    return false;
}
bool augment() {
    memset(seen,0,sizeof(seen));
    for(int a=0;a<K;++a) if(mt[a]<0 && dfs(a)) return true;
    return false;
}
int main(){
    int n,d;
    while(scanf("%d%d%d%d",&n,&K,&P,&d)==4 && n+K+P+d){
        Point airport[50],tower[50];
        for(int i=0;i<n;++i) scanf("%lf%lf",&airport[i].x,&airport[i].y);
        for(int i=0;i<K;++i) scanf("%lf%lf",&tower[i].x,&tower[i].y);
        vector<Edge> e;
        for(int b=0;b<P;++b){
            int h,m,f,t; double s;
            scanf("%d%d%d%d%lf",&h,&m,&f,&t,&s); --f;
            for(int a=0;a<K;++a){
                double dx=airport[f].x-tower[a].x,dy=airport[f].y-tower[a].y;
                Edge z; z.a=a; z.b=b; z.t=(h*60.0+m)*60.0+sqrt(dx*dx+dy*dy)/s;
                e.push_back(z);
            }
        }
        if(d>K || d>P){puts("Impossible!");continue;}
        if(d<=1){puts("0:0");continue;}
        sort(e.begin(),e.end());
        memset(active,0,sizeof(active)); memset(mt,-1,sizeof(mt)); memset(mp,-1,sizeof(mp));
        int left=0,count=0; double best=1e100;
        for(int r=0;r<(int)e.size();++r){
            active[e[r].a][e[r].b]=true;
            if(count<d && augment()) ++count;
            while(count>=d){
                best=min(best,e[r].t-e[left].t);
                int a=e[left].a,b=e[left].b;
                active[a][b]=false; ++left;
                if(mt[a]==b){mt[a]=-1;mp[b]=-1;--count;if(augment())++count;}
            }
        }
        int minutes=(int)(best/60.0+0.5);
        printf("%d:%d\n",minutes/60,minutes%60);
    }
}
