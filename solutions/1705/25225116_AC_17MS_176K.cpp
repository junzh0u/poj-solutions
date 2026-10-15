// POJ 1705 - Generational Replacement
// Model: gpt-6.1-sol
// Keep one FIFO per priority and a reference bit per cached address.
// On a miss, save the existing heads, fetch the new block, then move
// those heads from highest to lowest priority, clearing their bits.
// A fetched block precedes any head rotated back into the highest pool.
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
using namespace std;
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        static bool first=true;
        if (!first) puts("");
        first=false;
        deque<unsigned long> q[10];
        map<unsigned long,bool> ref;
        char s[100]; int count;
        while (scanf("%99s",s)==1 && s[0]!='#') {
            scanf("%d",&count);
            unsigned long a=strtoul(s,0,16);
            if (count<=0) continue;
            if (ref.find(a)!=ref.end()) ref[a]=true;
            else {
                bool had[10];
                for(int i=0;i<n;++i) had[i]=!q[i].empty();
                q[n-1].push_back(a); ref[a]=false;
                for(int i=n-1;i>=0;--i) if(had[i]) {
                    unsigned long b=q[i].front(); q[i].pop_front();
                    bool hit=ref[b]; ref[b]=false;
                    int dest=hit ? (i+1<n?i+1:i) : i-1;
                    if(dest>=0) q[dest].push_back(b);
                    else ref.erase(b);
                }
                if(count>1) ref[a]=true;
            }
        }
        for(int i=0;i<n;++i) {
            printf("%d:",i);
            for(deque<unsigned long>::iterator it=q[i].begin();it!=q[i].end();++it)
                printf(" 0x%08lx",*it);
            puts("");
        }
    }
    return 0;
}
