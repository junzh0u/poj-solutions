// POJ 3966 - Hands of Poker
// Model: gpt-6-astra
// Rank directly within each category using colexicographic combination ranks.
// Remove excluded pair/triple ranks before ranking kickers. For high cards and
// flushes subtract the ten straights, including the ace-low wheel.
#include <stdio.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0
int choose(int n,int k) {
    if(n<k) return 0;
    int x=1;
    for(int i=1;i<=k;i++) x=x*(n-i+1)/i;
    return x;
}
int value(int *r,bool flush) {
    for(int i=0;i<5;i++) for(int j=i+1;j<5;j++) if(r[j]<r[i]) {int v=r[i];r[i]=r[j];r[j]=v;}
    int cnt[15]={0},p[2],np=0,t=0,q=0;
    for(int i=0;i<5;i++) cnt[r[i]]++;
    for(int i=2;i<=14;i++) {
        if(cnt[i]==2) p[np++]=i;
        if(cnt[i]==3) t=i;
        if(cnt[i]==4) q=i;
    }
    int st=0;
    bool distinct=true;
    for(int i=1;i<5;i++) if(r[i]==r[i-1]) distinct=false;
    if(distinct) {
        if(r[4]-r[0]==4) st=r[4];
        if(r[0]==2 && r[1]==3 && r[2]==4 && r[3]==5 && r[4]==14) st=5;
    }
    if(st && flush) return 7453+st-5;
    if(q) {
        int k=0; for(int i=2;i<=14;i++) if(cnt[i]==1) k=i;
        return 7297+(q-2)*12+k-2-(k>q);
    }
    if(t && np) return 7141+(t-2)*12+p[0]-2-(p[0]>t);
    if(st && !flush) return 5854+st-5;
    if(flush || (!np && !t)) {
        int rank=0;
        for(int i=0;i<5;i++) rank+=choose(r[i]-2,i+1);
        for(int high=5;high<=14;high++) {
            int s[5];
            for(int i=0;i<5;i++) s[i]=high-4+i;
            if(high==5) {s[0]=2;s[1]=3;s[2]=4;s[3]=5;s[4]=14;}
            bool less=false;
            for(int i=4;i>=0;i--) if(s[i]!=r[i]) {less=s[i]<r[i];break;}
            if(less) rank--;
        }
        return (flush?5864:1)+rank;
    }
    if(t) {
        int rank=0,j=1;
        for(int i=2;i<=14;i++) if(cnt[i]==1) rank+=choose(i-2-(i>t),j++);
        return 4996+(t-2)*66+rank;
    }
    if(np==2) {
        int k=0;for(int i=2;i<=14;i++) if(cnt[i]==1) k=i;
        return 4138+(choose(p[1]-2,2)+p[0]-2)*11+k-2-(k>p[0])-(k>p[1]);
    }
    int rank=0,j=1;
    for(int i=2;i<=14;i++) if(cnt[i]==1) rank+=choose(i-2-(i>p[0]),j++);
    return 1278+(p[0]-2)*220+rank;
}
int main(void) {
    FILE *contest=fopen("hands.in","r");
    if(contest) {fclose(contest);freopen("hands.in","r",stdin);freopen("hands.out","w",stdout);}
    const char *ranks="23456789TJQKA";
    int r[5]; char card[16],suit=0; bool flush=true;
    for(int i=0;i<5;i++) {
        if(scanf("%15s",card)!=1) return 0;
        r[i]=(int)(strchr(ranks,card[0])-ranks)+2;
        if(i==0) suit=card[1]; else if(card[1]!=suit) flush=false;
    }
    printf("%d\n",value(r,flush));
    return 0;
}
