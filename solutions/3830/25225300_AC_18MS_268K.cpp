// POJ 3830 - Ancient vending machine
// Model: gpt-6.1-sol
// Hold the zero-thickness coin vertically: its least possible projection is
// the minimum width of its convex hull. Compare with the longest segment
// contained in the (possibly concave) hole. A longest segment can be chosen
// on a line through two polygon vertices; split each such line at boundary
// intersections and join consecutive intervals lying inside or on the hole.
// The initial orientation is free; only rotation during the fall is forbidden.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double EPS=1e-8;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} };
P operator+(P a,P b){return P(a.x+b.x,a.y+b.y);}
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
P operator*(P a,double t){return P(a.x*t,a.y*t);}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double norm(P a){return sqrt(dot(a,a));}
bool cmp(P a,P b){return a.x<b.x || (a.x==b.x && a.y<b.y);}
bool inside(P q,const vector<P>& p){
    bool in=false; int n=p.size();
    for(int i=0;i<n;i++){
        P a=p[i],b=p[(i+1)%n];
        if(fabs(cross(b-a,q-a))<=EPS*norm(b-a) && dot(q-a,q-b)<=EPS) return true;
        if((a.y>q.y)!=(b.y>q.y)){
            double x=a.x+(b.x-a.x)*(q.y-a.y)/(b.y-a.y);
            if(x>q.x) in=!in;
        }
    }
    return in;
}
double chord(P a,P b,const vector<P>& p){
    P d=b-a; double len=norm(d); if(len<EPS)return 0;
    d=d*(1/len); vector<double> ts; int n=p.size();
    for(int i=0;i<n;i++){
        P u=p[i],v=p[(i+1)%n];
        double su=cross(d,u-a),sv=cross(d,v-a);
        if(fabs(su)<EPS)ts.push_back(dot(u-a,d));
        if(fabs(sv)<EPS)ts.push_back(dot(v-a,d));
        if((su>EPS && sv<-EPS)||(su<-EPS && sv>EPS)){
            P q=u+(v-u)*(su/(su-sv)); ts.push_back(dot(q-a,d));
        }
    }
    sort(ts.begin(),ts.end()); vector<double> t;
    for(size_t i=0;i<ts.size();i++) if(t.empty()||ts[i]-t.back()>EPS)t.push_back(ts[i]);
    double best=0,run=0;
    for(size_t i=1;i<t.size();i++){
        if(inside(a+d*((t[i-1]+t[i])/2),p))run+=t[i]-t[i-1]; else run=0;
        best=max(best,run);
    }
    return best;
}
double longest(const vector<P>& p){
    double ans=0;
    for(size_t i=0;i<p.size();i++)for(size_t j=i+1;j<p.size();j++)ans=max(ans,chord(p[i],p[j],p));
    return ans;
}
double width(vector<P> p){
    sort(p.begin(),p.end(),cmp); int n=p.size(),k=0; vector<P> h(2*n);
    for(int i=0;i<n;i++){
        while(k>=2 && cross(h[k-1]-h[k-2],p[i]-h[k-1])<=0)k--;
        h[k++]=p[i];
    }
    int lower=k;
    for(int i=n-2;i>=0;i--){
        while(k>lower && cross(h[k-1]-h[k-2],p[i]-h[k-1])<=0)k--;
        h[k++]=p[i];
    }
    k--; if(k<3)return 0;
    double ans=1e100;
    for(int i=0;i<k;i++){
        P d=h[(i+1)%k]-h[i]; double far=0;
        for(int j=0;j<k;j++)far=max(far,fabs(cross(d,h[j]-h[i]))/norm(d));
        ans=min(ans,far);
    }
    return ans;
}
int main(){
    int T,n,m; if(scanf("%d",&T)!=1)return 0;
    while(T--){
        scanf("%d",&n); vector<P> hole(n);
        for(int i=0;i<n;i++)scanf("%lf%lf",&hole[i].x,&hole[i].y);
        scanf("%d",&m); vector<P> coin(m);
        for(int i=0;i<m;i++)scanf("%lf%lf",&coin[i].x,&coin[i].y);
        puts(longest(hole)+EPS>=width(coin)?"legal":"illegal");
    }
    return 0;
}
