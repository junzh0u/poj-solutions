// POJ 1614 - Space AI Bombs
// Model: gpt-6.1-sol
// Group bombs by step and residue; reverse-search (vertex, frequency) states.
// A shield tests the incoming frequency, then allows zero or one step change.
// Planets do not change frequency. The discussion gives actual limits of
// 100 vertices, 500 edges and 500 bombs, smaller than the printed bounds.
#include <cstdio>
#include <vector>
#include <map>
#include <utility>
using namespace std;
struct Bomb { int h, f; Bomb(int a,int b):h(a),f(b){} };
int main() {
 int tests; if(scanf("%d", &tests)!=1) return 0;
 while(tests--) {
  int n,e,c,id,l,u; char word[32]; scanf("%d%d",&n,&e);
  vector<int> low(n,0),high(n,1000),kind(n,0),targets;
  vector< vector<int> > rev(n);
  scanf("%s%d",word,&c); while(c--) scanf("%d",&id);
  scanf("%s%d",word,&c); while(c--) {scanf("%d",&id);targets.push_back(id-1);}
  scanf("%s%d",word,&c); while(c--) {scanf("%d%d%d",&id,&l,&u);kind[id-1]=1;low[id-1]=l;high[id-1]=u;}
  scanf("%s%d",word,&c); while(c--) {scanf("%d%d",&l,&u);rev[u-1].push_back(l-1);}
  map< pair<int,int>, vector<Bomb> > groups;
  scanf("%s%d",word,&c); while(c--) {int h,f,d;scanf("%d%d%d",&h,&f,&d);if(d<0)d=-d;if(d==0 || d>1000)d=1001;groups[make_pair(d,f%d)].push_back(Bomb(h-1,f));}
  int answer=0;
  for(map< pair<int,int>, vector<Bomb> >::iterator it=groups.begin();it!=groups.end();++it) {
   int d=it->first.first, r=it->first.second, width=(1000-r)/d+1;
   vector<unsigned char> seen(n*width,0);vector<int> q; q.reserve(n*width);
   for(unsigned j=0;j<targets.size();++j) for(int k=0;k<width;++k) {int s=targets[j]*width+k;if(!seen[s]){seen[s]=1;q.push_back(s);}}
   for(unsigned head=0;head<q.size();++head) {
    int v=q[head]/width,k=q[head]%width;
    for(unsigned j=0;j<rev[v].size();++j) {
     int a=rev[v][j], first=k,last=k;
     if(kind[a]) {first=k-1;last=k+1;}
     for(int z=first;z<=last;++z) if(z>=0 && z<width) {
      int f=r+z*d,s=a*width+z;
      if(f>=low[a] && f<=high[a] && !seen[s]) {seen[s]=1;q.push_back(s);}
     }
    }
   }
   for(unsigned j=0;j<it->second.size();++j){Bomb b=it->second[j];answer+=seen[b.h*width+(b.f-r)/d]!=0;}
  }
  printf("%d\n",answer);
 }
 return 0;
}
