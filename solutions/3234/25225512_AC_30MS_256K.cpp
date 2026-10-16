// POJ 3234 - The Kylin Den
// Model: gpt-6.1-sol
// Enumerate ordered subsets of walls and unfold the object backwards.
// A sequence is visible iff its ray meets each finite wall in order.
// Transparent crossings are unrestricted; coincident images count separately.
#include <cstdio>
#include <cmath>
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} };
P operator+(P a,P b){return P(a.x+b.x,a.y+b.y);}
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
P operator*(P a,double t){return P(a.x*t,a.y*t);}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
P a[8],b[8],eye,obj; int n,seq[8],answer=1;
P reflect(P p,int i){
 if(a[i].x==b[i].x)return P(2*a[i].x-p.x,p.y);
 if(a[i].y==b[i].y)return P(p.x,2*a[i].y-p.y);
 if(a[i].x+a[i].y==b[i].x+b[i].y){double c=a[i].x+a[i].y;return P(c-p.y,c-p.x);}
 double c=a[i].x-a[i].y;return P(p.y+c,p.x-c);
}
bool visible(int k){
 P im[9]; im[k]=obj;
 for(int j=k-1;j>=0;--j)im[j]=reflect(im[j+1],seq[j]);
 P p=eye;
 for(int j=0;j<k;++j){int i=seq[j];P d=im[j]-p,e=b[i]-a[i];double den=cross(d,e);
  if(std::fabs(den)<1e-10)return false;
  double t=cross(a[i]-p,e)/den,u=cross(a[i]-p,d)/den;
  if(t<=1e-10 || t>=1-1e-10 || u < -1e-10 || u > 1+1e-10)return false;
  p=p+d*t;
 }
 return true;
}
void dfs(int k,int mask){for(int i=0;i<n;++i)if(!(mask&(1<<i))){seq[k]=i;if(visible(k+1))++answer;dfs(k+1,mask|(1<<i));}}
int main(){if(scanf("%lf%lf%lf%lf",&eye.x,&eye.y,&obj.x,&obj.y)!=4)return 0;scanf("%d",&n);for(int i=0;i<n;++i)scanf("%lf%lf%lf%lf",&a[i].x,&a[i].y,&b[i].x,&b[i].y);dfs(0,0);printf("%d\n",answer);}
