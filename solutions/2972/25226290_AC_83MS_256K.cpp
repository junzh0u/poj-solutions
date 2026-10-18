// POJ 2972 - Laser-Ball
// Model: gpt-6-astra
// Enumerate reflected target images; reject geometrically invalid sequences
// before tracing candidates against all mirrors to validate the emitted ray.
// Mirrors are two-sided; parallel rays do not collide, endpoints do reflect.
// Tiny angular offsets also permit a near-target hit around a blocking endpoint.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct P {
    double x,y;
    P(double a=0,double b=0):x(a),y(b){}
    P operator+(P b)const{return P(x+b.x,y+b.y);}
    P operator-(P b)const{return P(x-b.x,y-b.y);}
    P operator*(double b)const{return P(x*b,y*b);}
};
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
struct Mirror {P a,u; double len;} m[105];
int n,k;
P target,answer;
P reflectVector(P p,int i){return m[i].u*(2*dot(p,m[i].u))-p;}
P reflectPoint(P p,int i){return m[i].a+reflectVector(p-m[i].a,i);}
bool trace(P d){
    double length=sqrt(dot(d,d));
    if(length<1e-15)return false;
    d=d*(1/length);
    P p;
    int last=-1;
    for(int step=0;step<=k;++step){
        double nearest=1e100;
        int hit=-1;
        for(int i=0;i<n;++i)if(i!=last){
            double den=cross(d,m[i].u);
            if(fabs(den)<1e-12)continue;
            P v=m[i].a-p;
            double t=cross(v,m[i].u)/den;
            double s=cross(v,d)/den;
            if(t>1e-10 && s>=-1e-10 && s<=m[i].len+1e-10 && t<nearest){
                nearest=t;hit=i;
            }
        }
        P b=target-p;
        double along=dot(b,d);
        double t=max(0.0,min(nearest,along));
        P error=b-d*t;
        if(dot(error,error)<=1e-8*(1-1e-8))return true;
        if(hit<0 || step==k)return false;
        p=p+d*nearest;
        d=reflectVector(d,hit);
        last=hit;
    }
    return false;
}
int sequence[12];
bool feasible(P d,int depth){
    P p;
    double length=sqrt(dot(d,d));
    if(length<1e-15)return false;
    d=d*(1/length);
    for(int j=depth-1;j>=0;--j){
        int i=sequence[j];
        double den=cross(d,m[i].u);
        if(fabs(den)<1e-12)return false;
        P v=m[i].a-p;
        double t=cross(v,m[i].u)/den;
        double s=cross(v,d)/den;
        if(t<=1e-10 || s < -1e-10 || s>m[i].len+1e-10)return false;
        p=p+d*t;
        d=reflectVector(d,i);
    }
    return dot(target-p,d)>=-1e-10;
}
bool search(P image,int depth,int last){
    bool valid=feasible(image,depth);
    if(valid && trace(image)){answer=image;return true;}
    double len=sqrt(dot(image,image));
    if(valid && len>1e-12){
        P offset=P(-image.y,image.x)*(1e-7/(len*len));
        if(trace(image+offset)){answer=image+offset;return true;}
        if(trace(image-offset)){answer=image-offset;return true;}
    }
    if(depth==k)return false;
    for(int i=0;i<n;++i)if(i!=last){
        sequence[depth]=i;
        if(search(reflectPoint(image,i),depth+1,i))return true;
    }
    return false;
}
int main(){
    if(scanf("%lf%lf%d%d",&target.x,&target.y,&n,&k)!=4)return 0;
    for(int i=0;i<n;++i){
        P b;scanf("%lf%lf%lf%lf",&m[i].a.x,&m[i].a.y,&b.x,&b.y);
        P u=b-m[i].a;m[i].len=sqrt(dot(u,u));m[i].u=u*(1/m[i].len);
    }
    if(dot(target,target)<1e-8){puts("1 0");return 0;}
    if(search(target,0,-1)){
        double scale=max(fabs(answer.x),fabs(answer.y));
        printf("%.15f %.15f\n",answer.x/scale,answer.y/scale);
    }else puts("impossible");
    return 0;
}
