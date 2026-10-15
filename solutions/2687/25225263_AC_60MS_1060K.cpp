// POJ 2687 - Earth Observation with a Mobile Robot Team
// Model: gpt-6.1-sol
// Earliest information-arrival Dijkstra; solve each pair's contact quadratic
// on integer-time linear pieces. Waiting preserves information, making each
// temporal edge FIFO. Equal arrival times permit instantaneous multihop relays.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
int x[100][1001],y[100][1001],n,T,R;
double meet(int u,int v,double start) {
    int first=(int)floor(start);
    if(first>T) return 1e30;
    if(first==T) {
        double dx=x[u][T]-x[v][T],dy=y[u][T]-y[v][T];
        return dx*dx+dy*dy<=R*R+1e-8 ? start:1e30;
    }
    for(int t=first;t<T;++t) {
        double dx=x[u][t]-x[v][t],dy=y[u][t]-y[v][t];
        double vx=x[u][t+1]-x[u][t]-x[v][t+1]+x[v][t];
        double vy=y[u][t+1]-y[u][t]-y[v][t+1]+y[v][t];
        double lo=max(0.0,start-t),px=dx+vx*lo,py=dy+vy*lo;
        if(px*px+py*py<=R*R+1e-8) return t+lo;
        double a=vx*vx+vy*vy,b=dx*vx+dy*vy,c=dx*dx+dy*dy-R*R;
        double disc=b*b-a*c;
        if(a==0 || disc<0) continue;
        double entry=(-b-sqrt(disc))/a;
        if(entry>=lo-1e-10 && entry<=1+1e-10)
            return t+max(lo,min(1.0,entry));
    }
    return 1e30;
}
int main() {
    while(cin>>n>>T>>R && n) {
        string name[100];
        for(int i=0;i<n;++i) {
            cin>>name[i];
            int t,xx,yy; cin>>t>>xx>>yy;
            x[i][0]=xx;y[i][0]=yy;
            while(t<T) {
                int end,vx,vy;cin>>end>>vx>>vy;
                while(t<end) {x[i][t+1]=x[i][t]+vx;y[i][t+1]=y[i][t]+vy;++t;}
            }
        }
        double d[100];bool done[100]={false};
        for(int i=0;i<n;++i)d[i]=1e30;
        d[0]=0;
        for(int k=0;k<n;++k) {
            int u=-1;
            for(int i=0;i<n;++i)if(!done[i] && (u<0 || d[i]<d[u]))u=i;
            if(u<0 || d[u]>T+1e-9) break;
            done[u]=true;
            for(int v=0;v<n;++v)if(!done[v]) d[v]=min(d[v],meet(u,v,d[u]));
        }
        vector<string> ans;
        for(int i=0;i<n;++i)if(d[i]<=T+1e-9)ans.push_back(name[i]);
        sort(ans.begin(),ans.end());
        for(size_t i=0;i<ans.size();++i)cout<<ans[i]<<'\n';
    }
}
