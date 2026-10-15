// POJ 1865 - Practice
// Model: gpt-6.1-sol
// Maximize the concave logistic log likelihood with damped Newton steps.
// Center and scale the observations for a well-conditioned 2x2 Hessian.
// The requested finite maximum is found without multiplying probabilities.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
int k, w[105];
double x[105];
double loss(double a,double b) {
    double r=0;
    for(int i=0;i<k;++i) {
        double z=a+b*x[i];
        r += (z>0 ? z+log(1+exp(-z)) : log(1+exp(z)))-w[i]*z;
    }
    return r;
}
int main() {
    while(scanf("%d",&k)==1 && k) {
        double mean=0, scale=1;
        for(int i=0;i<k;++i) { scanf("%lf%d",&x[i],&w[i]); mean+=x[i]; }
        mean/=k;
        for(int i=0;i<k;++i) scale=max(scale,fabs(x[i]-mean));
        for(int i=0;i<k;++i) x[i]=(x[i]-mean)/scale;
        double a=0,b=0;
        for(int it=0;it<200;++it) {
            double g=0,h=0,A=0,B=0,C=0;
            for(int i=0;i<k;++i) {
                double z=a+b*x[i], e=exp(-fabs(z));
                double p=z>=0 ? 1/(1+e) : e/(1+e);
                double v=e/((1+e)*(1+e));
                g+=p-w[i]; h+=(p-w[i])*x[i];
                A+=v; B+=v*x[i]; C+=v*x[i]*x[i];
            }
            double det=A*C-B*B;
            if(det<=1e-30) break;
            double da=(C*g-B*h)/det, db=(A*h-B*g)/det;
            if(fabs(da-mean*db/scale)<1e-10 && fabs(db/scale)<1e-10) break;
            double step=1, old=loss(a,b);
            while(step>1e-10 && loss(a-step*da,b-step*db)>old+1e-13) step*=0.5;
            a-=step*da; b-=step*db;
        }
        double aa=a-b*mean/scale, bb=b/scale;
        if(fabs(aa)<0.00005) aa=0;
        if(fabs(bb)<0.00005) bb=0;
        printf("%.4f %.4f\n",aa,bb);
    }
    return 0;
}
