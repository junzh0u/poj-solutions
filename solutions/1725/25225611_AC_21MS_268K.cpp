// POJ 1725 - BALL
// Model: gpt-6-astra
// Fix tile 1 and its orientation by rotational symmetry of the dodecahedron.
// Backtrack on the unfilled face with the fewest legal tile rotations.
// Neighbor lists are clockwise as viewed from outside, including both hemispheres.
#include <cstdio>
#include <cstring>
int adj[12][5]={{2,3,4,5,6},{1,6,7,11,3},{1,2,11,10,4},{1,3,10,9,5},{1,4,9,8,6},{1,5,8,7,2},{12,11,2,6,8},{12,7,6,5,9},{12,8,5,4,10},{12,9,4,3,11},{12,10,3,2,7},{7,8,9,10,11}};
int tile[12][5], chosen[12], rot[12], reverseEdge[12][5];
bool used[12], distinct[12][5];
bool fits(int f,int t,int r) {
 for(int k=0;k<5;k++) { int g=adj[f][k];
  if(chosen[g]>=0 && tile[t][(k-r+5)%5]!=tile[chosen[g]][(reverseEdge[f][k]-rot[g]+5)%5]) return false;
 }
 return true;
}
bool search(int remaining) {
 if(!remaining) return true;
 int best=-1, count=61, bt[60], br[60];
 for(int f=0;f<12;f++) if(chosen[f]<0) {
  int ts[60],rs[60],n=0;
  for(int t=0;t<12;t++) if(!used[t]) for(int r=0;r<5;r++)
   if(distinct[t][r] && fits(f,t,r)) {ts[n]=t;rs[n++]=r;}
  if(!n) return false;
  if(n<count) {best=f;count=n;for(int j=0;j<n;j++){bt[j]=ts[j];br[j]=rs[j];}}
 }
 for(int j=0;j<count;j++) {
  int t=bt[j]; chosen[best]=t;rot[best]=br[j];used[t]=true;
  if(search(remaining-1)) return true;
  used[t]=false;chosen[best]=-1;
 }
 return false;
}
int main() {
 int counts[3]={0,0,0};
 for(int t=0;t<12;t++) for(int k=0;k<5;k++) {
  if(scanf("%d",&tile[t][k])!=1) return 0;
  counts[tile[t][k]]++;
 }
 for(int c=0;c<3;c++) if(counts[c]%2) {puts("-1");return 0;}
 for(int f=0;f<12;f++) for(int k=0;k<5;k++) --adj[f][k];
 for(int f=0;f<12;f++) for(int k=0;k<5;k++)
  for(int j=0;j<5;j++) if(adj[adj[f][k]][j]==f) reverseEdge[f][k]=j;
 for(int t=0;t<12;t++) for(int r=0;r<5;r++) {
  distinct[t][r]=true;
  for(int s=0;s<r;s++) {
   bool equal=true;
   for(int k=0;k<5;k++) if(tile[t][(k+r)%5]!=tile[t][(k+s)%5]) equal=false;
   if(equal) distinct[t][r]=false;
  }
 }
 memset(chosen,-1,sizeof(chosen));chosen[0]=0;rot[0]=0;used[0]=true;
 if(search(11)) for(int f=0;f<12;f++) printf("%d %d\n",chosen[f]+1,adj[f][rot[f]]+1);
 else puts("-1");
 return 0;
}
