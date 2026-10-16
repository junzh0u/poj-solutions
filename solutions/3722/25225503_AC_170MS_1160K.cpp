// POJ 3722 - We're Going In
// Model: gpt-6.1-sol
// Enumerate reflection, boundary-coverage and pairwise gap events in one
// period. The uncovered length is linear between events; its sliding
// integral is quadratic, so endpoints and stationary points suffice.
// Time and position are doubled to keep all event times integral.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int L,R,N,T,P;
vector<int> x,d,k;
vector<double> v,a;
int pos(int i,int t) {
 int z=(2*x[i]+d[i]*t)%P; if(z<0)z+=P;
 return z<=2*L?z:P-z;
}
void addroot(int l,int r,int u,int w,int c) {
 if(u==w)return;
 double t=l+(double)(c-u)/(w-u)*(r-l);
 if(t>=l && t<=r)k.push_back((int)(t+0.5));
}
double val(double t) {
 if(t>=P)t-=P;
 int i=upper_bound(k.begin(),k.end(),t)-k.begin()-1;
 if(i==(int)k.size()-1)return v[i];
 return v[i]+(v[i+1]-v[i])*(t-k[i])/(k[i+1]-k[i]);
}
double integ(double t) {
 double base=0; if(t>=P){base=a.back();t-=P;}
 int i=upper_bound(k.begin(),k.end(),t)-k.begin()-1;
 if(i==(int)k.size()-1)return base+a[i];
 double h=t-k[i];
 return base+a[i]+v[i]*h+0.5*(v[i+1]-v[i])*h*h/(k[i+1]-k[i]);
}
int main(){
 if(scanf("%d%d%d%d",&L,&R,&N,&T)!=4)return 0;
 x.resize(N);d.resize(N);
 for(int i=0;i<N;i++)scanf("%d%d",&x[i],&d[i]);
 if(!L||!T){puts("0.000");return 0;}
 if(!N||!R){printf("%.3f\n",(double)L*T);return 0;}
 if(R>=L){puts("0.000");return 0;}
 P=4*L;k.push_back(0);k.push_back(P);
 vector< vector<int> > b(N);
 for(int i=0;i<N;i++){
  b[i].push_back(0);b[i].push_back(P);
  int z=d[i]==1?2*(L-x[i]):2*x[i];
  for(;z<=P;z+=2*L)if(z>0)b[i].push_back(z);
  sort(b[i].begin(),b[i].end());b[i].erase(unique(b[i].begin(),b[i].end()),b[i].end());
  for(int j=0;j+1<(int)b[i].size();j++){
   int l=b[i][j],r=b[i][j+1],u=pos(i,l),w=pos(i,r);
   k.push_back(l);addroot(l,r,u,w,2*R);addroot(l,r,u,w,2*(L-R));
  }
 }
 for(int i=0;i<N;i++)for(int j=0;j<i;j++){
  vector<int> e=b[i];e.insert(e.end(),b[j].begin(),b[j].end());
  sort(e.begin(),e.end());e.erase(unique(e.begin(),e.end()),e.end());
  for(int h=0;h+1<(int)e.size();h++){
   int l=e[h],r=e[h+1],u=pos(i,l)-pos(j,l),w=pos(i,r)-pos(j,r);
   addroot(l,r,u,w,0);addroot(l,r,u,w,4*R);addroot(l,r,u,w,-4*R);
  }
 }
 sort(k.begin(),k.end());k.erase(unique(k.begin(),k.end()),k.end());
 for(int h=0;h<(int)k.size();h++){
  vector<int> p;for(int i=0;i<N;i++)p.push_back(pos(i,k[h]));sort(p.begin(),p.end());
  int g=max(0,p[0]-2*R)+max(0,2*L-p.back()-2*R);
  for(int i=1;i<N;i++)g+=max(0,p[i]-p[i-1]-4*R);
  v.push_back(g);
 }
 a.push_back(0);for(int i=1;i<(int)k.size();i++)a.push_back(a.back()+(v[i-1]+v[i])*0.5*(k[i]-k[i-1]));
 int rem=(2*T)%P;double base=(2*T/P)*a.back();
 vector<int> e=k;for(int i=0;i<(int)k.size();i++)e.push_back((k[i]-rem+P)%P);
 sort(e.begin(),e.end());e.erase(unique(e.begin(),e.end()),e.end());
 double best=0;
 for(int i=0;i+1<(int)e.size();i++){
  double l=e[i],r=e[i+1],f=integ(l+rem)-integ(l),g=val(l+rem)-val(l),gr=val(r+rem)-val(r);
  best=max(best,f);best=max(best,integ(r+rem)-integ(r));
  if(g!=gr){double h=-g*(r-l)/(gr-g);if(h>0&&h<r-l)best=max(best,f+g*h+0.5*(gr-g)*h*h/(r-l));}
 }
 printf("%.3f\n",(base+best)/4);
 return 0;
}
