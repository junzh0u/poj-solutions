// POJ 4020 - John's Inversions
// Model: gpt-6.1-sol
// Sort by red, breaking ties by blue, then merge-count strict blue inversions.
// Oppositely ordered pairs force one inversion; all other pairs can contribute
// zero simultaneously in this ordering. Equal values are not inversions.
// POJ's statement is empty; the original NEERC 2011 Northern statement supplies
// the constraints and strict comparison. Accept complete instances until EOF.
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;
struct Card { int r,b; };
Card cards[100005];
int a[100005], temp[100005];
bool cmp(const Card &x,const Card &y) {
    return x.r<y.r || (x.r==y.r && x.b<y.b);
}
long long countInv(int l,int r) {
    if(r-l<2) return 0;
    int m=(l+r)/2;
    long long ans=countInv(l,m)+countInv(m,r);
    int i=l,j=m,k=l;
    while(i<m && j<r) {
        if(a[i]<=a[j]) temp[k++]=a[i++];
        else { ans+=m-i; temp[k++]=a[j++]; }
    }
    while(i<m) temp[k++]=a[i++];
    while(j<r) temp[k++]=a[j++];
    for(i=l;i<r;++i) a[i]=temp[i];
    return ans;
}
int main() {
    int n;
    while(scanf("%d",&n)==1) {
        for(int i=0;i<n;++i) scanf("%d%d",&cards[i].r,&cards[i].b);
        sort(cards,cards+n,cmp);
        for(int i=0;i<n;++i) a[i]=cards[i].b;
        cout<<countInv(0,n)<<'\n';
    }
    return 0;
}
