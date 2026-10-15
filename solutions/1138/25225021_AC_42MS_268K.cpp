// POJ 1138 - Ships
// Model: gpt-6.1-sol
// Enumerate legal fleet completions with an MRV exact-cover search.
// A safe guess has exactly one distinct completion on its miss branch;
// take its hit branch and repeat. Occupancy, not ship partition, is unique.
// POJ input dimensions are height, width (unlike the original statement).
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
struct Bits { unsigned int a[8]; Bits(){memset(a,0,sizeof(a));} void set(int p){a[p/32]|=1U<<(p%32);} bool has(int p)const{return (a[p/32]>>(p%32))&1U;} };
Bits join(const Bits&x,const Bits&y){Bits z;for(int i=0;i<8;i++)z.a[i]=x.a[i]|y.a[i];return z;}
bool overlap(const Bits&x,const Bits&y){for(int i=0;i<8;i++)if(x.a[i]&y.a[i])return true;return false;}
bool equal(const Bits&x,const Bits&y){for(int i=0;i<8;i++)if(x.a[i]!=y.a[i])return false;return true;}
struct Place{Bits b;int t;};
vector<Place> ps;vector<int> bytype[7],bycell[256];
Bits need,ban,first;int found;
bool valid(int id,const Bits&used){return !overlap(ps[id].b,used)&&!overlap(ps[id].b,ban);}
void dfs(int types,const Bits&used){
 if(found==2)return;
 if(types==127){for(int i=0;i<8;i++)if(need.a[i]&~used.a[i])return;if(!found){first=used;found=1;}else if(!equal(first,used))found=2;return;}
 vector<int> best;int count=100000;
 for(int c=0;c<256;c++)if(need.has(c)&&!used.has(c)){
  vector<int> v;for(size_t k=0;k<bycell[c].size();k++){int id=bycell[c][k];if(!(types&(1<<ps[id].t))&&valid(id,used))v.push_back(id);}
  if(v.empty())return;if((int)v.size()<count){count=v.size();best.swap(v);if(count==1)break;}
 }
 if(count==100000)for(int t=0;t<7;t++)if(!(types&(1<<t))){vector<int> v;for(size_t k=0;k<bytype[t].size();k++)if(valid(bytype[t][k],used))v.push_back(bytype[t][k]);if(v.empty())return;if((int)v.size()<count){count=v.size();best.swap(v);}}
 for(size_t k=0;k<best.size()&&found<2;k++){int id=best[k];dfs(types|(1<<ps[id].t),join(used,ps[id].b));}
}
int completions(int forbidden){Bits old=ban;if(forbidden>=0)ban.set(forbidden);found=0;dfs(0,Bits());ban=old;return found;}
int main(){int w,h,game=0;while(scanf("%d%d",&h,&w)==2&&w&&h){
 ps.clear();for(int i=0;i<7;i++)bytype[i].clear();for(int i=0;i<256;i++)bycell[i].clear();need=Bits();ban=Bits();vector<int> dots;char row[40];
 for(int r=0;r<h;r++){scanf("%s",row);for(int c=0;c<w;c++){int p=r*w+c;if(row[c]=='x')need.set(p);else if(row[c]=='o')ban.set(p);else dots.push_back(p);}}
 int shape[7][4][2]={{{0,0},{0,1},{1,0},{1,1}},{{0,0},{0,1},{1,1},{1,2}},{{0,1},{0,2},{1,0},{1,1}},{{0,0},{1,0},{1,1},{1,2}},{{0,2},{1,0},{1,1},{1,2}},{{0,1},{1,0},{1,1},{1,2}},{{0,0},{0,1},{0,2},{0,3}}};
 for(int t=0;t<7;t++){vector<vector<pair<int,int> > > seen;for(int rot=0;rot<4;rot++){vector<pair<int,int> > s;int mr=100,mc=100;for(int k=0;k<4;k++){int r=shape[t][k][0],c=shape[t][k][1];for(int z=0;z<rot;z++){int tmp=r;r=c;c=-tmp;}s.push_back(make_pair(r,c));mr=min(mr,r);mc=min(mc,c);}for(int k=0;k<4;k++){s[k].first-=mr;s[k].second-=mc;}sort(s.begin(),s.end());if(find(seen.begin(),seen.end(),s)!=seen.end())continue;seen.push_back(s);
 for(int r=0;r<h;r++)for(int c=0;c<w;c++){Place p;p.t=t;bool ok=true;for(int k=0;k<4;k++){int rr=r+s[k].first,cc=c+s[k].second;if(rr>=h||cc>=w){ok=false;break;}p.b.set(rr*w+cc);}if(ok&&!overlap(p.b,ban)){int id=ps.size();ps.push_back(p);bytype[t].push_back(id);for(int k=0;k<4;k++)bycell[(r+s[k].first)*w+c+s[k].second].push_back(id);}}}}
 bool yes=false;while(true){if(completions(-1)<2){yes=true;break;}bool progress=false;for(size_t k=0;k<dots.size();k++){int c=dots[k];if(need.has(c))continue;int n=completions(c);if(n<2){need.set(c);progress=true;break;}}if(!progress)break;}
 printf("Game #%d\n%s.\n\n",++game,yes?"yes":"no");
 }return 0;}
