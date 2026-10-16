// POJ 3833 - Hamlet's gambling
// Model: gpt-6.1-sol
// Let XY be the sum of 2^k over suffix-X/prefix-Y overlaps of length k.
// The winning odds are (BB-BA):(AA-AB). Compare BB+AB with AA+BA.
// KMP finds all overlaps; binary carries compare these huge integers exactly.
// This compares first-occurrence probabilities, not separate waiting times.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
void add(const string &x, const string &y, vector<int> &bits) {
    string s=y+"#"+x;
    vector<int> p(s.size(),0);
    for (int i=1;i<(int)s.size();++i) {
        int j=p[i-1];
        while(j && s[i]!=s[j]) j=p[j-1];
        if(s[i]==s[j]) ++j;
        p[i]=j;
    }
    for(int k=p[s.size()-1];k;k=p[k-1]) ++bits[k];
}
int main() {
    ios::sync_with_stdio(false);
    int n,m; string a,b;
    while(cin>>n>>m && (n||m)) {
        cin>>a>>b;
        int z=max(n,m)+3;
        vector<int> l(z,0),r(z,0);
        add(b,b,l); add(a,b,l);
        add(a,a,r); add(b,a,r);
        for(int i=0;i+1<z;++i) {
            l[i+1]+=l[i]/2; l[i]%=2;
            r[i+1]+=r[i]/2; r[i]%=2;
        }
        int c=0;
        for(int i=z-1;i>=0;--i) if(l[i]!=r[i]) {c=l[i]>r[i]?1:-1;break;}
        cout<<(c>0?"Hamlet":c<0?"Laertes":"Equal")<<'\n';
    }
}
