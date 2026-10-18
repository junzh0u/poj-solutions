// POJ 3479 - RoBa Has a Second Art Gallery!
// Model: gpt-6-astra
// Move each camera to a polygon vertex without increasing its viewing angle.
// For each vertex pair, optimize the coverage meeting point independently on
// each boundary chain. On an edge, minimizing the two base angles is the
// same as maximizing the angle subtended by the camera segment. Its derivative
// has a quadratic numerator, so endpoints and stationary points suffice.
#include <cstdio>
#include <cmath>
#include <algorithm>
struct Point { double x,y; Point(double X=0,double Y=0):x(X),y(Y){} } p[105];
Point operator-(Point a,Point b){return Point(a.x-b.x,a.y-b.y);}
Point operator+(Point a,Point b){return Point(a.x+b.x,a.y+b.y);}
Point operator*(Point a,double t){return Point(a.x*t,a.y*t);}
double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
double dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
double angle(Point a,Point b){return atan2(fabs(cross(a,b)),dot(a,b));}
double val(int a,int b,int k,int l,double t){
    Point x=p[k]+(p[l]-p[k])*t;
    Point u=x-p[a],v=x-p[b];
    if(t==0 && k==a) u=p[l]-p[k];
    if(t==1 && l==a) u=p[k]-p[l];
    if(t==0 && k==b) v=p[l]-p[k];
    if(t==1 && l==b) v=p[k]-p[l];
    return angle(u,p[b]-p[a])+angle(v,p[a]-p[b]);
}
double edge(int a,int b,int k,int l){
    double ans=std::min(val(a,b,k,l,0),val(a,b,k,l,1));
    if(k==a || l==a || k==b || l==b) return ans;
    Point u=p[a]-p[k],v=p[b]-p[k],d=p[l]-p[k];
    double c0=cross(u,v),c1=cross(d,u-v);
    double d0=dot(u,v),d1=-dot(d,u+v),d2=dot(d,d);
    double A=-c1*d2,B=-2*c0*d2,C=c1*d0-c0*d1;
    if(A==0){
        if(B!=0){double t=-C/B;if(t>0 && t<1)ans=std::min(ans,val(a,b,k,l,t));}
    }else{
        double disc=B*B-4*A*C;
        if(disc>=0){
            double q=-0.5*(B+(B>=0?1:-1)*sqrt(disc));
            double t=q/A;
            if(t>0 && t<1)ans=std::min(ans,val(a,b,k,l,t));
            if(q!=0){t=C/q;if(t>0 && t<1)ans=std::min(ans,val(a,b,k,l,t));}
        }
    }
    return ans;
}
int main(){
    int n;if(scanf("%d",&n)!=1)return 0;
    for(int i=0;i<n;++i)scanf("%lf%lf",&p[i].x,&p[i].y);
    double ans=acos(-1.0);
    for(int i=0;i<n;++i)ans=std::min(ans,angle(p[(i+1)%n]-p[i],p[(i+n-1)%n]-p[i]));
    for(int a=0;a<n;++a)for(int b=a+1;b<n;++b){
        double s=10,t=10;
        for(int k=a;k<b;++k)s=std::min(s,edge(a,b,k,k+1));
        for(int k=b;k!=a;k=(k+1)%n)t=std::min(t,edge(a,b,k,(k+1)%n));
        ans=std::min(ans,s+t);
    }
    printf("%.13f\n",ans);
    return 0;
}
