// POJ 2359 - Questions
// Model: gpt-5.6-terra
// Approach: Josephus elimination with an order-statistics Fenwick tree.
#include <iostream>
#include <string>
using namespace std;
int bit[30005];
void add(int x,int v){for(;x<=30000;x+=x&-x)bit[x]+=v;}
int kth(int k){int x=0;for(int d=16384;d;d>>=1)if(x+d<=30000&&bit[x+d]<k){x+=d;k-=bit[x];}return x+1;}
int main(){string s,t;while(getline(cin,t))s+=t;int n=s.size(),p=0;for(int i=1;i<=n;++i)add(i,1);for(int left=n;left>1;--left){p=(p+1998)%left;add(kth(p+1),-1);if(p==left-1)p=0;}char c=s[kth(1)-1];if(c=='?')cout<<"Yes\n";else if(c==' ')cout<<"No\n";else cout<<"No comments\n";}
