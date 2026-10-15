// POJ 2055 - Kid's Problem
// Model: gpt-6.1-sol
// Count counter-clockwise turns with modular Gaussian elimination and
// branch-and-bound back substitution. Teeth are numbered counter-clockwise,
// so rotating a gear counter-clockwise decreases its top tooth number.
// Nonunit pivots retain wrap equations, preserving composite-modulus rules.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int k,n,col[20][20],target[20],best;
int mod(int v){v%=n;return v<0?v+n:v;}
int egcd(int a,int b,int &s,int &t){
 if(!b){s=1;t=0;return a;}
 int u,v,g=egcd(b,a%b,u,v);s=v;t=u-a/b*v;return g;
}
int mat[60][21],piv[20],val[20],nextcol[20];
bool finish[20];
void backtrack(int c,int used){
 if(used>=best)return;
 if(c<0){best=used;return;}
 if(finish[c]){
  int lower=0;
  for(int i=0;i<=c;++i)if(piv[i]>=0){
   int row=piv[i],t=mat[row][k];for(int j=c+1;j<k;++j)t=mod(t-mat[row][j]*val[j]);
   int v=0;while(v<n && mat[row][i]*v%n!=t)++v;
   if(v==n)return;lower+=v;
  }
  if(used+lower<best)best=used+lower;return;
 }
 int r=piv[c],rhs=0;
 if(r>=0){rhs=mat[r][k];for(int j=c+1;j<k;++j)rhs=mod(rhs-mat[r][j]*val[j]);}
 for(int v=0;v<n;++v)if(r<0 || mat[r][c]*v%n==rhs){val[c]=v;backtrack(c-1,used+v);}
}
void solve(){
 memset(mat,0,sizeof(mat));int rows=k,r=0;
 for(int i=0;i<k;++i){for(int j=0;j<k;++j)mat[i][j]=col[j][i];mat[i][k]=target[i];piv[i]=-1;}
 for(int c=0;c<k;++c){
  int q=r;while(q<rows && !mat[q][c])++q;
  if(q==rows)continue;
  for(int j=0;j<=k;++j)swap(mat[r][j],mat[q][j]);
  for(int i=r+1;i<rows;++i)while(mat[i][c]){
   int t=mat[r][c]/mat[i][c];
   for(int j=c;j<=k;++j){mat[r][j]=mod(mat[r][j]-t*mat[i][j]);swap(mat[r][j],mat[i][j]);}
  }
  int s,t,d=egcd(mat[r][c],n,s,t),u=1;
  while(mat[r][c]*u%n!=d || (egcd(u,n,s,t)!=1))++u;
  for(int j=c;j<=k;++j)mat[r][j]=mat[r][j]*u%n;
  if(d>1){for(int j=c+1;j<=k;++j)mat[rows][j]=mat[r][j]*(n/d)%n;++rows;}
  piv[c]=r++;
 }
 for(int i=r;i<rows;++i)if(mat[i][k]){best=1000000;return;}
 for(int c=k-1;c>=0;--c)if(piv[c]>=0){
  int row=piv[c],d=mat[row][c];
  for(int i=0;i<row;++i)if(mat[i][c]%d==0){int q=mat[i][c]/d;for(int j=c;j<=k;++j)mat[i][j]=mod(mat[i][j]-q*mat[row][j]);}
 }
 for(int c=0;c<k;++c){nextcol[c]=k;if(piv[c]>=0)for(int j=c+1;j<k;++j)if(mat[piv[c]][j]){nextcol[c]=j;break;}}
 for(int c=0;c<k;++c){
  finish[c]=true;
  for(int i=0;i<=c;++i){
   if(piv[i]>=0){if(nextcol[i]<=c)finish[c]=false;}
   else for(int z=0;z<r;++z)if(mat[z][i])finish[c]=false;
  }
 }
 best=k*n+1;backtrack(k-1,0);
}
int main(){
 while(scanf("%d%d",&k,&n)==2 && k){
  memset(col,0,sizeof(col));memset(target,0,sizeof(target));
  for(int i=0;i<k;++i){scanf("%d",&target[i]);target[i]=mod(1-target[i]);}
  for(int i=0;i<k;++i){int p;scanf("%d",&p);while(p--){int g,b;scanf("%d%d",&g,&b);col[i][g-1]=b;}}
  solve();if(best>k*(n-1))puts("No solution");else printf("%d\n",best);
 }
 return 0;
}
