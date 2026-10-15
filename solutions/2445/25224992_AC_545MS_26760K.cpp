// POJ 2445 - Squares
// Model: gpt-6.1-sol
// Merge drawn intervals, then compute continuous runs in four directions.
// Sweep each top-left/bottom-right diagonal with a Fenwick tree and expiry
// buckets, counting pairs whose two corner reaches cover their distance.
// Overlapping and adjacent input lines form a continuous side; cases end at EOF.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int N=1502;
int h[N][N],v[N][N];
short a[N][N],b[N][N];
int bit[N],head[N],nxt[N];
void add(int p,int x,int len){for(;p<=len;p+=p&-p)bit[p]+=x;}
int sum(int p){int s=0;for(;p;p-=p&-p)s+=bit[p];return s;}
int diagonal(int r,int c,int n){
    int len=min(n-r+1,n-c+1),answer=0;
    fill(bit,bit+len+1,0);fill(head,head+len+2,-1);
    for(int j=1;j<=len;++j){
        for(int p=head[j];p!=-1;p=nxt[p])add(p,-1,len);
        int back=b[r+j-1][c+j-1];
        answer+=sum(j-1)-sum(max(0,j-back-1));
        int reach=a[r+j-1][c+j-1];
        if(reach){
            add(j,1,len);
            int end=min(len+1,j+reach+1);
            nxt[j]=head[end];head[end]=j;
        }
    }
    return answer;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)==2){
        for(int i=0;i<=n+1;++i){fill(h[i],h[i]+n+2,0);fill(v[i],v[i]+n+2,0);}
        for(int z=0;z<m;++z){char ch;int i,j,k;scanf(" %c%d%d%d",&ch,&i,&j,&k);
            if(ch=='H'){++h[i][j];--h[i][j+k];}
            else{++v[i][j];--v[i+k][j];}
        }
        for(int i=1;i<=n;++i){int s=0;for(int j=1;j<=n;++j){s+=h[i][j];h[i][j]=(s>0);}}
        for(int j=1;j<=n;++j){int s=0;for(int i=1;i<=n;++i){s+=v[i][j];v[i][j]=(s>0);}}
        vector<int> down(n+2,0),up(n+2,0);
        for(int i=n;i>=1;--i){int right=0;for(int j=n;j>=1;--j){
            right=h[i][j]?right+1:0;down[j]=v[i][j]?down[j]+1:0;a[i][j]=min(right,down[j]);
        }}
        for(int i=1;i<=n;++i){int left=0;for(int j=1;j<=n;++j){
            left=h[i][j-1]?left+1:0;up[j]=v[i-1][j]?up[j]+1:0;b[i][j]=min(left,up[j]);
        }}
        int answer=0;
        for(int c=1;c<=n;++c)answer+=diagonal(1,c,n);
        for(int r=2;r<=n;++r)answer+=diagonal(r,1,n);
        printf("%d\n",answer);
    }
    return 0;
}
