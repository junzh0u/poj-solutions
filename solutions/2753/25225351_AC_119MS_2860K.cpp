// POJ 2753 - Similarity of necklaces
// Model: gpt-6.1-sol
// Start at all lower bounds. Off-diagonal increments consume two units
// of the full symmetric table sum; diagonal increments consume one.
// For each class, sorting coefficients gives a concave prefix-value curve.
// Binary search the concave objective over the off-diagonal increment count.
// The figure requires the full table sum to be zero; entries are integers.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long LL;
struct Item { int c, w; Item(int a,int b):c(a),w(b){} };
bool cmp(const Item&a,const Item&b){return a.c>b.c;}
struct Curve {
 vector<Item> a; vector<int> count; vector<LL> value;
 void build(){sort(a.begin(),a.end(),cmp);count.push_back(0);value.push_back(0);for(unsigned i=0;i<a.size();++i){count.push_back(count.back()+a[i].w);value.push_back(value.back()+(LL)a[i].c*a[i].w);}}
 LL get(int k)const {int p=int(upper_bound(count.begin(),count.end(),k)-count.begin())-1;if(p==(int)a.size())return value[p];return value[p]+(LL)(k-count[p])*a[p].c;}
 int marginal(int k)const {int p=int(lower_bound(count.begin(),count.end(),k)-count.begin())-1;return a[p].c;}
};
int main(){ios::sync_with_stdio(false);cin.tie(0);int n;while(cin>>n){Curve d,o;int r=0;LL base=0;for(int i=0;i<n;++i)for(int j=i;j<n;++j){int p,l,u;cin>>p>>l>>u;base+=(LL)p*l;r-=(i==j?1:2)*l;if(i==j)d.a.push_back(Item(p,u-l));else o.a.push_back(Item(p,u-l));}d.build();o.build();int lo=max(0,(r-d.count.back()+1)/2),hi=min(o.count.back(),r/2);while(lo<hi){int k=lo+(hi-lo)/2;int x=r-2*k;LL delta=(LL)o.marginal(k+1)-d.marginal(x)-d.marginal(x-1);if(delta>0)lo=k+1;else hi=k;}cout<<base+o.get(lo)+d.get(r-2*lo)<<'\n';}return 0;}
