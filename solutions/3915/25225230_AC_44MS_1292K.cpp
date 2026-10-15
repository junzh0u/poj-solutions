// POJ 3915 - Black Vienna
// Model: gpt-6.1-sol
// Enumerate all deals consistent with each player's own hand. A different
// gang remains possible until the first interrogation that contradicts it.
// Everyone sees every answer, but knows only their own five cards initially.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int pc[1<<18];
int mask(const string &s) { int m=0; for(int i=0;i<(int)s.size();++i) m|=1<<(s[i]-'A'); return m; }
int main() {
    for(int i=1;i<(1<<18);++i) pc[i]=pc[i>>1]+(i&1);
    int t;
    while(cin>>t && t) {
        int actual[4]; string s;
        for(int i=0;i<4;++i) { cin>>s; actual[i]=mask(s); }
        int who[15], query[15], answer[15];
        for(int i=0;i<t;++i) { cin>>who[i]>>s>>answer[i]; --who[i]; query[i]=mask(s); }
        int result=t+1;
        for(int observer=0;observer<3;++observer) {
            int others[2], k=0;
            for(int i=0;i<3;++i) if(i!=observer) others[k++]=i;
            int available=((1<<18)-1)^actual[observer];
            int latest=0;
            for(int a=available;a;a=(a-1)&available) if(pc[a]==5) {
                int rem=available^a;
                for(int b=rem;b;b=(b-1)&rem) if(pc[b]==5) {
                    int gang=rem^b;
                    if(gang==actual[3]) continue;
                    int hand[3]; hand[observer]=actual[observer]; hand[others[0]]=a; hand[others[1]]=b;
                    int turn=0;
                    while(turn<t && pc[hand[who[turn]]&query[turn]]==answer[turn]) ++turn;
                    latest=max(latest,turn+1);
                }
            }
            result=min(result,latest);
        }
        if(result>t) cout<<"?\n"; else cout<<result<<'\n';
    }
    return 0;
}
