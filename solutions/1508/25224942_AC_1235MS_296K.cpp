// POJ 1508 - Skyscraper Floors
// Model: gpt-6.1-sol
// Join elevators whose arithmetic progressions intersect inside the building.
// Extended Euclid finds the first common stop at or above both starting floors.
// A == B needs no elevator; starts and intersections outside the building do not count.
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long Int;
Int step[100], start[100], floors;
int parent[100];
int root(int a) { return parent[a]==a?a:parent[a]=root(parent[a]); }
Int egcd(Int a, Int b, Int &x, Int &y) {
    if (!b) { x=1; y=0; return a; }
    Int u,v,g=egcd(b,a%b,u,v); x=v; y=u-(a/b)*v; return g;
}
bool stops(int i, Int f) { return f>=start[i] && (f-start[i])%step[i]==0; }
bool meets(int i,int j) {
    if(start[i]>=floors || start[j]>=floors) return false;
    if(step[i]>=floors) return stops(j,start[i]);
    if(step[j]>=floors) return stops(i,start[j]);
    Int u,v,g=egcd(step[i],step[j],u,v), d=start[j]-start[i];
    if(d%g) return false;
    Int modulus=step[j]/g;
    Int n=((d/g)*u)%modulus;
    if(n<0) n+=modulus;
    Int f=start[i]+step[i]*n, period=step[i]*modulus;
    Int low=max(start[i],start[j]);
    if(f<low) f+=((low-f+period-1)/period)*period;
    return f<floors;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int cases; cin>>cases;
    while(cases--) {
        int e; Int a,b; cin>>floors>>e>>a>>b;
        for(int i=0;i<e;++i) { cin>>step[i]>>start[i]; parent[i]=i; }
        for(int i=0;i<e;++i) for(int j=0;j<i;++j)
            if(meets(i,j)) parent[root(i)]=root(j);
        bool possible=(a==b);
        for(int i=0;i<e;++i) if(stops(i,a))
            for(int j=0;j<e;++j) if(stops(j,b) && root(i)==root(j)) possible=true;
        cout<<(possible?"It is possible to move the furniture.":"The furniture cannot be moved.")<<'\n';
    }
    return 0;
}
