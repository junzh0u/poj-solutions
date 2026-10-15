// POJ 2024 - Know When to Hold 'em
// Model: gpt-6.1-sol
// Enumerate every five-card hand using at most two unseen cards.
// Compare poker values without suits; intersect tied hands to retain only
// mandatory cards, displaying interchangeable suits as '*'. Aces are high
// for output, but A2345 is a five-high straight for comparisons.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
using namespace std;
int board[5], unseen[45], hand[5], best, common[52], ranks[15], category;
const char *names[]={"", "", "", "THREE OF A KIND", "STRAIGHT", "FLUSH", "FULL HOUSE", "FOUR OF A KIND", "STRAIGHT FLUSH", "ROYAL FLUSH"};
int value() {
    int cnt[15]={0}, order[5], n=0, flush=1;
    for(int i=0;i<5;++i) {++cnt[hand[i]/4+2]; if(hand[i]%4!=hand[0]%4) flush=0;}
    for(int r=14;r>=2;--r) if(cnt[r]) order[n++]=r;
    int straight=0;
    if(n==5) {
        if(order[0]-order[4]==4) straight=order[0];
        else if(order[0]==14 && order[1]==5 && order[4]==2) straight=5;
    }
    int four=0,three=0,pair=0;
    for(int r=14;r>=2;--r) {if(cnt[r]==4)four=r; if(cnt[r]==3)three=r; if(cnt[r]==2)pair=r;}
    int cat=0, t[5]={0}, k=0;
    if(flush && straight) {cat=straight==14?9:8;t[k++]=straight;}
    else if(four) {cat=7;t[k++]=four;for(int r=14;r>=2;--r)if(cnt[r]==1)t[k++]=r;}
    else if(three && pair) {cat=6;t[k++]=three;t[k++]=pair;}
    else if(flush) {cat=5;for(int i=0;i<5;++i)t[k++]=order[i];}
    else if(straight) {cat=4;t[k++]=straight;}
    else if(three) {cat=3;t[k++]=three;for(int r=14;r>=2;--r)if(cnt[r]==1)t[k++]=r;}
    else return 0;
    int v=cat;for(int i=0;i<5;++i)v=v*15+t[i];return v;
}
void consider() {
    int v=value(); if(v<best || !v)return;
    int present[52]={0};for(int i=0;i<5;++i)present[hand[i]]=1;
    if(v>best) {
        best=v;memcpy(common,present,sizeof common);memset(ranks,0,sizeof ranks);
        for(int i=0;i<5;++i)++ranks[hand[i]/4+2];
    } else for(int i=0;i<52;++i)common[i]&=present[i];
}
int main() {
    int cases;cin>>cases;string s;string rr="23456789TJQKA",ss="cdhs";
    while(cases--) {
        bool used[52]={0};
        for(int i=0;i<7;++i){cin>>s;int c=int(rr.find(s[0]))*4+int(ss.find(s[1]));used[c]=true;if(i<5)board[i]=c;}
        int m=0;for(int c=0;c<52;++c)if(!used[c])unseen[m++]=c;
        best=0;
        for(int mask=0;mask<32;++mask){int k=0;for(int i=0;i<5;++i)if(mask&(1<<i))hand[k++]=board[i];
            if(k==5)consider();
            if(k==4)for(int a=0;a<m;++a){hand[4]=unseen[a];consider();}
            if(k==3)for(int a=0;a<m;++a)for(int b=a+1;b<m;++b){hand[3]=unseen[a];hand[4]=unseen[b];consider();}
        }
        for(int r=14;r>=2;--r){int left=ranks[r];for(int su=3;su>=0;--su)if(common[(r-2)*4+su]){cout<<rr[r-2]<<ss[su]<<' ';--left;}while(left-->0)cout<<rr[r-2]<<"* ";}
        int cat=best;for(int i=0;i<5;++i)cat/=15;cout<<names[cat]<<'\n';
    }
}
