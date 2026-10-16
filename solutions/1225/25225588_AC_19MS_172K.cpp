// POJ 1225 - STRICTLY INSCRIBED SIMILAR TRIANGLES
// Model: gpt-6-astra
// Enumerate the four directed/reflected similarities and all edge assignments.
// Three supporting-line constraints determine translation and positive scale.
// The angle is signed counterclockwise modulo 180 degrees, not plus/minus theta.
// Strict interior tests reject vertices of the outer triangle.
// Accurate pi preserves right-angle endpoint exclusions in the official data.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double s)const{return P(x*s,y*s);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
struct Tri { P p[3]; };
bool solve(double a[3][4],double x[3]) {
 for(int c=0;c<3;c++) {
  int k=c; for(int i=c+1;i<3;i++) if(fabs(a[i][c])>fabs(a[k][c])) k=i;
  if(fabs(a[k][c])<1e-12) return false;
  for(int j=c;j<4;j++) swap(a[c][j],a[k][j]);
  double d=a[c][c]; for(int j=c;j<4;j++) a[c][j]/=d;
  for(int i=0;i<3;i++) if(i!=c) {d=a[i][c];for(int j=c;j<4;j++) a[i][j]-=d*a[c][j];}
 }
 for(int i=0;i<3;i++) x[i]=a[i][3]; return true;
}
int main(){
 int n; if(scanf("%d",&n)!=1)return 0;
 while(n--){
  P orig[3]; double th; for(int i=0;i<3;i++)scanf("%lf%lf",&orig[i].x,&orig[i].y);scanf("%lf",&th);
  P e=orig[1]-orig[0]; double l=sqrt(dot(e,e));e=e*(1/l);P f(-e.y,e.x);
  P c=orig[2]-orig[0];P p[3]={P(),P(1,0),P(dot(c,e)/l,dot(c,f)/l)};
  double a=th*acos(-1.0)/180,co=cos(a),si=sin(a);vector<Tri> ans;
  for(int sign=-1;sign<=1;sign+=2)for(int ref=-1;ref<=1;ref+=2){
   P r[3];for(int i=0;i<3;i++)r[i]=P(co*p[i].x-si*p[i].y*ref,si*p[i].x+co*p[i].y*ref)*sign;
   int perm[3]={0,1,2};do{
    double mat[3][4],x[3];for(int i=0;i<3;i++){
     P b=p[perm[i]],d=p[(perm[i]+1)%3]-b;double len=sqrt(dot(d,d));d=d*(1/len);
     mat[i][0]=-d.y;mat[i][1]=d.x;mat[i][2]=cross(d,r[i]);mat[i][3]=cross(d,b);
    }
    if(!solve(mat,x)||x[2]<=1e-9)continue;
    Tri t;bool ok=true;for(int i=0;i<3;i++){
     P q=P(x[0],x[1])+r[i]*x[2],b=p[perm[i]],d=p[(perm[i]+1)%3]-b;
     double u=dot(q-b,d)/dot(d,d);if(u<=1e-8||u>=1-1e-8)ok=false;
     t.p[i]=orig[0]+e*(q.x*l)+f*(q.y*l);
    }
    if(ok)ans.push_back(t);
   }while(next_permutation(perm,perm+3));
  }
  printf("%d\n",(int)ans.size());for(size_t j=0;j<ans.size();j++){
   puts("");for(int i=0;i<3;i++){double x=ans[j].p[i].x,y=ans[j].p[i].y;if(fabs(x)<0.00005)x=0;if(fabs(y)<0.00005)y=0;printf("%.4f %.4f\n",x,y);}puts("");
  }
 }
}
