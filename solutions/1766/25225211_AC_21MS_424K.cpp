// POJ 1766 - Football
// Model: gpt-6.1-sol
// Construct the truncated icosahedron as an oriented face map. Follow the
// boundary with green on the left, then flood its faces across uncut edges.
// The given final hexagon/hexagon edge fixes a starting edge up to symmetry.
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
vector<int> face[32], adj[144];
int leftFace[144][144], nextVertex[144][144];
bool cutEdge[144][144], seen[32];
int pentNext[12][12];
void triangle(int u,int v,int w,int f) {
    int a[6]={u*12+v,v*12+u,v*12+w,w*12+v,w*12+u,u*12+w};
    for(int i=0;i<6;++i) face[f].push_back(a[i]);
    pentNext[u][v]=w; pentNext[v][w]=u; pentNext[w][u]=v;
}
void flood(int f) {
    if(seen[f]) return;
    seen[f]=true;
    for(int i=0;i<(int)face[f].size();++i) {
        int u=face[f][i],v=face[f][(i+1)%face[f].size()];
        if(!cutEdge[u][v]) flood(leftFace[v][u]);
    }
}
int main() {
    memset(pentNext,-1,sizeof(pentNext));
    int f=12;
    for(int i=0;i<5;++i) {
        int j=(i+1)%5;
        triangle(0,1+i,1+j,f++);
        triangle(1+i,6+i,1+j,f++);
        triangle(1+j,6+i,6+j,f++);
        triangle(11,6+j,6+i,f++);
    }
    for(int u=0;u<12;++u) {
        int v=0; while(pentNext[u][v]<0) ++v;
        int start=v;
        do { face[u].push_back(u*12+v); v=pentNext[u][v]; } while(v!=start);
    }
    for(f=0;f<32;++f) for(int i=0;i<(int)face[f].size();++i) {
        int u=face[f][i],v=face[f][(i+1)%face[f].size()];
        leftFace[u][v]=f;
        nextVertex[u][v]=face[f][(i+2)%face[f].size()];
        adj[u].push_back(v);
    }
    int n; if(!(cin>>n)) return 0;
    int u=12,v=1; // The directed edge (1,0) -> (0,1).
    int green=leftFace[u][v];
    for(int i=0;i<n;++i) {
        int a; cin>>a;
        cutEdge[u][v]=cutEdge[v][u]=true;
        int w=nextVertex[u][v];
        if(a==2) for(int j=0;j<3;++j)
            if(adj[v][j]!=u && adj[v][j]!=nextVertex[u][v]) w=adj[v][j];
        u=v; v=w;
    }
    flood(green);
    int b=12,w=20,g=0;
    for(f=0;f<32;++f) if(seen[f]) { ++g; if(f<12) --b; else --w; }
    cout<<b<<' '<<w<<' '<<g<<'\n';
}
