// POJ 3473 - Stochastic Finite State Automaton
// Model: gpt-6-astra
// Sum the zeroth, first and second moments of A^k b using one LU
// factorization of I-A. Columns describe outgoing probabilities.
// The halting decision itself is not counted as a transition.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const int N=205;
int n, piv[N];
long double a[N][N], lu[N][N], b[N], x[N], y[N], z[N], rhs[N], halt[N];
void solve(const long double *r, long double *v) {
    for(int i=0;i<n;++i) v[i]=r[i];
    for(int k=0;k<n;++k) swap(v[k],v[piv[k]]);
    for(int k=0;k<n;++k) {
        for(int i=k+1;i<n;++i) v[i]-=lu[i][k]*v[k];
    }
    for(int i=n-1;i>=0;--i) {
        for(int j=i+1;j<n;++j) v[i]-=lu[i][j]*v[j];
        v[i]/=lu[i][i];
    }
}
int main() {
    if(scanf("%d",&n)!=1) return 0;
    for(int j=0;j<n;++j) halt[j]=1;
    for(int i=0;i<n;++i) for(int j=0;j<n;++j) {
        scanf("%Lf",&a[i][j]); halt[j]-=a[i][j];
        lu[i][j]=(i==j?1.0L:0.0L)-a[i][j];
    }
    for(int i=0;i<n;++i) scanf("%Lf",&b[i]);
    for(int k=0;k<n;++k) {
        piv[k]=k;
        for(int i=k+1;i<n;++i) if(fabsl(lu[i][k])>fabsl(lu[piv[k]][k])) piv[k]=i;
        for(int j=0;j<n;++j) swap(lu[k][j],lu[piv[k]][j]);
        for(int i=k+1;i<n;++i) {
            lu[i][k]/=lu[k][k];
            for(int j=k+1;j<n;++j) lu[i][j]-=lu[i][k]*lu[k][j];
        }
    }
    solve(b,x);
    for(int i=0;i<n;++i) {
        rhs[i]=0;
        for(int j=0;j<n;++j) rhs[i]+=a[i][j]*x[j];
    }
    solve(rhs,y);
    for(int i=0;i<n;++i) {
        rhs[i]=0;
        for(int j=0;j<n;++j) rhs[i]+=a[i][j]*(2*y[j]+x[j]);
    }
    solve(rhs,z);
    for(int i=0;i<n;++i) {
        long double mean=x[i]>0?y[i]/x[i]:0;
        long double var=x[i]>0?z[i]/x[i]-mean*mean:0;
        printf("%.18Le %.18Le %.18Le\n",halt[i]*x[i],mean,sqrtl(max(0.0L,var)));
    }
    return 0;
}
