// POJ 3853 - Painting
// Model: gpt-6.1-sol
// Greedily fix the smallest feasible next color. Feasibility peels last
// strokes in reverse, reserving the chosen prefix until all others vanish.
// A color must fit one row or column; repeated occurrences are one stroke.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int m,n,k,a[100][100],rc[200],cc[200],cnt[200],vals[10000];
vector<int> rows[200],cols[200],prefix;
int rd[100],cd[100],rx[100],cx[100],baseR[100],baseC[100],xorR[100],xorC[100];
bool removed[200],reserved[200];
int q[1000],head,tail;
bool eligible(int c){return (rows[c].size()==1 && rd[rows[c][0]]==1) || (cols[c].size()==1 && cd[cols[c][0]]==1);}
void eraseColor(int c){
 removed[c]=true;
 for(unsigned i=0;i<rows[c].size();++i){int r=rows[c][i]; --rd[r];rx[r]^=c; if(rd[r]==1)q[tail++]=rx[r];}
 for(unsigned i=0;i<cols[c].size();++i){int s=cols[c][i]; --cd[s];cx[s]^=c; if(cd[s]==1)q[tail++]=cx[s];}
}
bool feasible(){
 for(int r=0;r<m;++r){rd[r]=baseR[r];rx[r]=xorR[r];}
 for(int c=0;c<n;++c){cd[c]=baseC[c];cx[c]=xorC[c];}
 head=tail=0;
 for(int c=0;c<k;++c){removed[c]=reserved[c]=false;}
 for(unsigned j=0;j<prefix.size();++j)reserved[prefix[j]]=true;
 for(int c=0;c<k;++c)if(!reserved[c]&&eligible(c))q[tail++]=c;
 int left=k;
 while(head<tail){int c=q[head++];if(!removed[c]&&!reserved[c]&&eligible(c)){eraseColor(c);--left;}}
 if(left!=(int)prefix.size())return false;
 for(int j=(int)prefix.size()-1;j>=0;--j){int c=prefix[j];if(!eligible(c))return false;eraseColor(c);}
 return true;
}
int main(){
 while(scanf("%d%d",&m,&n)==2 && m){
 vector<int> v;
 for(int r=0;r<m;++r)for(int c=0;c<n;++c){scanf("%d",&a[r][c]);v.push_back(a[r][c]);}
 sort(v.begin(),v.end());v.erase(unique(v.begin(),v.end()),v.end());k=v.size();
 for(int c=0;c<k;++c){vals[v[c]]=c;rows[c].clear();cols[c].clear();}
 for(int r=0;r<m;++r){baseR[r]=xorR[r]=0;bool seen[200]={false};for(int c=0;c<n;++c){int x=vals[a[r][c]];if(!seen[x]){seen[x]=true;rows[x].push_back(r);++baseR[r];xorR[r]^=x;}}}
 for(int c=0;c<n;++c){baseC[c]=xorC[c]=0;bool seen[200]={false};for(int r=0;r<m;++r){int x=vals[a[r][c]];if(!seen[x]){seen[x]=true;cols[x].push_back(c);++baseC[c];xorC[c]^=x;}}}
 prefix.clear();bool used[200]={false};
 for(int j=0;j<k;++j)for(int c=0;c<k;++c)if(!used[c]){prefix.push_back(c);if(feasible()){used[c]=true;break;}prefix.pop_back();}
 for(int j=0;j<k;++j)printf("%s%d",j?" ":"",v[prefix[j]]);puts("");
 }
}
