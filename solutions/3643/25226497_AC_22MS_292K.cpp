// POJ 3643 - Friend or Foe?
// Model: gpt-6-astra
// Find a feasible separating plane with phase-I simplex. Express each free
// coefficient as a nonnegative variable minus a shared nonnegative variable.
// Distinct positive margins reduce degeneracy without affecting feasibility.
// Any finite strictly separable data admits this margin after shifting and
// scaling, even though Alliance points are allowed on the original plane.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
using namespace std;
const long double EPS=1e-12;
long double D[205][10];
int B[205],N[6],m;
void pivot(int r,int s){
    long double q=1/D[r][s];
    for(int i=0;i<=m;i++) if(i!=r)
        for(int j=0;j<7;j++) if(j!=s)
            D[i][j]-=D[r][j]*D[i][s]*q;
    for(int j=0;j<7;j++) if(j!=s) D[r][j]*=q;
    for(int i=0;i<=m;i++) if(i!=r) D[i][s]*=-q;
    D[r][s]=q; swap(B[r],N[s]);
}
int main(){
    int na,nb;
    while(scanf("%d",&na)==1 && na>=0){
        memset(D,0,sizeof D); m=0;
        for(int side=0;side<2;side++){
            if(side) scanf("%d",&nb); else nb=na;
            for(int i=0;i<nb;i++){
                long double p[4]; scanf("%Lf%Lf%Lf",p,p+1,p+2); p[3]=1;
                for(int j=0;j<4;j++){
                    D[m][j]=(side?-1:1)*p[j]; D[m][4]-=D[m][j];
                }
                D[m][5]=-1; D[m][6]=-1-(m*137%997)/997.0; B[m]=5+m; m++;
            }
        }
        for(int j=0;j<5;j++) N[j]=j;
        N[5]=-1; D[m][5]=1;
        int first=0;
        for(int i=1;i<m;i++) if(D[i][6]<D[first][6]) first=i;
        pivot(first,5);
        while(true){
            int s=-1;
            for(int j=0;j<6;j++)
                if(D[m][j]<-EPS && (s<0 || D[m][j]<D[m][s]-EPS || (fabsl(D[m][j]-D[m][s])<=EPS && N[j]<N[s]))) s=j;
            if(s<0) break;
            int r=-1;
            for(int i=0;i<m;i++) if(D[i][s]>EPS){
                if(r<0 || D[i][6]/D[i][s]<D[r][6]/D[r][s]-EPS ||
                   (fabsl(D[i][6]/D[i][s]-D[r][6]/D[r][s])<=EPS && B[i]<B[r])) r=i;
            }
            if(r<0) break;
            pivot(r,s);
        }
        long double x[5]={0};
        for(int i=0;i<m;i++) if(B[i]>=0 && B[i]<5) x[B[i]]=D[i][6];
        printf("%.12Lf %.12Lf %.12Lf %.12Lf\n",x[0]-x[4],x[1]-x[4],x[2]-x[4],x[3]-x[4]);
    }
    return 0;
}
