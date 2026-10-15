// POJ 1919 - Ship Journey
// Model: gpt-6.1-sol
// Enumerate integer departure minutes and locate arrival in prefix distances.
// Distances are scaled by 60; compare fractional travel times exactly.
// Arrival must be strictly before the deadline, including exact-minute arrivals.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int cases; if (scanf("%d", &cases)!=1) return 0;
    while (cases--) {
        int deadline,n; scanf("%d%d", &deadline,&n);
        vector<pair<int,int> > changes(n);
        for(int i=0;i<n;++i) scanf("%d%d", &changes[i].first,&changes[i].second);
        sort(changes.begin(),changes.end());
        vector<int> prefix(deadline+1,0), speed(deadline,0);
        int k=0,v=10;
        for(int m=0;m<deadline;++m) {
            while(k<n && changes[k].first<=m) { v=10+changes[k].second; ++k; }
            speed[m]=v; prefix[m+1]=prefix[m]+v;
        }
        int answer=0,bestNum=100000000,bestDen=1;
        for(int t=0;t<deadline;++t) {
            int target=prefix[t]+6000;
            vector<int>::iterator it=lower_bound(prefix.begin()+t+1,prefix.end(),target);
            if(it==prefix.end()) continue;
            int j=int(it-prefix.begin())-1;
            int den=speed[j], num=(j-t)*den+target-prefix[j];
            if(num >= (deadline-t)*den) continue;
            if(num*bestDen <= bestNum*den) {
                bestNum=num; bestDen=den; answer=t;
            }
        }
        printf("%d\n",answer);
    }
    return 0;
}
