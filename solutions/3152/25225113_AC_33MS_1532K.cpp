// POJ 3152 - Exchange
// Model: gpt-6.1-sol
// Maintain price/time ordered sets and aggregate quantities at each price.
// Trades use the resting order's price; message numbers identify cancellations.
// The prose reverses bid/ask once; the explicit matching rules determine order.
#include <cstdio>
#include <set>
#include <utility>
#include <algorithm>
using namespace std;
int qty[10005], price[10005], side[10005];
int total[2][100000];
set<pair<int,int> > book[2];
int main() {
    int n;
    if (scanf("%d", &n)!=1) return 0;
    for(int i=1;i<=n;++i) {
        char cmd[10]; scanf("%s",cmd);
        if(cmd[0]=='C') {
            int id; scanf("%d",&id);
            if(qty[id]) {
                int s=side[id], p=price[id];
                book[s].erase(make_pair(s ? p : -p,id));
                total[s][p]-=qty[id]; qty[id]=0;
            }
        } else {
            int q,p; scanf("%d%d",&q,&p);
            int s=(cmd[0]=='S'), other=1-s;
            side[i]=s; price[i]=p;
            while(q && !book[other].empty()) {
                int id=book[other].begin()->second, rp=price[id];
                if((s==0 && rp>p) || (s==1 && rp<p)) break;
                int traded=min(q,qty[id]);
                printf("TRADE %d %d\n",traded,rp);
                q-=traded; qty[id]-=traded; total[other][rp]-=traded;
                if(!qty[id]) book[other].erase(book[other].begin());
            }
            qty[i]=q;
            if(q) {
                book[s].insert(make_pair(s ? p : -p,i));
                total[s][p]+=q;
            }
        }
        int bp=book[0].empty() ? 0 : -book[0].begin()->first;
        int ap=book[1].empty() ? 99999 : book[1].begin()->first;
        printf("QUOTE %d %d - %d %d\n",total[0][bp],bp,total[1][ap],ap);
    }
    return 0;
}
