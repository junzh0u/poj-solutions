// POJ 3153 - Fool's Game
// Model: gpt-6.1-sol
// Memoized minimax over the two hands, table, and discarded cards.
// Attacks are played one at a time; after a refusal all legal toss subsets
// are considered. A successful defense discards the table and swaps roles.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int n, all, rankv[12], suit[12], beats[12], same[12];
int countv[4096], ranks[4096], enc[4096];
signed char memo[1<<24];
bool win(int a,int b,int t) {
    if (!a) return true;
    if (!b) return false;
    int key=enc[a]+2*enc[b]+3*enc[t];
    if(memo[key]) return memo[key]>0;
    bool result=false;
    // Stopping is legal only after at least one covered attack.
    if(t && !win(b,a,0)) result=true;
    int attacks=a;
    if(t) attacks &= ranks[t];
    for(int i=0; !result && i<n; ++i) if(attacks&(1<<i)) {
        int aa=a^(1<<i), tt=t|(1<<i);
        // On refusal, the attacker chooses the additional tossed cards.
        int eligible=aa&ranks[tt];
        bool refusal=false;
        for(int s=eligible;;s=(s-1)&eligible) {
            if(countv[s]<countv[b] && win(aa^s,b|tt|s,0)) {refusal=true;break;}
            if(!s) break;
        }
        if(!refusal) continue;
        bool forced=true;
        int covers=b&beats[i];
        for(int j=0; forced && j<n; ++j) if(covers&(1<<j))
            if(!win(aa,b^(1<<j),tt|(1<<j))) forced=false;
        if(forced) result=true;
    }
    memo[key]=result?1:-1;
    return result;
}
int main(){
    int n1,n2; char trump[4],s[8];
    if(scanf("%d%d%s",&n1,&n2,trump)!=3) return 0;
    n=n1+n2; all=(1<<n)-1;
    const char *rs="6789TJQKA";
    for(int i=0;i<n;++i){scanf("%s",s);rankv[i]=strchr(rs,s[0])-rs;suit[i]=s[1];}
    for(int i=0;i<n;++i)for(int j=0;j<n;++j){
        if(rankv[i]==rankv[j]) same[i]|=1<<j;
        if((suit[i]==suit[j] && rankv[j]>rankv[i]) || (suit[j]==trump[0] && suit[i]!=trump[0])) beats[i]|=1<<j;
    }
    for(int m=1;m<=all;++m){int i=0;while(!(m&(1<<i)))++i;int r=m^(1<<i);countv[m]=countv[r]+1;ranks[m]=ranks[r]|same[i];enc[m]=enc[r]|(1<<(2*i));}
    puts(win((1<<n1)-1,all^((1<<n1)-1),0)?"FIRST":"SECOND");
    return 0;
}
