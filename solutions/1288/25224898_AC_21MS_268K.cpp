// POJ 1288 - Sly Number
// Model: gpt-6.1-sol
// Solve the circulant system modulo each prime-power factor of Q, using
// unit pivots, and combine by CRT. An inverse in the finite polynomial
// ring is unique; its residues must have representatives in {0,1,2}.
// For Q=2, residue zero also permits coefficient 2.
#include <cstdio>
#include <algorithm>
using namespace std;
int a[50], mat[50][51], answer[50];
int inverse(int x,int m) {
    for(int i=1;i<m;++i) if(x*i%m==1) return i;
    return 0;
}
bool solve(int n,int q) {
    int remaining=q, combined=1;
    for(int i=0;i<n;++i) answer[i]=0;
    for(int p=2;p<=remaining;++p) if(remaining%p==0) {
        int m=1;
        while(remaining%p==0) { remaining/=p; m*=p; }
        for(int i=0;i<n;++i) {
            for(int j=0;j<n;++j) mat[i][j]=a[(i-j+n)%n]%m;
            mat[i][n]=(i==0);
        }
        for(int c=0;c<n;++c) {
            int r=c;
            while(r<n && mat[r][c]%p==0) ++r;
            if(r==n) return false;
            for(int j=c;j<=n;++j) swap(mat[c][j],mat[r][j]);
            int inv=inverse(mat[c][c],m);
            for(int j=c;j<=n;++j) mat[c][j]=mat[c][j]*inv%m;
            for(int i=0;i<n;++i) if(i!=c) {
                int factor=mat[i][c];
                for(int j=c;j<=n;++j)
                    mat[i][j]=(mat[i][j]-factor*mat[c][j]%m+m)%m;
            }
        }
        int inv=inverse(combined%m,m);
        for(int i=0;i<n;++i) {
            int delta=(mat[i][n]-answer[i]%m+m)%m;
            answer[i]+=combined*(delta*inv%m);
        }
        combined*=m;
    }
    for(int i=0;i<n;++i) if(answer[i]>2) return false;
    return true;
}
int main() {
    int cases,q,n;
    if(scanf("%d",&cases)!=1) return 0;
    while(cases--) {
        scanf("%d%d",&q,&n);
        for(int i=0;i<n;++i) scanf("%d",&a[i]);
        puts(solve(n,q)?"A solution can be found":"No solution");
    }
    return 0;
}
