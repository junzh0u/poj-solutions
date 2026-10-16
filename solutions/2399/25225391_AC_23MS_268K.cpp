// POJ 2399 - A Mayor Problem
// Model: gpt-6.1-sol
// Floyd-Warshall distances identify every directed edge on any shortest
// route between two listed places. Opposite directions share one block.
// The first street string indexes rows; the second indexes columns.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int u,v,b; Edge(int a,int c,int d):u(a),v(c),b(d){} };
int main() {
    int cases; cin >> cases;
    for(int tc=1;tc<=cases;++tc) {
        string ew,ns; cin >> ew >> ns;
        int r=ew.size(),c=ns.size(),n=r*c,blocks=0;
        vector<Edge> edges;
        for(int i=0;i<r;++i) for(int j=0;j<c;++j) {
            int u=i*c+j;
            if(j+1<c) {
                if(ew[i]!='W') edges.push_back(Edge(u,u+1,blocks));
                if(ew[i]!='E') edges.push_back(Edge(u+1,u,blocks));
                ++blocks;
            }
            if(i+1<r) {
                if(ns[j]!='N') edges.push_back(Edge(u,u+c,blocks));
                if(ns[j]!='S') edges.push_back(Edge(u+c,u,blocks));
                ++blocks;
            }
        }
        const int INF=100000;
        vector< vector<int> > d(n,vector<int>(n,INF));
        for(int i=0;i<n;++i) d[i][i]=0;
        for(size_t k=0;k<edges.size();++k) d[edges[k].u][edges[k].v]=1;
        for(int k=0;k<n;++k) for(int i=0;i<n;++i) for(int j=0;j<n;++j)
            d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
        int count; cin >> count;
        vector<int> places;
        for(int i=0;i<count;++i) { int x,y; cin >> x >> y; places.push_back(x*c+y); }
        sort(places.begin(),places.end());
        places.erase(unique(places.begin(),places.end()),places.end());
        vector<bool> paint(blocks,false);
        for(size_t e=0;e<edges.size();++e) {
            int u=edges[e].u,v=edges[e].v;
            for(size_t a=0;a<places.size() && !paint[edges[e].b];++a)
                for(size_t b=0;b<places.size();++b) {
                    int s=places[a],t=places[b];
                    if(d[s][t]<INF && d[s][u]+1+d[v][t]==d[s][t]) {
                        paint[edges[e].b]=true; break;
                    }
                }
        }
        int answer=0;
        for(int b=0;b<blocks;++b) if(paint[b]) ++answer;
        cout << "Case #" << tc << ":\n" << answer << " street blocks need to be painted.\n\n";
    }
}
