// POJ 3947 - Top Spinning
// Model: gpt-6-astra
// Add signed circular segments to the polygon of arc chords to find mass
// and first moments. Negative radii turn counterclockwise, adding a segment;
// positive radii turn clockwise, subtracting it. Only minor arcs are used.
// Test containment by horizontal ray crossings of lines and y-monotone arcs.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
struct P {
    double x,y;
    P(double X=0,double Y=0):x(X),y(Y){}
    P operator+(P b)const{return P(x+b.x,y+b.y);}
    P operator-(P b)const{return P(x-b.x,y-b.y);}
    P operator*(double t)const{return P(x*t,y*t);}
};
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
struct Edge { P a,b,c; double r,t,d; };
int main(){
    char cmd[20];
    while(scanf("%19s",cmd)==1 && strcmp(cmd,"end")){
        P first,prev; scanf("%lf%lf",&first.x,&first.y); prev=first;
        vector<Edge> e; double area=0; P moment;
        for(;;){
            scanf("%19s",cmd); Edge z; z.a=prev; z.r=0;
            bool done=!strcmp(cmd,"close");
            if(done) z.b=first;
            else scanf("%lf%lf",&z.b.x,&z.b.y);
            if(!strcmp(cmd,"arc")) scanf("%lf",&z.r);
            double a=cross(z.a,z.b)/2;
            area+=a; moment=moment+(z.a+z.b)*(a/3);
            if(z.r!=0){
                P v=z.b-z.a; double len=sqrt(v.x*v.x+v.y*v.y);
                double R=fabs(z.r), s=z.r<0?1:-1;
                P left(-v.y/len,v.x/len), mid=(z.a+z.b)*0.5;
                double h=sqrt(R*R-len*len/4);
                z.c=mid+left*(s*h);
                double theta=2*asin(len/(2*R));
                double cap=R*R*(theta-sin(theta))/2;
                double dist=len*len*len/(12*cap);
                P center=z.c-left*(s*dist);
                area+=s*cap; moment=moment+center*(s*cap);
                z.t=atan2(z.a.y-z.c.y,z.a.x-z.c.x); z.d=s*theta;
            }
            e.push_back(z); prev=z.b; if(done) break;
        }
        P q=moment*(1/area); bool inside=false;
        for(size_t i=0;i<e.size();++i){
            Edge z=e[i];
            if(z.r==0){
                if((z.a.y>q.y)!=(z.b.y>q.y)){
                    double x=z.a.x+(z.b.x-z.a.x)*(q.y-z.a.y)/(z.b.y-z.a.y);
                    if(x>q.x) inside=!inside;
                }
            }else{
                double lo=min(z.t,z.t+z.d),hi=max(z.t,z.t+z.d),R=fabs(z.r);
                vector<double> t; t.push_back(lo);
                for(int k=-8;k<=8;++k){double v=k*PI/2;if(v>lo && v<hi)t.push_back(v);}
                t.push_back(hi);
                for(size_t j=1;j<t.size();++j){
                    double ya=z.c.y+R*sin(t[j-1]),yb=z.c.y+R*sin(t[j]);
                    if((ya>q.y)!=(yb>q.y)){
                        double dy=q.y-z.c.y;
                        double dx=sqrt(max(0.0,R*R-dy*dy));
                        double x=z.c.x+(cos((t[j-1]+t[j])/2)>0?dx:-dx);
                        if(x>q.x) inside=!inside;
                    }
                }
            }
        }
        printf("%.5f %.5f %c\n",q.x,q.y,inside?'+':'-');
    }
    return 0;
}
