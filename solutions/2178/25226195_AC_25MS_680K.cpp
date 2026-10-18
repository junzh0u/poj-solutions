// POJ 2178 - Heroes Of Might And Magic
// Model: gpt-6-astra
// Control: C++98 translation of Roman Elizarov's NEERC 2002 jury solution
// https://neerc.ifmo.ru/archive/2002/solutions.zip (h_public.pas).
// Memoize the minimum hero health required at each mana/monster-health/position
// state, then follow the recorded optimal spell. Death is checked before the
// monsters respond; healing cannot exceed the initial hero health.
#include <cstdio>
#include <algorithm>
using namespace std;
int n,hp,mp,unit,num,v,dh,l[11];
int memo[51][101][11],spell[51][101][11];
int need(int t,int h,int p) {
    if (!t) return hp+1;
    int &best=memo[t][h][p];
    if (best) return best;
    int next=max(1,p-v), nh=h-l[p], action=11;
    if (nh<=0) best=1;
    else {
        best=need(t-1,nh,next);
        if (next==1) best+=(nh+unit-1)/unit;
        best=min(best,hp+1);
    }
    int cur=need(t-1,h,next), monsters=(h+unit-1)/unit;
    if (cur<=hp) {
        if (next==1) cur+=monsters;
        if (cur<=hp) {
            cur=max(1,cur-dh);
            if (cur<best) {best=cur;action=12;}
        }
    }
    for (int i=v+1;i<=n;++i) {
        next=i-v; cur=need(t-1,h,next);
        if (next==1) cur+=monsters;
        if (cur<best) {best=cur;action=i;}
    }
    spell[t][h][p]=action;
    return best;
}
int main() {
    if (scanf("%d%d%d%d%d%d%d",&n,&hp,&mp,&unit,&num,&v,&dh)!=7) return 0;
    for (int i=1;i<=n;++i) scanf("%d",&l[i]);
    int h=unit*num,p=n;
    if (need(mp,h,p)>hp) {puts("DEFEATED");return 0;}
    puts("VICTORIOUS");
    for (int t=mp;t>0;--t) {
        int a=spell[t][h][p];
        if (a==11) {puts("L");h-=l[p];}
        else if (a==12) puts("H");
        else {printf("T %d\n",a);p=a;}
        if (h<=0) break;
        p=max(1,p-v);
    }
    return 0;
}
