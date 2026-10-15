// POJ 1812 - Zuma Game
// Model: gpt-6.1-sol
// Iterative deepening tries useful insertions and performs every forced
// chain reaction. Exact packed-state caching and necessary color counts prune
// failures. Insert next to its color, or split a pair to delay its removal;
// other setup insertions can be postponed until they meet their color.
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
const int SZ=131071;
struct Entry { unsigned a,b,c; } memo[SZ];
int hand[5], initial[5], lim;
const char *colors="RYBGW";
string reduce(string s) {
    for (;;) {
        bool changed=false; string t;
        for (int i=0;i<(int)s.size();) {
            int j=i+1; while(j<(int)s.size() && s[j]==s[i]) ++j;
            if(j-i>=3) changed=true; else t.append(s,i,j-i);
            i=j;
        }
        if(!changed) return s;
        s=t;
    }
}
bool dfs(const string &s,int left) {
    if(s.empty()) return true;
    if(!left) return false;
    int cnt[5]={0,0,0,0,0}, need=0;
    for(int i=0;i<(int)s.size();++i) ++cnt[(int)s[i]];
    for(int c=0;c<5;++c) if(cnt[c]) {
        if(cnt[c]+hand[c]<3) return false;
        if(cnt[c]<3) need+=3-cnt[c];
    }
    if(need>left) return false;
    unsigned key[3]={0,0,0};
    for(int i=0;i<(int)s.size();++i) {
        int bit=3*i; unsigned v=(unsigned)s[i]+1;
        key[bit/32]|=v<<(bit%32);
        if(bit%32>29) key[bit/32+1]|=v>>(32-bit%32);
    }
    unsigned hc=0; for(int c=0;c<5;++c) hc=hc*6+hand[c];
    key[2]|=(hc<<11); key[2]|=((unsigned)left<<25);
    unsigned h=(key[0]*1000003u+key[1]*9176u+key[2])%SZ;
    if(memo[h].a==key[0] && memo[h].b==key[1] && memo[h].c==key[2]) return false;
    for(int c=0;c<5;++c) if(hand[c]) {
        --hand[c];
        for(int p=0;p<=(int)s.size();++p) {
            if(p && s[p-1]==c) continue; // identical insertion within a run
            if(!((p<(int)s.size() && s[p]==c) ||
                 (p>0 && p<(int)s.size() && s[p-1]==s[p]))) continue;
            string t=s; t.insert(t.begin()+p,(char)c); t=reduce(t);
            if(dfs(t,left-1)) { ++hand[c]; return true; }
        }
        ++hand[c];
    }
    memo[h].a=key[0]; memo[h].b=key[1]; memo[h].c=key[2];
    return false;
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        int m,k; char b[40],h[10]; scanf("%d%d%s%s",&m,&k,b,h);
        string s; memset(initial,0,sizeof(initial));
        for(int i=0;i<m;++i) s+=(char)(strchr(colors,b[i])-colors);
        for(int i=0;i<k;++i) ++initial[strchr(colors,h[i])-colors];
        int ans=-1;
        for(lim=1;lim<=k;++lim) {
            memcpy(hand,initial,sizeof(hand)); memset(memo,255,sizeof(memo));
            if(dfs(s,lim)) { ans=lim; break; }
        }
        if(ans<0) puts("lose"); else printf("%d\n",ans);
    }
}
