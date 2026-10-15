// POJ 1424 - Diamonds
// Model: gpt-6.1-sol
// Transform (x,y) into (x+y,x-y+M-1); diamonds become rectangles.
// Prefix sums count each radius at every original grid intersection.
// Emit only increases of the guaranteed minimum, using positive radii.
// Input is coordinate pairs to EOF, without a point count; reject invalid pairs.
#include <cstdio>
#include <algorithm>
using namespace std;
int sum[202][202];
bool used[100][100];
int main() {
    int m,n,x,y;
    if (scanf("%d%d",&m,&n)!=2) return 0;
    int s=m+n-1,k=0;
    while(scanf("%d%d",&x,&y)==2) {
        if(x<0 || x>=n || y<0 || y>=m || used[y][x]) continue;
        used[y][x]=true; ++k;
        ++sum[x+y+1][x-y+m];
    }
    for(int i=1;i<=s;++i) for(int j=1;j<=s;++j)
        sum[i][j]+=sum[i-1][j]+sum[i][j-1]-sum[i-1][j-1];
    printf("Pmin  Rmin(Pmin)  Pmax(Pmin)\n");
    int previous=0;
    for(int r=1;r<=max(1,m+n-2) && previous<k;++r) {
        int lo=k,hi=0;
        for(y=0;y<m;++y) for(x=0;x<n;++x) {
            int u=x+y,v=x-y+m-1;
            int a=max(0,u-r),b=min(s-1,u+r);
            int c=max(0,v-r),d=min(s-1,v+r);
            int count=sum[b+1][d+1]-sum[a][d+1]-sum[b+1][c]+sum[a][c];
            lo=min(lo,count); hi=max(hi,count);
        }
        if(lo>previous) printf("%4d%12d%12d\n",lo,r,hi);
        previous=lo;
    }
    return 0;
}
