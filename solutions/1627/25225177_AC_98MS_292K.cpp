// POJ 1627 - Experiment "X": Explosions Expected
// Model: gpt-6.1-sol
// Inclusion-exclusion of upward orthants, grouped by the sum of their
// coordinatewise maximum. Stars and bars counts each intersection.
// Empty and single-ingredient plans are excluded separately; every given
// failed experiment contains at least two positive coordinates.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int L=48, B=1000;
struct Big {
    int d[L];
    Big(int x=0) { memset(d,0,sizeof(d)); d[0]=x; }
    void mul(int x) { int c=0; for(int i=0;i<L;++i){int t=d[i]*x+c;d[i]=t%B;c=t/B;} }
    void divi(int x) { int c=0; for(int i=L-1;i>=0;--i){int t=c*B+d[i];d[i]=t/x;c=t%x;} }
    void add(const Big& a,int x) { int c=0;for(int i=0;i<L;++i){int t=d[i]+a.d[i]*x+c;d[i]=t%B;c=t/B;} }
    void sub(const Big& a) { int c=0;for(int i=0;i<L;++i){int t=d[i]-a.d[i]-c;c=t<0;if(c)t+=B;d[i]=t;} }
    void print() const {int i=L-1;while(i>0&&!d[i])--i;printf("%d",d[i]);while(i--)printf("%03d",d[i]);puts("");}
};
int k,s,m,a[20][30],v[21][30],coef[10001];
void dfs(int start,int sum,int sign,int depth) {
    coef[s-sum]+=sign;
    for(int j=start;j<m;++j){
        int ns=0;
        for(int t=0;t<k;++t){v[depth+1][t]=max(v[depth][t],a[j][t]);ns+=v[depth+1][t];}
        if(ns<=s)dfs(j+1,ns,-sign,depth+1);
    }
}
int main(){
    if(scanf("%d%d%d",&k,&s,&m)!=3)return 0;
    for(int i=0;i<m;++i)for(int j=0;j<k;++j)scanf("%d",&a[i][j]);
    dfs(0,0,1,0);
    Big ways(1),pos,neg;
    for(int r=0;r<=s;++r){
        if(coef[r]>0)pos.add(ways,coef[r]);
        if(coef[r]<0)neg.add(ways,-coef[r]);
        if(r<s){ways.mul(r+k+1);ways.divi(r+1);}
    }
    neg.add(Big(k*s+1),1);
    pos.sub(neg);pos.print();return 0;
}
