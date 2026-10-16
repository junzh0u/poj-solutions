// POJ 3851 - Wormholes
// Model: gpt-6.1-sol
// Relax earliest arrival times at wormhole exits. A negative predecessor
// cycle is repeated directly to its creation-time floor, avoiding millions
// of individual trips. Entry before creation requires waiting until t.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct P { int x,y,z; };
int distanceP(P a,P b) {
    double x=a.x-b.x,y=a.y-b.y,z=a.z-b.z;
    return (int)ceil(sqrt(x*x+y*y+z*z));
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        P start,goal,s[50],e[50]; int n,t[50],d[50],w[50][50],a[50],p[50];
        scanf("%d%d%d%d%d%d",&start.x,&start.y,&start.z,&goal.x,&goal.y,&goal.z);
        scanf("%d",&n);
        for(int i=0;i<n;++i) {
            scanf("%d%d%d%d%d%d%d%d",&s[i].x,&s[i].y,&s[i].z,&e[i].x,&e[i].y,&e[i].z,&t[i],&d[i]);
            a[i]=max(distanceP(start,s[i]),t[i])+d[i];
        }
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) w[i][j]=distanceP(e[i],s[j])+d[j];
        for(;;) {
            for(int i=0;i<n;++i) p[i]=-1;
            int last=-1;
            for(int k=0;k<n;++k) {
                last=-1;
                for(int i=0;i<n;++i) for(int j=0;j<n;++j) {
                    int b=max(a[i]+w[i][j],t[j]+d[j]);
                    if(b<a[j]) { a[j]=b; p[j]=i; last=j; }
                }
                if(last<0) break;
            }
            if(last<0) break;
            int v=last;
            for(int k=0;k<n && v>=0;++k) v=p[v];
            if(v<0) continue;
            vector<int> cycle; int u=v;
            do { cycle.push_back(u); u=p[u]; } while(u!=v);
            reverse(cycle.begin(),cycle.end());
            int floor=-1000000000, sum=0;
            u=v;
            for(unsigned k=0;k<cycle.size();++k) {
                int j=cycle[k];
                floor=max(floor+w[u][j],t[j]+d[j]);
                sum+=w[u][j]; u=j;
            }
            if(sum<0) a[v]=min(a[v],floor);
        }
        int answer=distanceP(start,goal);
        for(int i=0;i<n;++i) answer=min(answer,a[i]+distanceP(e[i],goal));
        printf("%d\n",answer);
    }
}
