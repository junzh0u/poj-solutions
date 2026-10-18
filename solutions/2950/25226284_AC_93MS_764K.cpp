// POJ 2950 - Nuclear Plants
// Model: gpt-6-astra
// Split the affected area into unit squares. Integer centers and the two
// fixed radii give only 3^4 * 2^8 local configurations; cache their exact
// circle-union areas, integrating exposed arcs and covered square edges.
// Duplicate plants are harmless, and a large plant subsumes a colocated small one.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI;
const int X[12]={0,1,0,1,-1,-1,2,2,0,1,0,1};
const int Y[12]={0,0,1,1,0,1,0,1,-1,-1,2,2};
struct Circle { double x,y,r; };
double cache[65536];
typedef pair<double,double> Interval;
void arc(vector<Interval>& v,double a,double b) {
    double l=a-b,h=a+b;
    while(l<0) {l+=TAU;h+=TAU;}
    while(l>=TAU) {l-=TAU;h-=TAU;}
    if(h>TAU) {v.push_back(Interval(l,TAU));v.push_back(Interval(0,h-TAU));}
    else v.push_back(Interval(l,h));
}
double primitive(const Circle& c,double a,double b) {
    return (c.r*c.r*(b-a)+c.r*c.x*(sin(b)-sin(a))
           +c.r*c.y*(cos(a)-cos(b)))*0.5;
}
double merged(vector<Interval>& v) {
    sort(v.begin(),v.end()); double result=0,end=0;
    for(size_t i=0;i<v.size();++i) {
        double l=max(0.0,v[i].first),h=min(1.0,v[i].second);
        if(h>max(l,end)) result+=h-max(l,end);
        end=max(end,h);
    }
    return result;
}
double area(int mask) {
    if(cache[mask]>=0) return cache[mask];
    vector<Circle> c;
    for(int i=0;i<12;++i) {
        int t=i<4?((mask>>(2*i))&3):((mask>>(i+4))&1)*2;
        if(t) {Circle z={double(X[i]),double(Y[i]),t==1?0.58:1.31};c.push_back(z);}
    }
    double result=0;
    for(size_t i=0;i<c.size();++i) {
        const Circle &p=c[i]; vector<Interval> v;
        double d[4]={p.x,1-p.x,p.y,1-p.y};
        double a[4]={PI,0,1.5*PI,0.5*PI}; bool hidden=false;
        for(int k=0;k<4;++k) {
            if(d[k]<=-p.r) {hidden=true;break;}
            if(d[k]<p.r) arc(v,a[k],acos(d[k]/p.r));
        }
        if(hidden) continue;
        for(size_t j=0;j<c.size();++j) if(i!=j) {
            double dx=c[j].x-p.x,dy=c[j].y-p.y;
            double dist=sqrt(dx*dx+dy*dy),r=c[j].r;
            if(dist+p.r<=r+1e-12) {hidden=true;break;}
            if(dist>=p.r+r || dist+r<=p.r) continue;
            arc(v,atan2(dy,dx),acos((dist*dist+p.r*p.r-r*r)/(2*dist*p.r)));
        }
        if(hidden) continue;
        sort(v.begin(),v.end()); double end=0;
        for(size_t j=0;j<v.size();++j) {
            if(v[j].first>end) result+=primitive(p,end,v[j].first);
            end=max(end,v[j].second);
        }
        if(end<TAU) result+=primitive(p,end,TAU);
    }
    vector<Interval> right,top;
    for(size_t i=0;i<c.size();++i) {
        double q=c[i].r*c[i].r-(1-c[i].x)*(1-c[i].x);
        if(q>0) {double h=sqrt(q);right.push_back(Interval(c[i].y-h,c[i].y+h));}
        q=c[i].r*c[i].r-(1-c[i].y)*(1-c[i].y);
        if(q>0) {double h=sqrt(q);top.push_back(Interval(c[i].x-h,c[i].x+h));}
    }
    result+=(merged(right)+merged(top))*0.5;
    return cache[mask]=max(0.0,min(1.0,result));
}
int main() {
    fill(cache,cache+65536,-1.0);cache[0]=0;
    int n,m,ks,kl;
    while(scanf("%d%d%d%d",&n,&m,&ks,&kl)==4 && (n||m||ks||kl)) {
        map<int,int> cells;
        for(int k=0;k<ks+kl;++k) {
            int x,y;scanf("%d%d",&x,&y);bool big=k>=ks;
            for(int i=0;i<(big?12:4);++i) {
                int cx=x-X[i],cy=y-Y[i];
                if(cx<0||cx>=n||cy<0||cy>=m) continue;
                int &mask=cells[cx*m+cy];
                if(i<4) {
                    int old=(mask>>(2*i))&3,t=big?2:1;
                    if(t>old) mask=(mask&~(3<<(2*i)))|(t<<(2*i));
                } else mask|=1<<(i+4);
            }
        }
        double answer=double(n)*m;
        for(map<int,int>::const_iterator it=cells.begin();it!=cells.end();++it) answer-=area(it->second);
        printf("%.2f\n",max(0.0,answer));
    }
    return 0;
}
