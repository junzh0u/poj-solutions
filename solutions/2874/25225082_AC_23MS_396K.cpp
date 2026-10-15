// POJ 2874 - A Baron Landscape
// Model: gpt-6.1-sol
// Maximize the minimum tax difference over all legal rival castles.
// Start with independent radius-six totals; in overlapping disks the
// baseline difference is zero, so add the tax with the nearer owner's sign.
// Distances are squared Euclidean distances: separation > 9, revenue <= 36.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Term { int offset, sign; Term(int o,int s):offset(o),sign(s){} };
struct Castle { int x,y,pos,total; Castle(int a,int b,int c):x(a),y(b),pos(c),total(0){} };
int land[74*74], worst[2500];
vector<Term> overlap[25][25];
int main() {
    int w,h;
    if (scanf("%d%d",&w,&h)!=2) return 0;
    vector<Castle> c;
    for(int y=0;y<h;++y) for(int x=0;x<w;++x) {
        int v; scanf("%d",&v); int p=(y+12)*74+x+12; land[p]=v;
        if(v) c.push_back(Castle(x,y,p));
    }
    for(int dx=-12;dx<=12;++dx) for(int dy=-12;dy<=12;++dy) {
        if(dx*dx+dy*dy<=9) continue;
        for(int x=-6;x<=6;++x) for(int y=-6;y<=6;++y) {
            int a=x*x+y*y, b=(x-dx)*(x-dx)+(y-dy)*(y-dy);
            if(a<=36 && b<=36 && a!=b)
                overlap[dx+12][dy+12].push_back(Term(y*74+x,a<b?1:-1));
        }
    }
    int n=(int)c.size();
    for(int i=0;i<n;++i) {
        worst[i]=1000000;
        for(int x=-6;x<=6;++x) for(int y=-6;y<=6;++y)
            if(x*x+y*y<=36) c[i].total+=land[c[i].pos+y*74+x];
    }
    for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) {
        int dx=c[j].x-c[i].x, dy=c[j].y-c[i].y;
        if(dx*dx+dy*dy<=9) continue;
        int d=c[i].total-c[j].total;
        if(dx>=-12 && dx<=12 && dy>=-12 && dy<=12) {
            const vector<Term>& t=overlap[dx+12][dy+12];
            for(int k=0;k<(int)t.size();++k) d+=t[k].sign*land[c[i].pos+t[k].offset];
        }
        worst[i]=min(worst[i],d); worst[j]=min(worst[j],-d);
    }
    int best=0;
    for(int i=1;i<n;++i) if(worst[i]>worst[best] ||
       (worst[i]==worst[best] && (c[i].x<c[best].x ||
       (c[i].x==c[best].x && c[i].y<c[best].y)))) best=i;
    printf("Place your castle at: %d %d\n",c[best].x,c[best].y);
    return 0;
}
