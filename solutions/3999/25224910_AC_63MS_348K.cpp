// POJ 3999 - Kind of a Blur
// Model: gpt-6.1-sol
// Each pixel gives a sum equation over its clipped Manhattan neighborhood.
// Solve the unique linear system by Gaussian elimination with absolute-value
// partial pivoting. Border averages use only pixels inside the image.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double a[100][101], x[100];
int main() {
    int w,h,d; bool first=true;
    while (scanf("%d%d%d",&w,&h,&d)==3 && (w||h||d)) {
        int n=w*h;
        for (int i=0;i<n;++i) {
            double b; scanf("%lf",&b); int count=0;
            for (int j=0;j<n;++j) {
                a[i][j]=(abs(i/w-j/w)+abs(i%w-j%w)<=d)?1.0:0.0;
                if (a[i][j]!=0) ++count;
            }
            a[i][n]=b*count;
        }
        for (int k=0;k<n;++k) {
            int p=k;
            for (int i=k+1;i<n;++i) if (fabs(a[i][k])>fabs(a[p][k])) p=i;
            for (int j=k;j<=n;++j) swap(a[k][j],a[p][j]);
            for (int i=k+1;i<n;++i) {
                double f=a[i][k]/a[k][k];
                a[i][k]=0;
                for (int j=k+1;j<=n;++j) a[i][j]-=f*a[k][j];
            }
        }
        for (int i=n-1;i>=0;--i) {
            double b=a[i][n];
            for (int j=i+1;j<n;++j) b-=a[i][j]*x[j];
            x[i]=b/a[i][i];
            if (fabs(x[i])<0.0000001) x[i]=0;
        }
        if (!first) puts(""); first=false;
        for (int i=0;i<h;++i) {
            for (int j=0;j<w;++j) printf("%8.2f",x[i*w+j]);
            puts("");
        }
    }
    return 0;
}
