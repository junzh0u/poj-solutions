// POJ 3529 - Matrix Analysis
// Model: gpt-6.1-sol
// Let L collect the weighted entries strictly above and to the left.
// The recurrence uses the current layer there: B^k=(I-L)^(-1) B^(k-1).
// L^(m+n-1)=0, so the negative binomial expansion terminates.
#include <cstdio>
const int MOD=1000000007;
int a[20][20], v[39][20][20], inv[39];
int main(){
    int m,n,t;
    if(scanf("%d%d%d",&m,&n,&t)!=3) return 0;
    for(int i=0;i<m;i++) for(int j=0;j<n;j++) scanf("%d",&a[i][j]);
    for(int i=0;i<m;i++) for(int j=0;j<n;j++) scanf("%d",&v[0][i][j]);
    int d=m+n-2;
    for(int r=1;r<=d;r++) {
        int col[20]={0};
        for(int i=0;i<m;i++) {
            int row=0;
            for(int j=0;j<n;j++) {
                v[r][i][j]=(row+col[j])%MOD;
                int z=(int)((long long)a[i][j]*v[r-1][i][j]%MOD);
                row=(row+z)%MOD;
                col[j]=(col[j]+z)%MOD;
            }
        }
    }
    inv[1]=1;
    for(int r=2;r<=d;r++) inv[r]=(int)(MOD-(long long)(MOD/r)*inv[MOD%r]%MOD);
    while(t--) {
        int i,j,k; scanf("%d%d%d",&i,&j,&k); --i; --j;
        long long c=1,ans=v[0][i][j];
        for(int r=1;r<=i+j;r++) {
            c=c*(k+r-2)%MOD*inv[r]%MOD;
            ans=(ans+c*v[r][i][j])%MOD;
        }
        printf("%d\n",(int)ans);
    }
    return 0;
}
