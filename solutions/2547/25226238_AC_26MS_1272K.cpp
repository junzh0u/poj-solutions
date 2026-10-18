// POJ 2547 - No Tipping
// Model: gpt-6-astra
// Control: Waterloo 2001-06-02 official A.c algorithm, with explicit headers
// and C++98 main return type. Build balanced subsets and print removals while
// unwinding. Original: icpc.student.cs.uwaterloo.ca/~acm00/010602/data/A.c
#include <cstdio>
#include <cstring>
char reach[1<<24];
double w[24], x[24], len, wt, cg;
int n;
int solve(int m, double sx, double sw) {
   int i;
   if (reach[m]) return 0;
   reach[m] = 1;
   cg = sx / sw;
   if (cg < -1.5 || cg > 1.5) return 0;
   if (m == (1<<n)-1) return 1;
   for (i=0;i<n;i++) {
      if (!(m & (1<<i)) && solve(m|(1<<i),sx+x[i]*w[i],sw+w[i])) {
         printf("%lg %lg\n",x[i],w[i]);
         return 1;
      }
   }
   return 0;
}
int main(){
   int cc,i;
   for (cc=1; 3 == scanf("%lf%lf%d",&len,&wt,&n) && n; cc++) {
      memset(reach,0,1<<n);
      for (i=0;i<n;i++) scanf("%lf%lf",&x[i],&w[i]);
      printf("Case %d:\n",cc);
      if (!solve(0,0,wt)) printf("Impossible\n");
   }
   return 0;
}
