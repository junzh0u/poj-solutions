// POJ 3477 - System of Linear Equations
// Model: gpt-6-astra
// Phase-I simplex minimizes the sum of nonnegative artificial variables.
// Each equation is signed to give a nonnegative initial right-hand side.
// Largest reduced cost pivots, with Bland's rule after 1000 iterations.
// Print a nonnegative witness at high precision, including dependent systems.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
const long double EPS=1e-12L;
long double a[101][202], x[101];
int basic[101];
bool used[201];
int main() {
    int m,n;
    if(!(cin>>m>>n)) return 0;
    int k=n+m;
    for(int i=0;i<m;++i) {
        for(int j=0;j<n;++j) cin>>a[i][j];
        cin>>a[i][k];
        if(a[i][k]<0) for(int j=0;j<=k;++j) a[i][j]=-a[i][j];
        a[i][n+i]=1; basic[i]=n+i; used[n+i]=true;
    }
    int iterations=0;
    for(;;) {
        ++iterations;
        long double best=EPS;
        int e=-1;
        for(int j=0;j<k;++j) if(!used[j]) {
            long double r=(j<n?0:-1);
            for(int i=0;i<m;++i) if(basic[i]>=n) r+=a[i][j];
            if(r>best) {e=j;best=r;if(iterations>1000) break;}
        }
        if(e<0) break;
        int l=-1;
        for(int i=0;i<m;++i) if(a[i][e]>EPS) {
            if(l<0) l=i;
            else {
                long double r=a[i][k]/a[i][e], s=a[l][k]/a[l][e];
                if(r<s-EPS || (fabsl(r-s)<=EPS && basic[i]<basic[l])) l=i;
            }
        }
        if(l<0) break;
        long double p=a[l][e];
        for(int j=0;j<=k;++j) a[l][j]/=p;
        a[l][e]=1;
        for(int i=0;i<m;++i) if(i!=l) {
            long double t=a[i][e];
            for(int j=0;j<=k;++j) if(j!=e) a[i][j]-=t*a[l][j];
            a[i][e]=0;
        }
        used[basic[l]]=false;basic[l]=e;used[e]=true;
    }
    long double residual=0;
    for(int i=0;i<m;++i) {
        if(basic[i]>=n) residual+=max(0.0L,a[i][k]);
        else x[basic[i]]=max(0.0L,a[i][k]);
    }
    if(residual>1e-10L) cout<<"impossible\n";
    else for(int j=0;j<n;++j) cout<<fixed<<setprecision(18)<<x[j]<<'\n';
}
