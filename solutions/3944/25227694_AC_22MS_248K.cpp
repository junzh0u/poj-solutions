// POJ 3944 - Spherical Mirrors
// Model: gpt-6-astra
// Trace the nearest forward ray-sphere intersection, then reflect the
// direction across the radial normal. The last sphere cannot be hit again
// immediately because the reflected ray points out of its convex surface.
// The input guarantees one to five reflections and excludes grazing hits.
#include <cstdio>
#include <cmath>
struct Vec {
    double x,y,z;
    Vec(double a=0,double b=0,double c=0):x(a),y(b),z(c){}
    Vec operator+(const Vec& b)const{return Vec(x+b.x,y+b.y,z+b.z);}
    Vec operator-(const Vec& b)const{return Vec(x-b.x,y-b.y,z-b.z);}
    Vec operator*(double k)const{return Vec(x*k,y*k,z*k);}
    double dot(const Vec& b)const{return x*b.x+y*b.y+z*b.z;}
    Vec unit()const{return *this*(1.0/sqrt(dot(*this)));}
};
int main(){
    int n;
    Vec c[100]; double r[100];
    while(scanf("%d",&n)==1 && n){
        Vec p,d;
        scanf("%lf%lf%lf",&d.x,&d.y,&d.z);
        d=d.unit();
        for(int i=0;i<n;++i) scanf("%lf%lf%lf%lf",&c[i].x,&c[i].y,&c[i].z,&r[i]);
        int last=-1;
        for(int step=0;step<5;++step){
            double best=1e100; int hit=-1;
            for(int i=0;i<n;++i) if(i!=last){
                Vec v=c[i]-p;
                double along=v.dot(d);
                if(along<=0) continue;
                double disc=r[i]*r[i]-(v.dot(v)-along*along);
                if(disc<0) continue;
                double t=along-sqrt(disc);
                if(t>0 && t<best){best=t;hit=i;}
            }
            if(hit<0) break;
            p=p+d*best;
            Vec normal=(p-c[hit]).unit();
            d=(d-normal*(2*d.dot(normal))).unit();
            last=hit;
        }
        printf("%.4f %.4f %.4f\n",p.x,p.y,p.z);
    }
    return 0;
}
