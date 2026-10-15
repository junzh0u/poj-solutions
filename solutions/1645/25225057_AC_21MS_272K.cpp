// POJ 1645 - BSP Trees
// Model: gpt-6.1-sol
// Encode each object's far/near side for the planes in input order.
// Descending binary codes give far-child-first BSP traversal; equal
// codes retain alphabetical order. No plane intersects an object, so
// its first vertex determines the side of the entire object.
#include <iostream>
#include <algorithm>
using namespace std;
struct Object { long double x,z; int code,id; } a[20];
bool before(const Object& a,const Object& b) {
    if(a.code!=b.code) return a.code>b.code;
    return a.id<b.id;
}
int main() {
    int n;
    if(!(cin>>n)) return 0;
    for(int i=0;i<n;++i) {
        int m; cin>>m;
        a[i].id=i; a[i].code=0;
        for(int j=0;j<m;++j) {
            long double x,z; cin>>x>>z;
            if(j==0) { a[i].x=x; a[i].z=z; }
        }
    }
    int p; cin>>p;
    for(int j=0;j<p;++j) {
        long double x1,z1,x2,z2; cin>>x1>>z1>>x2>>z2;
        if(x1>x2) { swap(x1,x2); swap(z1,z2); }
        for(int i=0;i<n;++i) {
            long double cross=(x2-x1)*(a[i].z-z1)-(z2-z1)*(a[i].x-x1);
            a[i].code=2*a[i].code+(cross>0 ? 1 : 0);
        }
    }
    sort(a,a+n,before);
    for(int i=0;i<n;++i) cout<<char('A'+a[i].id);
    cout<<'\n';
    return 0;
}
