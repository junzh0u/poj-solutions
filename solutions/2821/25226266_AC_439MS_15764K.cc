// POJ 2821 - TN's Kingdom III - Assassination
// Model: gpt-6-astra
// Reversing beta and then rotating once makes gamma the cyclic convolution
// of alpha and beta. Divide their mixed-radix Fourier transforms, then invert.
// Each radix is at most 19. Roots are computed directly to avoid drift.
#include <cstdio>
#include <cmath>
#include <complex>
#include <vector>
using namespace std;
typedef complex<double> C;
int total;
vector<C> roots;
void transform(const C* in, int stride, C* out, int n, bool inverse) {
    if (n == 1) { out[0] = in[0]; return; }
    int p = 2;
    while (n % p) ++p;
    int m = n / p;
    for (int j=0;j<p;++j) transform(in+j*stride,stride*p,out+j*m,m,inverse);
    vector<C> tmp(n);
    int scale=total/n;
    for (int k=0;k<n;++k) {
        C sum(0,0);
        for (int j=0;j<p;++j) {
            int ix=(j*k)%n*scale;
            C w=inverse ? conj(roots[ix]) : roots[ix];
            sum+=out[j*m+k%m]*w;
        }
        tmp[k]=sum;
    }
    for (int k=0;k<n;++k) out[k]=tmp[k];
}
int main() {
    if (scanf("%d",&total)!=1) return 0;
    vector<C> b(total),g(total),fb(total),fg(total),a(total);
    double x;
    for(int i=0;i<total;++i) { scanf("%lf",&x); b[i]=C(x,0); }
    for(int i=0;i<total;++i) { scanf("%lf",&x); g[i]=C(x,0); }
    roots.resize(total);
    double pi=acos(-1.0);
    for(int i=0;i<total;++i) roots[i]=C(cos(2*pi*i/total),-sin(2*pi*i/total));
    transform(&b[0],1,&fb[0],total,false);
    transform(&g[0],1,&fg[0],total,false);
    for(int i=0;i<total;++i) fg[i]=norm(fb[i])<1e-24 ? C(0,0) : fg[i]/fb[i];
    transform(&fg[0],1,&a[0],total,true);
    for(int i=0;i<total;++i) printf("%.4f\n",a[i].real()/total);
    return 0;
}
