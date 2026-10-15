// POJ 2814 - Mobiles
// Model: gpt-6.1-sol
// Each subtree weight is affine in the single unknown weight; every bar
// requires equal weights at its ends. Merge radial envelopes by depth to
// check clearance. Bars stay horizontal, and touching is not free motion.
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;
struct Node { char type; int l,r; double len,a,b; Node():type('D'),l(0),r(0),len(0),a(0),b(0){} };
struct Envelope { deque<double> v; double add; Envelope():add(0){} };
bool equalWeight(double x,double y) { return fabs(x-y)<=1e-8*max(1.0,max(fabs(x),fabs(y))); }
int main(){
 int id;
 while(cin>>id){
  if(id<0) break;
  vector<Node> n(1); int unknown=0,ending=id;
  while(id>0){
   if((int)n.size()<=id)n.resize(id+1);
   char t;cin>>t;n[id].type=t;
   if(t=='B')cin>>n[id].len>>n[id].l>>n[id].r;
   else {string s;cin>>s;if(s=="X"){n[id].a=1;unknown=id;}else n[id].b=atof(s.c_str());}
   cin>>id;ending=id;
  }
  vector<int> parent(n.size(),0),order;
  for(int i=1;i<(int)n.size();++i)if(n[i].type=='B'){parent[n[i].l]=i;parent[n[i].r]=i;}
  int root=1;while(parent[root])root=parent[root];
  order.push_back(root);
  for(size_t j=0;j<order.size();++j){int u=order[j];if(n[u].type=='B'){order.push_back(n[u].l);order.push_back(n[u].r);}}
  bool balanced=true,freeSwing=true;double answer=-1;
  vector<Envelope> e(n.size());
  for(int k=(int)order.size()-1;k>=0;--k){
   int u=order[k];
   if(n[u].type!='B'){e[u].v.push_back(0);continue;}
   int l=n[u].l,r=n[u].r;double da=n[l].a-n[r].a,db=n[l].b-n[r].b;
   if(da!=0){double x=-db/da;if(answer<0)answer=x;else if(!equalWeight(answer,x))balanced=false;if(x<=0)balanced=false;}
   else if(!equalWeight(db,0))balanced=false;
   n[u].a=n[l].a+n[r].a;n[u].b=n[l].b+n[r].b;
   size_t common=min(e[l].v.size(),e[r].v.size());
   for(size_t d=0;d<common;++d)if(e[l].v[d]+e[l].add+e[r].v[d]+e[r].add>=n[u].len-1e-9)freeSwing=false;
   if(e[l].v.size()<e[r].v.size())swap(l,r);
   e[u].v.swap(e[l].v);e[u].add=e[l].add;
   for(size_t d=0;d<e[r].v.size();++d)e[u].v[d]=max(e[u].v[d],e[r].v[d]+e[r].add-e[u].add);
   e[r].v.clear();e[u].add+=n[u].len/2;e[u].v.push_front(n[u].len/2-e[u].add);
  }
  if(!balanced||answer<=0)cout<<"The mobile cannot be balanced.\n";
  else {cout<<"Object "<<unknown<<" must have weight "<<fixed<<setprecision(2)<<answer<<"\n";cout<<"The mobile will "<<(freeSwing?"":"not ")<<"swing freely.\n";}
  if(ending<0)break;
 }
}
