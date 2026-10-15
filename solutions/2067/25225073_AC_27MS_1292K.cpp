// POJ 2067 - Young, Poor and Busy
// Model: gpt-6.1-sol
// Time-expanded dynamic programming computes cheapest outward and return trips.
// Enumerate a city and a common 30-minute stay, including either hometown.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=100000000;
struct Train { int u,v,d,a,p; };
int f[2][601][105],g[2][601][105];
int minute(const string &s) {return (s[0]-'0')*600+(s[1]-'0')*60+(s[3]-'0')*10+s[4]-'0'-480;}
int city(map<string,int>& ids,const string &s) {
    if(ids.find(s)==ids.end()) {int n=ids.size(); ids[s]=n;}
    return ids[s];
}
int main() {
    int m;
    while(cin>>m && m) {
        map<string,int> ids; city(ids,"Hakodate"); city(ids,"Tokyo");
        vector<Train> dep[601],arr[601];
        for(int i=0;i<m;++i) {
            string u,v,d,a; Train e; cin>>u>>d>>v>>a>>e.p;
            e.u=city(ids,u); e.v=city(ids,v); e.d=minute(d); e.a=minute(a);
            if(e.d>=0 && e.a<=600) {dep[e.d].push_back(e);arr[e.a].push_back(e);}
        }
        int n=ids.size();
        for(int h=0;h<2;++h) {
            for(int t=0;t<=600;++t) for(int c=0;c<n;++c) f[h][t][c]=g[h][t][c]=INF;
            f[h][0][h]=0; g[h][600][h]=0;
            for(int t=0;t<=600;++t) {
                if(t) for(int c=0;c<n;++c) f[h][t][c]=min(f[h][t][c],f[h][t-1][c]);
                for(unsigned j=0;j<dep[t].size();++j) {
                    Train e=dep[t][j];
                    f[h][e.a][e.v]=min(f[h][e.a][e.v],f[h][t][e.u]+e.p);
                }
            }
            for(int t=600;t>=0;--t) {
                if(t<600) for(int c=0;c<n;++c) g[h][t][c]=min(g[h][t][c],g[h][t+1][c]);
                for(unsigned j=0;j<arr[t].size();++j) {
                    Train e=arr[t][j];
                    g[h][e.d][e.u]=min(g[h][e.d][e.u],g[h][t][e.v]+e.p);
                }
            }
        }
        int ans=INF;
        for(int c=0;c<n;++c) for(int t=0;t<=570;++t) {
            int cost=0; bool ok=true;
            for(int h=0;h<2;++h) {
                if(f[h][t][c]==INF || g[h][t+30][c]==INF) ok=false;
                cost+=f[h][t][c]+g[h][t+30][c];
            }
            if(ok) ans=min(ans,cost);
        }
        cout<<(ans==INF?0:ans)<<'\n';
    }
}
