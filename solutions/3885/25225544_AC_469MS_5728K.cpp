// POJ 3885 - Escape from the Minefield
// Model: gpt-6.1-sol
// Build the Delaunay triangulation by divide and conquer. Add its edges
// in increasing length order, maintaining ray-crossing parity in a DSU.
// The first odd cycle surrounds the starting point and closes escape.
// Clearance is also bounded by the nearest mine at the starting point;
// subtract the mines' two-meter radius before converting radius to area.
// Integer orientation and a two-word exact incircle determinant handle
// collinear and cocircular inputs without perturbing the geometry.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
struct Wide {
    ULL lo; LL hi;
    Wide():lo(0),hi(0){}
    Wide(LL a, LL b) {
        bool neg=(a<0)!=(b<0); ULL x=a<0?-a:a,y=b<0?-b:b;
        ULL z=(x&0xffffffffULL)*(y&0xffffffffULL);
        ULL m=(x>>32)*(y&0xffffffffULL)+(y>>32)*(x&0xffffffffULL);
        lo=z+(m<<32); hi=(LL)((x>>32)*(y>>32)+(m>>32)+(lo<z));
        if(neg) {lo=~lo+1;hi=~hi+(lo==0);}
    }
    void add(const Wide &b) {ULL old=lo;lo+=b.lo;hi+=b.hi+(lo<old);}
};
struct Point {LL x,y; bool operator<(const Point &b)const{return x<b.x||(x==b.x&&y<b.y);} };
vector<Point> p;
LL cross(int a,int b,int c) {return (p[b].x-p[a].x)*(p[c].y-p[a].y)-(p[b].y-p[a].y)*(p[c].x-p[a].x);}
bool incircle(int a,int b,int c,int d) {
    LL ax=p[a].x-p[d].x, ay=p[a].y-p[d].y;
    LL bx=p[b].x-p[d].x, by=p[b].y-p[d].y;
    LL cx=p[c].x-p[d].x, cy=p[c].y-p[d].y;
    Wide z(ax*ax+ay*ay,bx*cy-by*cx);
    z.add(Wide(bx*bx+by*by,cx*ay-cy*ax));
    z.add(Wide(cx*cx+cy*cy,ax*by-ay*bx));
    return z.hi>0||(z.hi==0&&z.lo>0);
}
struct QE {int org,next;};
vector<QE> q;
vector<int> avail;
int rot(int e){return (e&~3)|((e+1)&3);}
int rev(int e){return e^2;}
int dest(int e){return q[rev(e)].org;}
int ln(int e){return rot(q[rot(rev(e))].next);}
int op(int e){return rot(q[rot(e)].next);}
int makeedge(int a,int b){
    int e;
    if(avail.empty()){e=q.size();q.resize(e+4);}else{e=avail.back();avail.pop_back();}
    q[e].org=a;q[e+2].org=b;q[e+1].org=q[e+3].org=-1;
    q[e].next=e;q[e+2].next=e+2;q[e+1].next=e+3;q[e+3].next=e+1;
    return e;
}
void splice(int a,int b){swap(q[rot(q[a].next)].next,q[rot(q[b].next)].next);swap(q[a].next,q[b].next);}
void eraseedge(int e){splice(e,op(e));splice(rev(e),op(rev(e)));e&=~3;q[e].org=-1;avail.push_back(e);}
int connect(int a,int b){int e=makeedge(dest(a),q[b].org);splice(e,ln(a));splice(rev(e),b);return e;}
bool leftof(int x,int e){return cross(q[e].org,dest(e),x)>0;}
bool rightof(int x,int e){return cross(q[e].org,dest(e),x)<0;}
pair<int,int> build(int l,int r){
    if(r-l==1){int a=makeedge(l,r);return make_pair(a,rev(a));}
    if(r-l==2){int a=makeedge(l,l+1),b=makeedge(l+1,r);splice(rev(a),b);LL s=cross(l,l+1,r);if(!s)return make_pair(a,rev(b));int c=connect(b,a);return s>0?make_pair(a,rev(b)):make_pair(rev(c),c);}
    int mid=(l+r)/2;pair<int,int>L=build(l,mid),R=build(mid+1,r);
    int ldo=L.first,ldi=L.second,rdi=R.first,rdo=R.second;
    while(true){if(leftof(q[rdi].org,ldi))ldi=ln(ldi);else if(rightof(q[ldi].org,rdi))rdi=q[rev(rdi)].next;else break;}
    int base=connect(rev(rdi),ldi);
    if(q[ldi].org==q[ldo].org)ldo=rev(base);
    if(q[rdi].org==q[rdo].org)rdo=base;
    while(true){
        int lc=q[rev(base)].next;
        if(rightof(dest(lc),base))while(incircle(dest(base),q[base].org,dest(lc),dest(q[lc].next))){int t=q[lc].next;eraseedge(lc);lc=t;}
        int rc=op(base);
        if(rightof(dest(rc),base))while(incircle(dest(base),q[base].org,dest(rc),dest(op(rc)))){int t=op(rc);eraseedge(rc);rc=t;}
        bool vl=rightof(dest(lc),base),vr=rightof(dest(rc),base);
        if(!vl&&!vr)break;
        if(!vl||(vr&&incircle(dest(lc),q[lc].org,q[rc].org,dest(rc))))base=connect(rc,rev(base));
        else base=connect(rev(base),rev(lc));
    }
    return make_pair(ldo,rdo);
}
struct Edge {int a,b;LL w;bool operator<(const Edge &e)const{return w<e.w;}};
vector<int> parent,sz,parity;
int root(int a){if(parent[a]==a)return a;int old=parent[a];parent[a]=root(old);parity[a]^=parity[old];return parent[a];}
int ray(int a,int b){
    // Half-open crossing rule, including vertices on the ray only once.
    if((p[a].y>0)==(p[b].y>0))return 0;
    LL z=p[a].x*p[b].y-p[a].y*p[b].x;
    return (p[b].y>p[a].y)?z>0:z<0;
}
int main(){
    int T;scanf("%d",&T);while(T--){
        int n;scanf("%d",&n);p.resize(n);double near2=1e100;
        for(int i=0;i<n;i++){int x,y;scanf("%d%d",&x,&y);p[i].x=x;p[i].y=y;near2=min(near2,double(p[i].x*p[i].x+p[i].y*p[i].y));}
        sort(p.begin(),p.end());q.clear();avail.clear();q.reserve(12*n);
        if(n>=2)build(0,n-1);
        vector<Edge> edges;edges.reserve(3*n);
        for(int i=0;i<(int)q.size();i+=4)if(q[i].org>=0){Edge e;e.a=q[i].org;e.b=dest(i);LL dx=p[e.a].x-p[e.b].x,dy=p[e.a].y-p[e.b].y;e.w=dx*dx+dy*dy;edges.push_back(e);}
        sort(edges.begin(),edges.end());parent.resize(n);sz.assign(n,1);parity.assign(n,0);for(int i=0;i<n;i++)parent[i]=i;
        double limit=near2;
        for(int i=0;i<(int)edges.size();i++){
            Edge e=edges[i];if(e.w>=4*near2)break;
            int a=root(e.a),b=root(e.b),v=parity[e.a]^parity[e.b]^ray(e.a,e.b);
            if(a==b){if(v){limit=min(limit,e.w/4.0);break;}}
            else{if(sz[a]>sz[b])swap(a,b);parent[a]=b;parity[a]=v;sz[b]+=sz[a];}
        }
        double radius=sqrt(limit)-2;
        LL answer=radius<=0?0:(LL)floor(acos(-1.0)*radius*radius+1e-7);
        cout<<answer<<'\n';
    }
}
