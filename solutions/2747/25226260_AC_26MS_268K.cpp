// POJ 2747 - Shy Polygons
// Model: gpt-6-astra
// Triangulate the solid polygons. Each pair's Minkowski difference, expanded
// by a radius-L disk, cuts the translation axis in one forbidden interval.
// Union these intervals and minimize the bounding width on their complement.
// Concavities and containment are covered by triangles, not boundary distance
// alone. Motion may pass through collisions; only the final placement matters.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
struct P {
    double x,y;
    P(double X=0,double Y=0):x(X),y(Y){}
    P operator-(P b)const{return P(x-b.x,y-b.y);}
    bool operator<(P b)const{return x!=b.x?x<b.x:y<b.y;}
};
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double turn(P a,P b,P c){return cross(b-a,c-a);}
typedef vector<P> Poly;
vector<Poly> triangulate(Poly p){
    vector<Poly> r;
    while(p.size()>3){
        bool cut=false;
        for(int i=0;i<(int)p.size();++i){
            int j=(i+1)%p.size(), k=(i+2)%p.size();
            if(turn(p[i],p[j],p[k])==0){
                p.erase(p.begin()+j);cut=true;break;
            }
            if(turn(p[i],p[j],p[k])<0)continue;
            bool empty=true;
            for(int v=0;v<(int)p.size();++v)if(v!=i&&v!=j&&v!=k)
                if(turn(p[i],p[j],p[v])>=0&&turn(p[j],p[k],p[v])>=0&&turn(p[k],p[i],p[v])>=0)empty=false;
            if(empty){
                Poly t;t.push_back(p[i]);t.push_back(p[j]);t.push_back(p[k]);r.push_back(t);
                p.erase(p.begin()+j);cut=true;break;
            }
        }
        assert(cut);
    }
    r.push_back(p);return r;
}
Poly hull(Poly p){
    sort(p.begin(),p.end());
    Poly h;
    for(int i=0;i<(int)p.size();++i){
        while(h.size()>1&&turn(h[h.size()-2],h.back(),p[i])<=0)h.pop_back();
        h.push_back(p[i]);
    }
    int s=h.size();
    for(int i=(int)p.size()-2;i>=0;--i){
        while((int)h.size()>s&&turn(h[h.size()-2],h.back(),p[i])<=0)h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();return h;
}
void addpoint(P p,double L,double &lo,double &hi){
    double d=sqrt(max(0.0,L*L-p.y*p.y));
    lo=min(lo,p.x-d);hi=max(hi,p.x+d);
}
pair<double,double> interval(Poly h,double L){
    double lo=1e100,hi=-1e100;
    for(int i=0;i<(int)h.size();++i){
        P a=h[i],b=h[(i+1)%h.size()];
        if(a.y>b.y)swap(a,b);
        if(a.y>L||b.y< -L)continue;
        if(a.y==b.y){addpoint(a,L,lo,hi);addpoint(b,L,lo,hi);continue;}
        double low=max(a.y,-L),high=min(b.y,L);
        double k=(b.x-a.x)/(b.y-a.y);
        addpoint(P(a.x+k*(low-a.y),low),L,lo,hi);
        addpoint(P(a.x+k*(high-a.y),high),L,lo,hi);
        double y=L*k/sqrt(1+k*k);
        if(y>=low&&y<=high)addpoint(P(a.x+k*(y-a.y),y),L,lo,hi);
        y=-y;
        if(y>=low&&y<=high)addpoint(P(a.x+k*(y-a.y),y),L,lo,hi);
    }
    return make_pair(lo,hi);
}
double width(double t,double a,double b,double c,double d){return max(b,d+t)-min(a,c+t);}
int main(){
    double L;
    while(scanf("%lf",&L)==1&&L>0){
        Poly p[2];double mn[2]={1e100,1e100},mx[2]={-1e100,-1e100};
        for(int z=0;z<2;++z){int n;scanf("%d",&n);while(n--){P a;scanf("%lf%lf",&a.x,&a.y);p[z].push_back(a);mn[z]=min(mn[z],a.x);mx[z]=max(mx[z],a.x);}}
        vector<Poly> a=triangulate(p[0]),b=triangulate(p[1]);
        vector<pair<double,double> > ranges,merged;
        for(int i=0;i<(int)a.size();++i)for(int j=0;j<(int)b.size();++j){
            Poly pts;
            for(int u=0;u<3;++u)for(int v=0;v<3;++v)pts.push_back(a[i][u]-b[j][v]);
            pair<double,double> q=interval(hull(pts),L);
            if(q.first<q.second)ranges.push_back(q);
        }
        sort(ranges.begin(),ranges.end());
        for(int i=0;i<(int)ranges.size();++i){
            if(merged.empty()||ranges[i].first>=merged.back().second-1e-10)merged.push_back(ranges[i]);
            else merged.back().second=max(merged.back().second,ranges[i].second);
        }
        double t=mn[0]-mn[1],ans=width(t,mn[0],mx[0],mn[1],mx[1]);
        for(int i=0;i<(int)merged.size();++i)if(t>merged[i].first&&t<merged[i].second){
            ans=min(width(merged[i].first,mn[0],mx[0],mn[1],mx[1]),width(merged[i].second,mn[0],mx[0],mn[1],mx[1]));break;
        }
        printf("%.9f\n",ans);
    }
}
