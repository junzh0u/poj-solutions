// POJ 3933 - New Liquid
// Model: gpt-6.1-sol
// Integrate linear drag, then enumerate integer launch angles. For each,
// bisect the positive return time to the inclined bottom in scaled time f*t.
// Gravity is 980 cm/s^2. The sample has no leading count: read triples to EOF.
#include <cstdio>
#include <cmath>
const double PI = acos(-1.0);
double phi(double t) {
    if (t < 0.01)
        return 1.0+t*(-0.5+t*(1.0/6+t*(-1.0/24+t*(1.0/120+t*(-1.0/720+t/5040)))));
    return (1.0-exp(-t))/t;
}
double distance_at(int a, int v, int f, int theta) {
    double al=a*PI/180, th=theta*PI/180;
    double q=1+v*f*sin(th-al)/(980*cos(al));
    double lo=0, hi=q;
    for (int j=0;j<80;++j) {
        double mid=(lo+hi)/2;
        if (phi(mid)>1/q) lo=mid;
        else hi=mid;
    }
    double t=(lo+hi)/2;
    return v*cos(th)*t*phi(t)/f;
}
int main() {
    int a,v,f;
    while (scanf("%d%d%d",&a,&v,&f)==3) {
        int best=a+1; double longest=-1;
        for (int th=a+1;th<=90;++th) {
            double d=distance_at(a,v,f,th);
            if (d>longest) { longest=d; best=th; }
        }
        printf("%d\n",best);
    }
    return 0;
}
