// POJ 2787 - UFO Cubes in Roswell
// Model: gpt-6.1-sol
// Sort mirrors along each axis to link consecutive mirrors, then trace each
// occupied sunlight column. Reversibility makes all traced directed states
// disjoint. The diagrams define opposite/same-sign coordinate swaps for even/
// odd orientations; faces are x-:3, x+:2, y-:5, y+:4, z-:1, z+:0.
#include <cstdio>
#include <algorithm>
using namespace std;
struct Mirror { int c[3], next[6], type; };
Mirror *m;
int axis;
bool lessMirror(int a,int b) {
    int p=(axis+1)%3,q=(axis+2)%3;
    if(m[a].c[p]!=m[b].c[p]) return m[a].c[p]<m[b].c[p];
    if(m[a].c[q]!=m[b].c[q]) return m[a].c[q]<m[b].c[q];
    return m[a].c[axis]<m[b].c[axis];
}
int main() {
    int s,n;
    if(scanf("%d%d",&s,&n)!=2) return 0;
    m=new Mirror[n]; int *order=new int[n];
    for(int i=0;i<n;++i) {
        scanf("%d%d%d%d",&m[i].c[0],&m[i].c[1],&m[i].c[2],&m[i].type);
        order[i]=i;
        for(int d=0;d<6;++d) m[i].next[d]=-1;
    }
    for(axis=0;axis<3;++axis) {
        sort(order,order+n,lessMirror);
        int p=(axis+1)%3,q=(axis+2)%3;
        for(int i=1;i<n;++i) {
            int a=order[i-1],b=order[i];
            if(m[a].c[p]==m[b].c[p] && m[a].c[q]==m[b].c[q]) {
                m[a].next[2*axis+1]=b; m[b].next[2*axis]=a;
            }
        }
    }
    int rays[6]={0,0,0,0,0,0},turns[6]={0,0,0,0,0,0};
    int face[6]={3,2,5,4,1,0};
    rays[4]=(s+1)*(s+1);
    for(int i=0;i<n;++i) if(m[i].next[2]==-1) {
        --rays[4]; int cur=i,d=3,count=0;
        while(true) {
            int t=m[cur].type,a=t/2,b=(a+1)%3;
            int k=d/2;
            if(k==a || k==b) {
                k=(k==a?b:a);
                d=2*k+((d%2)^((t%2)==0)); ++count;
            }
            int nxt=m[cur].next[d];
            if(nxt==-1) { ++rays[face[d]]; turns[face[d]]+=count; break; }
            cur=nxt;
        }
    }
    for(int f=0;f<6;++f) printf("%d\n%d\n",rays[f],turns[f]);
    delete[] order; delete[] m; return 0;
}
