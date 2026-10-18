// POJ 3474 - Ellipsoid
// Model: gpt-6-astra
// Uniform interior samples have covariance eigenvectors along the principal
// axes, with eigenvalues proportional to squared axis lengths. Center the
// samples and diagonalize the symmetric covariance by Jacobi rotations.
// Report eigenvectors in decreasing eigenvalue order; either sign is valid.
// The recovery is statistical, as the statement supplies random samples.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double x[5005][3];
int main() {
    int n;
    if (scanf("%d", &n)!=1) return 0;
    double mean[3]={0,0,0}, a[3][3]={{0}}, v[3][3]={{0}};
    for(int i=0;i<n;++i) for(int j=0;j<3;++j) {
        scanf("%lf", &x[i][j]); mean[j]+=x[i][j];
    }
    for(int j=0;j<3;++j) { mean[j]/=n; v[j][j]=1; }
    for(int i=0;i<n;++i) for(int j=0;j<3;++j)
        for(int k=0;k<3;++k) a[j][k]+=(x[i][j]-mean[j])*(x[i][k]-mean[k]);
    for(int it=0;it<100;++it) {
        int p=0,q=1;
        for(int j=0;j<3;++j) for(int k=j+1;k<3;++k)
            if(fabs(a[j][k])>fabs(a[p][q])) { p=j;q=k; }
        double scale=fabs(a[0][0])+fabs(a[1][1])+fabs(a[2][2]);
        if(fabs(a[p][q])<=1e-15*scale) break;
        double angle=0.5*atan2(2*a[p][q],a[q][q]-a[p][p]);
        double c=cos(angle),s=sin(angle);
        double pp=a[p][p],qq=a[q][q],pq=a[p][q];
        a[p][p]=c*c*pp-2*c*s*pq+s*s*qq;
        a[q][q]=s*s*pp+2*c*s*pq+c*c*qq;
        a[p][q]=a[q][p]=0;
        for(int k=0;k<3;++k) {
            if(k!=p && k!=q) {
                double kp=a[k][p],kq=a[k][q];
                a[k][p]=a[p][k]=c*kp-s*kq;
                a[k][q]=a[q][k]=s*kp+c*kq;
            }
            double vp=v[k][p],vq=v[k][q];
            v[k][p]=c*vp-s*vq; v[k][q]=s*vp+c*vq;
        }
    }
    int order[3]={0,1,2};
    for(int j=0;j<3;++j) for(int k=j+1;k<3;++k)
        if(a[order[k]][order[k]]>a[order[j]][order[j]]) swap(order[j],order[k]);
    for(int j=0;j<3;++j) printf("%.15f %.15f %.15f\n",v[0][order[j]],v[1][order[j]],v[2][order[j]]);
    return 0;
}
