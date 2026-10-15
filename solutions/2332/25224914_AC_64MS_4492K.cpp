// POJ 2332 - One is good, but two is better
// Model: gpt-6.1-sol
// Enumerate the first rectangle. The bounding box of all 2s outside it
// is the smallest possible second rectangle, so shrinking to it never
// increases the union. Prefix sums reject 1s; strip bounding boxes give
// the uncovered bounding box in constant time. O(N^2 M^2) time.
// Rectangles follow matrix rows/columns; overlapping cells count once.
#include <cstdio>
#include <algorithm>
using namespace std;
struct Box {
    int t,b,l,r;
    Box():t(51),b(0),l(51),r(0){}
    void add(int x,int y) {t=min(t,x);b=max(b,x);l=min(l,y);r=max(r,y);}
    void merge(const Box &v) {t=min(t,v.t);b=max(b,v.b);l=min(l,v.l);r=max(r,v.r);}
    int area() const {return b ? (b-t+1)*(r-l+1):0;}
};
Box pre[51][51][52],suf[51][51][52],up[52],downbox[52];
int a[52][52],p[52][52];
int count1(int t,int b,int l,int r) {return p[b][r]-p[t-1][r]-p[b][l-1]+p[t-1][l-1];}
int main(){
    int n,m;if(scanf("%d%d",&n,&m)!=2)return 0;
    for(int i=1;i<=n;++i)for(int j=1;j<=m;++j){scanf("%d",&a[i][j]);p[i][j]=p[i-1][j]+p[i][j-1]-p[i-1][j-1]+(a[i][j]==1);}
    for(int i=1;i<=n;++i){up[i]=up[i-1];for(int j=1;j<=m;++j)if(a[i][j]==2)up[i].add(i,j);}
    for(int i=n;i>=1;--i){downbox[i]=downbox[i+1];for(int j=1;j<=m;++j)if(a[i][j]==2)downbox[i].add(i,j);}
    for(int t=1;t<=n;++t)for(int b=t;b<=n;++b){
        for(int j=1;j<=m;++j){pre[t][b][j]=pre[t][b][j-1];for(int i=t;i<=b;++i)if(a[i][j]==2)pre[t][b][j].add(i,j);}
        for(int j=m;j>=1;--j){suf[t][b][j]=suf[t][b][j+1];for(int i=t;i<=b;++i)if(a[i][j]==2)suf[t][b][j].add(i,j);}
    }
    int ans=10000;
    for(int t=1;t<=n;++t)for(int b=t;b<=n;++b)for(int l=1;l<=m;++l)for(int r=l;r<=m;++r){
        if(count1(t,b,l,r))continue;
        Box v=up[t-1];v.merge(downbox[b+1]);v.merge(pre[t][b][l-1]);v.merge(suf[t][b][r+1]);
        if(v.b && count1(v.t,v.b,v.l,v.r))continue;
        int overlap=0;
        if(v.b)overlap=max(0,min(b,v.b)-max(t,v.t)+1)*max(0,min(r,v.r)-max(l,v.l)+1);
        ans=min(ans,(b-t+1)*(r-l+1)+v.area()-overlap);
    }
    printf("%d\n",ans==10000?-1:ans);return 0;
}
