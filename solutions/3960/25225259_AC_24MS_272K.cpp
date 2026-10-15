// POJ 3960 - Binary Operation
// Model: gpt-6.1-sol
// Treat each decimal position independently. A run of equal input digits is
// a power of a ten-state map; exponentiate full decimal cycles as maps too.
// Composition preserves the specified left-to-right order without assuming
// associativity of the digit operation. Print digits directly: the answer
// can exceed signed 64-bit range although both endpoints do not.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
typedef unsigned long long U;
struct Map { int v[10]; Map() { for(int i=0;i<10;++i)v[i]=i; } };
Map then(const Map &a,const Map &b) {
    Map c; for(int i=0;i<10;++i)c.v[i]=b.v[a.v[i]]; return c;
}
Map power(Map a,U n) {
    Map r; while(n) { if(n&1)r=then(r,a); a=then(a,a); n>>=1; } return r;
}
int op[10][10];
Map part(U offset,U count,U p) {
    Map r;
    while(count) {
        int d=int(offset/p); U take=min(count,p-offset%p);
        Map f; for(int x=0;x<10;++x)f.v[x]=op[x][d];
        r=then(r,power(f,take)); offset+=take; count-=take;
    }
    return r;
}
int main() {
    for(int i=0;i<10;++i)for(int j=0;j<10;++j)if(!(cin>>op[i][j]))return 0;
    U a,b; cin>>a>>b; string out;
    U p=1;
    for(int pos=0;pos<19;++pos,p*=10) {
        U cycle=p*10, n=b-a, offset=(a+1)%cycle;
        U take=min(n,cycle-offset);
        Map f=part(offset,take,p); n-=take;
        if(n>=cycle)f=then(f,power(part(0,cycle,p),n/cycle));
        n%=cycle; f=then(f,part(0,n,p));
        out+=char('0'+f.v[int(a/p%10)]);
    }
    while(out.size()>1 && out[out.size()-1]=='0')out.erase(out.size()-1);
    reverse(out.begin(),out.end()); cout<<out<<endl;
}
