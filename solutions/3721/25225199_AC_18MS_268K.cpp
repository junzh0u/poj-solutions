// POJ 3721 - Darkterror's Combos
// Model: gpt-6.1-sol
// Reachable sphere centers form a disk of radius Rw+Rc. Enumerate its
// arrangement with the nine coverage disks: vertices, open arcs, and both
// adjacent faces. Boundary heroes count; negative threats cannot be dropped.
// Coincident boundaries change together. Zero radii are handled separately.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;
typedef long double LD;
const LD PI=acosl(-1.0L), EPS=1e-10L;
struct Circle { LD x,y,r; int w; };
Circle c[10]; int best;
LD distance(LD x, LD y, Circle a) { return hypotl(x-a.x,y-a.y); }
bool same(Circle a,Circle b) {return a.x==b.x && a.y==b.y && a.r==b.r;}
void point(LD x,LD y) {
    if(distance(x,y,c[0])>c[0].r+EPS) return;
    int s=0;
    for(int j=1;j<10;++j) if(distance(x,y,c[j])<=c[j].r+EPS) s+=c[j].w;
    best=max(best,s);
}
void arc(int i,LD a) {
    LD x=c[i].x+c[i].r*cosl(a),y=c[i].y+c[i].r*sinl(a);
    for(int side=0;side<2;++side) {
        bool ok=true; int s=0;
        for(int j=0;j<10;++j) {
            bool in=same(c[i],c[j]) ? side==0 : distance(x,y,c[j])<c[j].r;
            if(j==0 && !in) ok=false;
            if(j && in) s+=c[j].w;
        }
        if(ok) best=max(best,s);
    }
    point(x,y);
}
int main() {
    int rw,rc,r;
    if(!(cin>>rw>>rc>>r)) return 0;
    for(int i=0;i<10;++i) {cin>>c[i].x>>c[i].y>>c[i].w; c[i].r=r;}
    c[0].r=rw+rc;
    best=INT_MIN;
    if(c[0].r==0) {point(c[0].x,c[0].y);}
    else if(r==0) {
        best=0;
        for(int i=1;i<10;++i) point(c[i].x,c[i].y);
    } else {
        point(c[0].x,c[0].y);
        for(int i=0;i<10;++i) {
            vector<LD> a; a.push_back(0); a.push_back(2*PI);
            for(int j=0;j<10;++j) if(i!=j) {
                LD dx=c[j].x-c[i].x,dy=c[j].y-c[i].y,d=hypotl(dx,dy);
                if(d==0 || d>c[i].r+c[j].r || d<fabsl(c[i].r-c[j].r)) continue;
                LD co=(d*d+c[i].r*c[i].r-c[j].r*c[j].r)/(2*d*c[i].r);
                co=max(-1.0L,min(1.0L,co));
                LD b=atan2l(dy,dx), t=acosl(co);
                for(int k=-1;k<=1;k+=2) {
                    LD z=b+k*t;
                    while(z<0) z+=2*PI;
                    while(z>=2*PI) z-=2*PI;
                    a.push_back(z);
                    point(c[i].x+c[i].r*cosl(z),c[i].y+c[i].r*sinl(z));
                }
            }
            sort(a.begin(),a.end());
            for(size_t k=1;k<a.size();++k) if(a[k]>a[k-1]) arc(i,(a[k]+a[k-1])/2);
        }
    }
    cout<<best<<'\n';
    return 0;
}
