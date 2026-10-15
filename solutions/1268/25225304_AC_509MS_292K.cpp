// POJ 1268 - Forests
// Model: gpt-6.1-sol
// Process equal-radius trunks by distance and maintain their angular union.
// A visible trunk must have its whole interval, plus both required gaps, free.
// A distance-ordered quadtree skips boxes entirely hidden by nearer trunks.
// Numeric input accepts both 0.xx and .xx, as noted on the discuss board.
#include <cstdio>
#include <cmath>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI, GAP=PI/18000;
map<double,double> cover;
bool piece(double l,double r,bool full) {
    map<double,double>::iterator it=cover.upper_bound(l);
    if(it!=cover.begin()) {--it; if(full) return it->first<=l && it->second>=r; if(it->second>=l) return true; ++it;}
    return !full && it!=cover.end() && it->first<=r;
}
bool query(double l,double r,bool full) {
    while(l<0) {l+=TAU;r+=TAU;} while(l>=TAU){l-=TAU;r-=TAU;}
    if(r<=TAU) return piece(l,r,full);
    if(full) return piece(l,TAU,true)&&piece(0,r-TAU,true);
    return piece(l,TAU,false)||piece(0,r-TAU,false);
}
void addpiece(double l,double r) {
    map<double,double>::iterator it=cover.upper_bound(l);
    if(it!=cover.begin()){--it;if(it->second<l)++it;}
    while(it!=cover.end()&&it->first<=r){l=min(l,it->first);r=max(r,it->second);cover.erase(it++);}
    cover[l]=r;
}
void add(double l,double r) {
    while(l<0){l+=TAU;r+=TAU;}while(l>=TAU){l-=TAU;r-=TAU;}
    if(r<=TAU)addpiece(l,r);else{addpiece(l,TAU);addpiece(0,r-TAU);}
}
struct Box {int a,b,c,d; double dist; bool operator<(const Box& o)const{return dist>o.dist;}};
double ox,oy,rad,limit;
priority_queue<Box> heap;
void push(int a,int b,int c,int d){
    if(a>b||c>d)return;
    double dx=max(max(a-ox,ox-b),0.0),dy=max(max(c-oy,oy-d),0.0);
    Box z={a,b,c,d,dx*dx+dy*dy}; if(z.dist<limit*limit)heap.push(z);
}
bool hidden(const Box& z){
    double x[2]={z.a-ox-rad,z.b-ox+rad},y[2]={z.c-oy-rad,z.d-oy+rad};
    if(x[0]<=0&&x[1]>=0&&y[0]<=0&&y[1]>=0)return false;
    double mid=atan2((y[0]+y[1])/2,(x[0]+x[1])/2),lo=1e9,hi=-1e9;
    for(int i=0;i<2;i++)for(int j=0;j<2;j++){
        double t=atan2(y[j],x[i]);while(t-mid>PI)t-=TAU;while(t-mid< -PI)t+=TAU;
        lo=min(lo,t);hi=max(hi,t);
    }
    return query(lo,hi,true);
}
int main(){
    double diameter;
    while(scanf("%lf%lf%lf",&diameter,&ox,&oy)==3&&diameter!=0){
        rad=diameter/2;limit=rad/sin(GAP/2);cover.clear();while(!heap.empty())heap.pop();
        int n=(int)ceil(limit)+1,ans=0;push(-n,n,-n,n);
        while(!heap.empty()){
            Box z=heap.top();heap.pop();if(hidden(z))continue;
            if(z.a==z.b&&z.c==z.d){
                double angle=atan2(z.c-oy,z.a-ox),half=asin(rad/sqrt(z.dist));
                if(2*half>GAP&&!query(angle-half-GAP,angle+half+GAP,false))++ans;
                add(angle-half,angle+half);
            }else if(z.b-z.a>=z.d-z.c){int m=z.a+(z.b-z.a)/2;push(z.a,m,z.c,z.d);push(m+1,z.b,z.c,z.d);}
            else{int m=z.c+(z.d-z.c)/2;push(z.a,z.b,z.c,m);push(z.a,z.b,m+1,z.d);}
        }
        printf("%d\n",ans);
    }
}
