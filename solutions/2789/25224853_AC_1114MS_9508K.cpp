// POJ 2789 - Pixel Shuffle
// Model: gpt-6.1-sol
// Compose the elementary pixel permutations, then take the LCM of cycle lengths.
// Operations act right to left; inverse operations reverse the same mapping.
// The figure defines mix as interleaving corresponding halves of adjacent rows.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int gcd(int a,int b) { while(b) { int t=a%b; a=b; b=t; } return a; }
int main() {
    int n;
    if(!(cin>>n)) return 0;
    vector<string> ops; string s;
    while(cin>>s) ops.push_back(s);
    int size=n*n, h=n/2;
    vector<int> p(size), q(size);
    for(int i=0;i<size;++i) p[i]=i;
    for(int k=(int)ops.size()-1;k>=0;--k) {
        s=ops[k]; bool inv=s[s.size()-1]=='-';
        if(inv) s.erase(s.size()-1);
        for(int r=0;r<n;++r) for(int c=0;c<n;++c) {
            int x=r,y=c;
            // Each formula gives the old position supplying output (r,c).
            if(s=="rot") { x=c; y=n-1-r; }
            else if(s=="sym") y=n-1-c;
            else if(s=="bhsym" && r>=h) y=n-1-c;
            else if(s=="bvsym" && r>=h) x=n+h-1-r;
            else if(s=="div") x=(r<h)?2*r:2*(r-h)+1;
            else if(s=="mix") { x=r-r%2+c%2; y=c/2+(r%2)*h; }
            int a=r*n+c, b=x*n+y;
            if(inv) q[b]=p[a]; else q[a]=p[b];
        }
        p.swap(q);
    }
    vector<char> seen(size,0);
    int answer=1;
    for(int i=0;i<size;++i) if(!seen[i]) {
        int len=0,j=i;
        do { seen[j]=1; ++len; j=p[j]; } while(j!=i);
        answer=answer/gcd(answer,len)*len;
    }
    cout<<answer<<'\n';
    return 0;
}
