// POJ 3796 - Spatial Concepts Test
// Model: gpt-6.1-sol
// Fold the given net into integer face frames. Enumerate the 24 ordered
// corner views and compare each face's distinguished-end vector.
// Edge directions in a corner view follow Figure 5, not net numbering.
#include <iostream>
#include <string>
using namespace std;
struct V { int x,y,z; V(int a=0,int b=0,int c=0):x(a),y(b),z(c){} };
V operator-(V a){return V(-a.x,-a.y,-a.z);}
V operator+(V a,V b){return V(a.x+b.x,a.y+b.y,a.z+b.z);}
V operator*(V a,int b){return V(a.x*b,a.y*b,a.z*b);}
bool operator==(V a,V b){return a.x==b.x&&a.y==b.y&&a.z==b.z;}
int dot(V a,V b){return a.x*b.x+a.y*b.y+a.z*b.z;}
V cross(V a,V b){return V(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
V n[6],u[6],r[6],d[6];
void neighbor(int a,int b,int dir){
 if(dir==0){n[b]=u[a];u[b]=-n[a];r[b]=r[a];}
 if(dir==1){n[b]=r[a];r[b]=-n[a];u[b]=u[a];}
 if(dir==2){n[b]=-u[a];u[b]=n[a];r[b]=r[a];}
 if(dir==3){n[b]=-r[a];r[b]=n[a];u[b]=u[a];}
}
bool valid(string s,string t){
 for(int i=0;i<6;i++){int k=s[2*i+1]-'1';d[i]=k==0?u[i]:k==1?r[i]:k==2?-u[i]:-r[i];}
 for(int a=0;a<6;a++)for(int b=0;b<6;b++)if(dot(n[a],n[b])==0){
  V c=cross(n[b],n[a]); int f=-1;for(int i=0;i<6;i++)if(n[i]==c)f=i;
  int ids[3]={a,b,f};
  V edges[3][4]={{c,-n[b],-c,n[b]}, {n[a],-c,-n[a],c}, {n[b],-n[a],-n[b],n[a]}};
  bool ok=true;for(int j=0;j<3;j++)if(s[2*ids[j]]!=t[2*j] || !(d[ids[j]]==edges[j][t[2*j+1]-'1']))ok=false;
  if(ok)return true;
 }
 return false;
}
int main(){
 n[2]=V(0,0,1);u[2]=V(0,1,0);r[2]=V(1,0,0);
 neighbor(2,0,0);neighbor(2,1,3);neighbor(2,3,1);neighbor(3,4,1);neighbor(2,5,2);
 int N;cin>>N;for(int cs=1;cs<=N;cs++){string s,t;cin>>s;bool ans[5];int sum=0;for(int i=0;i<5;i++){cin>>t;ans[i]=valid(s,t);sum+=ans[i];}cout<<cs<<' '<<sum;for(int i=0;i<5;i++)cout<<' '<<(ans[i]?'Y':'N');cout<<'\n';}
}
