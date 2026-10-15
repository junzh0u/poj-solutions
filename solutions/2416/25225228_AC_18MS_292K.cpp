// POJ 2416 - Return of the Jedi
// Model: gpt-6.1-sol
// A shortest route among disjoint circular trees consists of common tangents
// and circular arcs. Build their visibility graph and run Dijkstra.
// Input sizes are diameters; travel at 200 miles/hour is 18 seconds/mile.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} };
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double len(P a){return sqrt(dot(a,a));}
P c[12]; double r[12];
vector<P> p; vector<int> owner;
vector<pair<int,double> > adj[600];
int add(P a,int o){if(o<2)return o; p.push_back(a);owner.push_back(o);return (int)p.size()-1;}
void edge(int a,int b,double w){adj[a].push_back(make_pair(b,w));adj[b].push_back(make_pair(a,w));}
bool clear(P a,P b,int i,int j,int n){P v=b-a;double d=dot(v,v);for(int k=2;k<n;k++){if(k==i||k==j)continue;double t=d?dot(c[k]-a,v)/d:0;t=max(0.0,min(1.0,t));double z=len(c[k]-(a+v*t));if(z<r[k]-1e-9)return false;}return true;}
int main(){int n;if(scanf("%d%lf%lf%lf%lf",&n,&c[0].x,&c[0].y,&c[1].x,&c[1].y)!=5)return 0;n+=2;for(int i=2;i<n;i++){scanf("%lf%lf%lf",&c[i].x,&c[i].y,&r[i]);r[i]*=.5;}
p.push_back(c[0]);p.push_back(c[1]);owner.push_back(0);owner.push_back(1);
for(int i=0;i<n;i++)for(int j=i+1;j<n;j++){
 P v=c[j]-c[i];double d=dot(v,v);if(d==0){if(i==0&&j==1)edge(0,1,0);continue;}
 for(int s=-1;s<=1;s+=2){double dr=r[i]-s*r[j],h=d-dr*dr;if(h<-1e-9)continue;h=sqrt(max(0.0,h));for(int t=-1;t<=1;t+=2){P u=(v*dr+P(-v.y,v.x)*(h*t))*(1/d);P a=c[i]+u*r[i],b=c[j]+u*(s*r[j]);if(clear(a,b,i,j,n)){int x=add(a,i),y=add(b,j);edge(x,y,len(b-a));}}}
}
int m=(int)p.size();const double pi=acos(-1.0);
for(int i=0;i<m;i++)for(int j=i+1;j<m;j++)if(owner[i]==owner[j]){int k=owner[i];double a=fabs(atan2(p[i].y-c[k].y,p[i].x-c[k].x)-atan2(p[j].y-c[k].y,p[j].x-c[k].x));a=min(a,2*pi-a);edge(i,j,a*r[k]);}
vector<double> dist(m,1e100);vector<int> used(m,0);dist[0]=0;for(int it=0;it<m;it++){int u=-1;for(int i=0;i<m;i++)if(!used[i]&&(u<0||dist[i]<dist[u]))u=i;if(u==1)break;used[u]=1;for(size_t k=0;k<adj[u].size();k++){int v=adj[u][k].first;dist[v]=min(dist[v],dist[u]+adj[u][k].second);}}
printf("%.2f\n",dist[1]*18);return 0;}
