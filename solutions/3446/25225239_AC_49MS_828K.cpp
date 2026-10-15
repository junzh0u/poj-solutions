// POJ 3446 - Catch the Bus!
// Model: gpt-6.1-sol
// Time-dependent Dijkstra on stops and individual route occurrences.
// Staying on a bus costs only travel time; boarding after an arrival
// requires two minutes. Initial boarding and meeting need no extra time.
// Repeated visits to the same stop remain distinct route occurrences.
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <functional>
#include <algorithm>
#include <cstdio>
using namespace std;
const int INF=1000000000;
struct Occ { int stop,route,offset,next,cost; };
struct Route { int wait[60]; };
map<string,int> ids;
vector<vector<int> > at;
vector<Occ> occ;
vector<Route> routes;
int id(const string &s) {
    map<string,int>::iterator it=ids.find(s);
    if(it!=ids.end()) return it->second;
    int k=(int)ids.size(); ids[s]=k; at.push_back(vector<int>()); return k;
}
int board(int k,int t) {
    Occ &o=occ[k]; int w=routes[o.route].wait[(t-o.offset%60+60)%60];
    return w==INF?INF:t+w;
}
vector<int> solve(int start,int tm) {
    int n=(int)at.size(), m=(int)occ.size();
    vector<int> d(n+m,INF);
    typedef pair<int,int> P;
    priority_queue<P,vector<P>,greater<P> > q;
    d[start]=tm; q.push(P(tm,start));
    for(size_t j=0;j<at[start].size();++j) {
        int k=at[start][j], t=board(k,tm);
        if(t<d[n+k]) { d[n+k]=t; q.push(P(t,n+k)); }
    }
    while(!q.empty()) {
        int t=q.top().first, v=q.top().second; q.pop();
        if(t!=d[v]) continue;
        if(v<n) {
            for(size_t j=0;j<at[v].size();++j) {
                int k=at[v][j], nt=board(k,t+2);
                if(nt<d[n+k]) { d[n+k]=nt; q.push(P(nt,n+k)); }
            }
        } else {
            Occ &o=occ[v-n];
            if(t<d[o.stop]) { d[o.stop]=t; q.push(P(t,o.stop)); }
            if(o.next>=0 && t+o.cost<d[n+o.next]) {
                d[n+o.next]=t+o.cost; q.push(P(t+o.cost,n+o.next));
            }
        }
    }
    d.resize(n); return d;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int l;
    while(cin>>l && l>=0) {
        ids.clear(); at.clear(); occ.clear(); routes.clear();
        for(int r=0;r<l;++r) {
            string s; cin>>s; int offset=0,prev=-1;
            while(true) {
                Occ o; o.stop=id(s); o.route=r; o.offset=offset; o.next=-1; o.cost=0;
                int k=(int)occ.size(); occ.push_back(o); at[o.stop].push_back(k);
                if(prev>=0) occ[prev].next=k;
                int c; cin>>c; if(c<0) break;
                occ[k].cost=c; offset+=c; prev=k; cin>>s;
            }
            int h; cin>>h; bool dep[60]={false};
            for(int i=0;i<h;++i) { int x; cin>>x; dep[x]=true; }
            Route rt;
            for(int i=0;i<60;++i) {
                rt.wait[i]=INF;
                for(int w=0;w<60;++w) if(dep[(i+w)%60]) { rt.wait[i]=w; break; }
            }
            routes.push_back(rt);
        }
        int st[2], tm[2];
        for(int i=0;i<2;++i) {
            string t,s; cin>>t>>s; int h,m; sscanf(t.c_str(),"%d:%d",&h,&m);
            tm[i]=h*60+m; st[i]=id(s);
        }
        vector<int> a=solve(st[0],tm[0]), b=solve(st[1],tm[1]);
        int ans=INF;
        for(size_t i=0;i<a.size();++i) ans=min(ans,max(a[i],b[i]));
        if(ans==INF) cout<<"No connection\n";
        else { ans%=1440; cout<<ans/60<<':'<<(ans%60<10?"0":"")<<ans%60<<'\n'; }
    }
}
