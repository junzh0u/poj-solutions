// POJ 3949 - Zigzag
// Model: gpt-6-astra
// Every segment lies on a line through two input points. Dynamic programming
// records the covered subset and the ordered pair of lines at the last turn.
// Intersections fix internal segments; an optimal outer endpoint is an input
// point. Each finite segment, not just its supporting line, must cover >=2.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double INF=1e100, EPS=1e-8;
struct Line { int a,b,c,mask; double t[10],scale; };
struct Edge { int dest,mask; double len; Edge(int d,int m,double l):dest(d),mask(m),len(l){} };
int gcd(int a,int b) { return b?gcd(b,a%b):a; }
int countbits(int x) { int c=0; for(;x;x&=x-1) ++c; return c; }
int n,x[10],y[10];
vector<Line> lines;
int covered(int i,double a,double b) {
    if(a>b) swap(a,b);
    int m=0;
    for(int p=0;p<n;++p) if((lines[i].mask>>p&1) && lines[i].t[p]>=a-EPS && lines[i].t[p]<=b+EPS) m|=1<<p;
    return m;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        for(int i=0;i<n;++i) scanf("%d%d",&x[i],&y[i]);
        lines.clear();
        for(int i=0;i<n;++i) for(int j=0;j<i;++j) {
            Line l;
            l.a=y[i]-y[j]; l.b=x[j]-x[i]; l.c=-l.a*x[i]-l.b*y[i];
            int g=gcd(abs(l.a),abs(l.b)); if(!g) continue;
            if(l.a<0 || (l.a==0 && l.b<0)) g=-g;
            l.a/=g; l.b/=g; l.c/=g;
            bool found=false;
            for(size_t k=0;k<lines.size();++k) if(lines[k].a==l.a && lines[k].b==l.b && lines[k].c==l.c) found=true;
            if(found) continue;
            l.mask=0; l.scale=sqrt(double(l.a*l.a+l.b*l.b));
            for(int p=0;p<n;++p) {
                l.t[p]=double(l.b*x[p]-l.a*y[p])/l.scale;
                if(l.a*x[p]+l.b*y[p]+l.c==0) l.mask|=1<<p;
            }
            lines.push_back(l);
        }
        int m=lines.size(), S=1<<n, full=S-1;
        double answer=INF;
        for(int i=0;i<m;++i) if(lines[i].mask==full) {
            double lo=INF,hi=-INF;
            for(int p=0;p<n;++p) {lo=min(lo,lines[i].t[p]);hi=max(hi,lines[i].t[p]);}
            answer=min(answer,hi-lo);
        }
        if(answer<INF/2) {printf("0 %.10f\n",answer);continue;}
        double cross[45][45]; bool ok[45][45];
        for(int i=0;i<m;++i) for(int j=0;j<m;++j) {
            int d=lines[i].a*lines[j].b-lines[j].a*lines[i].b;
            ok[i][j]=d!=0;
            if(d) {
                double px=double(lines[i].b*lines[j].c-lines[j].b*lines[i].c)/d;
                double py=double(lines[i].c*lines[j].a-lines[j].c*lines[i].a)/d;
                cross[i][j]=(lines[i].b*px-lines[i].a*py)/lines[i].scale;
            }
        }
        vector<double> dp(m*m*S,INF), next(dp), ending(dp);
        vector<vector<Edge> > edges(m*m);
        for(int i=0;i<m;++i) for(int j=0;j<m;++j) if(ok[i][j]) {
            // State i*m+j is the intersection of incoming i and outgoing j.
            int st=i*m+j;
            for(int p=0;p<n;++p) if(lines[i].mask>>p&1) {
                int mask=covered(i,lines[i].t[p],cross[i][j]);
                if(countbits(mask)>=2) dp[st*S+mask]=min(dp[st*S+mask],fabs(lines[i].t[p]-cross[i][j]));
            }
            for(int p=0;p<n;++p) if(lines[j].mask>>p&1) {
                int mask=covered(j,cross[j][i],lines[j].t[p]);
                if(countbits(mask)>=2) ending[st*S+mask]=min(ending[st*S+mask],fabs(cross[j][i]-lines[j].t[p]));
            }
            // Minimum last-segment length covering at least a requested subset.
            for(int bit=0;bit<n;++bit) for(int mask=0;mask<S;++mask) if(!(mask>>bit&1))
                ending[st*S+mask]=min(ending[st*S+mask],ending[st*S+(mask|(1<<bit))]);
            for(int k=0;k<m;++k) if(ok[j][k]) {
                int mask=covered(j,cross[j][i],cross[j][k]);
                if(countbits(mask)>=2) edges[st].push_back(Edge(j*m+k,mask,fabs(cross[j][i]-cross[j][k])));
            }
        }
        int turns;
        for(turns=1;turns<=4;++turns) {
            answer=INF;
            for(int st=0;st<m*m;++st) for(int mask=0;mask<S;++mask)
                answer=min(answer,dp[st*S+mask]+ending[st*S+(full^mask)]);
            if(answer<INF/2) break;
            fill(next.begin(),next.end(),INF);
            for(int st=0;st<m*m;++st) for(int mask=0;mask<S;++mask) if(dp[st*S+mask]<INF/2) {
                double val=dp[st*S+mask];
                for(size_t e=0;e<edges[st].size();++e) {
                    const Edge &a=edges[st][e];
                    double &v=next[a.dest*S+(mask|a.mask)];
                    v=min(v,val+a.len);
                }
            }
            dp.swap(next);
        }
        printf("%d %.10f\n",turns,answer);
    }
    return 0;
}
