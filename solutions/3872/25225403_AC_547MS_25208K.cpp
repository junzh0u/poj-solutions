// POJ 3872 - K-equivalence
// Model: gpt-6.1-sol
// Canonical decimal trie of the interval union. Digits are equivalent exactly
// when their children agree at every prefix. Leading zero padding is harmless:
// only nonzero digits are replaced, so decimal length never changes.
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
typedef unsigned long long U;
vector<U> a,b;
map<vector<int>,int> canonical;
bool eqv[10][10];
int build(U lo,U width) {
    int k=lower_bound(b.begin(),b.end(),lo)-b.begin();
    if(k==(int)b.size() || a[k]>lo+width-1) return 0;
    if(a[k]<=lo && b[k]>=lo+width-1) return 1;
    U step=width/10;
    vector<int> c(10);
    for(int d=0;d<10;++d) c[d]=build(lo+step*d,step);
    for(int p=1;p<=9;++p) for(int q=p+1;q<=9;++q)
        if(c[p]!=c[q]) eqv[p][q]=eqv[q][p]=false;
    map<vector<int>,int>::iterator it=canonical.find(c);
    if(it!=canonical.end()) return it->second;
    int id=canonical.size()+2;
    canonical[c]=id;
    return id;
}
int main(){
    int n;if(!(cin>>n))return 0;
    a.resize(n);b.resize(n);
    for(int i=0;i<n;++i)cin>>a[i]>>b[i];
    for(int p=1;p<=9;++p)for(int q=1;q<=9;++q)eqv[p][q]=true;
    U width=1;for(int i=0;i<19;++i)width*=10;
    build(0,width);
    bool used[10]={false};
    for(int p=1;p<=9;++p)if(!used[p]){
        for(int q=p;q<=9;++q)if(eqv[p][q]){cout<<q;used[q]=true;}
        cout<<'\n';
    }
}
