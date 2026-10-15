// POJ 2534 - Falling cards
// Model: gpt-6.1-sol
// Clip each standing segment against the rectangle swept by the falling card.
// Point contact counts. Choose the next normal to point away from the previous
// card's supporting line; its dot product with the previous normal is positive.
// The promised single contact makes the cascade a chain, taking O(N^2) time.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point {
    double x,y;
    Point(double a=0,double b=0):x(a),y(b){}
    Point operator-(const Point& p)const{return Point(x-p.x,y-p.y);}
    Point operator*(double a)const{return Point(x*a,y*a);}
};
double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
Point a[100],b[100],u[100];
bool fallen[100];
const double EPS=1e-9;
bool clip(double p,double q,double low,double high,double& t0,double& t1){
    if(fabs(q)<1e-12)return p>=low-EPS && p<=high+EPS;
    double s=(low-p)/q,t=(high-p)/q;
    if(s>t)swap(s,t);
    t0=max(t0,s);t1=min(t1,t);
    return t0<=t1+EPS;
}
bool touches(int i,int j,Point normal,double h){
    Point p=a[j]-a[i],d=b[j]-a[j];
    double t0=0,t1=1;
    return clip(dot(p,u[i]),dot(d,u[i]),0,sqrt(dot(b[i]-a[i],b[i]-a[i])),t0,t1)
        && clip(dot(p,normal),dot(d,normal),0,h,t0,t1);
}
int main(){
    int n;double h;
    if(scanf("%d%lf",&n,&h)!=2)return 0;
    for(int i=0;i<n;++i){
        scanf("%lf%lf%lf%lf",&a[i].x,&a[i].y,&b[i].x,&b[i].y);
        Point d=b[i]-a[i];u[i]=d*(1/sqrt(dot(d,d)));
    }
    int cur=0;Point normal(-u[0].y,u[0].x);
    while(cur>=0){
        fallen[cur]=true;int next=-1;
        for(int j=0;j<n;++j)if(!fallen[j] && touches(cur,j,normal,h)){next=j;break;}
        if(next>=0){
            Point nn(-u[next].y,u[next].x);
            if(dot(nn,normal)<0)nn=nn*(-1);
            normal=nn;
        }
        cur=next;
    }
    bool first=true;
    for(int i=0;i<n;++i)if(fallen[i]){if(!first)printf(" ");printf("%d",i+1);first=false;}
    printf("\n");return 0;
}
