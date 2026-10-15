// POJ 1464 - Company
// Model: gpt-6.1-sol
// Branch-and-bound over linear-programming relaxations finds integer wages.
// Positivity and max(wage) <= 6*min(wage)-1 are required even when
// the weekly equations have free variables. A second feasible wage vector
// is sought by excluding each coordinate of the first solution in turn.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;
typedef long double Real;
typedef long long Int;
const Real EPS=1e-10L;
struct LP {
 int m,n; vector<int>B,N; vector<vector<Real> >D;
 LP(const vector<vector<Real> >&a,const vector<Real>&b):m(b.size()),n(a[0].size()),B(m),N(n+1),D(m+2,vector<Real>(n+2,0)) {
  for(int i=0;i<m;i++)for(int j=0;j<n;j++)D[i][j]=a[i][j];
  for(int i=0;i<m;i++){B[i]=n+i;D[i][n]=-1;D[i][n+1]=b[i];}
  for(int j=0;j<n;j++)N[j]=j;N[n]=-1;D[m+1][n]=1;
 }
 void pivot(int r,int s){Real inv=1/D[r][s];
  for(int i=0;i<m+2;i++)if(i!=r)for(int j=0;j<n+2;j++)if(j!=s)D[i][j]-=D[r][j]*D[i][s]*inv;
  for(int j=0;j<n+2;j++)if(j!=s)D[r][j]*=inv;
  for(int i=0;i<m+2;i++)if(i!=r)D[i][s]*=-inv;
  D[r][s]=inv;swap(B[r],N[s]);
 }
 bool simplex(int phase){int x=phase==1?m+1:m;
  for(;;){int s=-1;for(int j=0;j<=n;j++){
   if(phase==2&&N[j]==-1)continue;
   if(s==-1||D[x][j]<D[x][s]-EPS||(fabsl(D[x][j]-D[x][s])<=EPS&&N[j]<N[s]))s=j;
  }
  if(D[x][s]>=-EPS)return true;
  int r=-1;for(int i=0;i<m;i++)if(D[i][s]>EPS){
   if(r==-1||D[i][n+1]/D[i][s]<D[r][n+1]/D[r][s]-EPS||(fabsl(D[i][n+1]/D[i][s]-D[r][n+1]/D[r][s])<=EPS&&B[i]<B[r]))r=i;
  }
  if(r==-1)return false;pivot(r,s);
  }
 }
 bool solve(vector<Real>&x){int r=0;for(int i=1;i<m;i++)if(D[i][n+1]<D[r][n+1])r=i;
  if(D[r][n+1]<-EPS){pivot(r,n);if(!simplex(1)||D[m+1][n+1]<-1e-7L)return false;
   if(fabsl(D[m+1][n+1])>1e-7L)return false;
   for(int i=0;i<m;i++)if(B[i]==-1){int s=0;for(int j=1;j<=n;j++)if(D[i][j]<D[i][s]-EPS||(fabsl(D[i][j]-D[i][s])<=EPS&&N[j]<N[s]))s=j;if(fabsl(D[i][s])>EPS)pivot(i,s);}
  }
  simplex(2);x.assign(n,0);for(int i=0;i<m;i++)if(B[i]<n&&B[i]>=0)x[B[i]]=D[i][n+1];return true;
 }
};
int n; vector<vector<Real> >a; vector<Real>b;vector<vector<Int> >hours;vector<Int>paid,answer;
void addBound(int j,Real coeff,Real val){vector<Real>r(n+1,0);r[j]=coeff;a.push_back(r);b.push_back(val);}
Int gcdInt(Int x,Int y){while(y){Int z=x%y;x=y;y=z;}return x;}
bool search(){LP lp(a,b);vector<Real>x;if(!lp.solve(x))return false;
 int branch=-1;Real worst=0;vector<Int>v(n);
 for(int j=0;j<n;j++){v[j]=(Int)floorl(x[j]+0.5L);Real f=fabsl(x[j]-v[j]);if(f>worst){worst=f;branch=j;}}
 if(worst<1e-6L){bool ok=true;Int mn=v[0],mx=v[0];for(int j=0;j<n;j++){mn=min(mn,v[j]);mx=max(mx,v[j]);}
  if(mn<1||mx>=6*mn)ok=false;
  for(size_t i=0;i<hours.size();i++){Int sum=0;for(int j=0;j<n;j++)sum+=hours[i][j]*v[j];if(sum!=paid[i])ok=false;}
  if(ok){answer=v;return true;}
  // A nearly integral relaxation must still be split if exact verification fails.
  if(worst<1e-12L)return false;
 }
 Real cut=floorl(x[branch]);addBound(branch,1,cut);bool ok=search();a.pop_back();b.pop_back();if(ok)return true;
 addBound(branch,-1,-cut-1);ok=search();a.pop_back();b.pop_back();return ok;
}
int main(){int lines;while(cin>>lines){map<string,int>ids;vector<string>names;vector<vector<pair<int,Int> > >weeks;vector<pair<int,Int> >week;paid.clear();
 for(int i=0;i<lines;i++){string name;Int val;cin>>name>>val;if(name=="."){weeks.push_back(week);week.clear();paid.push_back(val);}else{if(!ids.count(name)){int id=names.size();ids[name]=id;names.push_back(name);}week.push_back(make_pair(ids[name],val));}}
 n=names.size();hours.assign(weeks.size(),vector<Int>(n,0));a.clear();b.clear();
 for(size_t i=0;i<weeks.size();i++){for(size_t j=0;j<weeks[i].size();j++)hours[i][weeks[i][j].first]+=weeks[i][j].second;
  vector<Real>r(n+1,0);for(int j=0;j<n;j++)r[j]=hours[i][j];a.push_back(r);b.push_back((Real)paid[i]);for(int j=0;j<n;j++)r[j]=-r[j];a.push_back(r);b.push_back(-(Real)paid[i]);}
 for(int j=0;j<n;j++){addBound(j,-1,-1);vector<Real>r(n+1,0);r[j]=-1;r[n]=1;a.push_back(r);b.push_back(0);r[j]=1;r[n]=-6;a.push_back(r);b.push_back(-1);}
 vector<Int>lo(n,1),hi(n,20000000000LL);bool impossible=false,changed=true;
 for(size_t i=0;i<hours.size();i++){Int g=0;for(int j=0;j<n;j++)g=gcdInt(g,hours[i][j]);if(g==0?paid[i]!=0:paid[i]%g!=0)impossible=true;}
 while(changed&&!impossible){changed=false;for(size_t i=0;i<hours.size();i++){
  Int low=0;for(int j=0;j<n;j++)low+=hours[i][j]*lo[j];
  if(low>paid[i]){impossible=true;break;}
  for(int j=0;j<n;j++)if(hours[i][j]){Int u=(paid[i]-low)/hours[i][j]+lo[j];if(u<hi[j]){hi[j]=u;changed=true;}}
 }
 Int minUpper=20000000000LL,maxLower=1;for(int j=0;j<n;j++){minUpper=min(minUpper,hi[j]);maxLower=max(maxLower,lo[j]);}
 for(int j=0;j<n;j++){Int u=6*minUpper-1,l=maxLower/6+1;if(hi[j]>u){hi[j]=u;changed=true;}if(lo[j]<l){lo[j]=l;changed=true;}if(lo[j]>hi[j])impossible=true;}
 }
 for(int j=0;j<n;j++){addBound(j,1,(Real)hi[j]);addBound(j,-1,-(Real)lo[j]);}
 if(n==0||impossible||!search())cout<<"Inconsistent data\n";
 else{vector<Int>first=answer;bool multiple=false;for(int j=0;j<n&&!multiple;j++){if(lo[j]==hi[j])continue;addBound(j,1,(Real)first[j]-1);multiple=search();a.pop_back();b.pop_back();if(!multiple){addBound(j,-1,-(Real)first[j]-1);multiple=search();a.pop_back();b.pop_back();}}
 if(multiple)cout<<"Incomplete data\n";else for(int j=0;j<n;j++)cout<<names[j]<<" "<<first[j]<<"\n";}
 cout<<".\n";
 }return 0;}
