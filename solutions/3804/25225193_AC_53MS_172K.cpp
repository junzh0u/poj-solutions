// POJ 3804 - Swimming Jam
// Model: gpt-6.1-sol
// Process endpoint arrivals in time order. A lane preserves entry order,
// so an entrant finishes at max(start + pace, previous entrant's finish).
// All simultaneous arrivals turn together, sorted by natural pace.
// Each planned lap consists of two lane traversals.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
struct Swimmer { int pace, left, lane, arrival; };
bool faster(const Swimmer &a, const Swimmer &b) { return a.pace < b.pace; }
int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        vector<Swimmer> s(n);
        for (int i=0;i<n;++i) {
            scanf("%d%d", &s[i].pace, &s[i].left);
            s[i].left *= 2; s[i].lane=0;
        }
        sort(s.begin(),s.end(),faster);
        int last[2]={0,0};
        for (int i=0;i<n;++i) {
            s[i].arrival=s[i].pace;
            last[0]=s[i].arrival;
        }
        int active=n, now=0;
        while (active) {
            int next=1000000000;
            for (int i=0;i<n;++i) if (s[i].left) next=min(next,s[i].arrival);
            now=next;
            // The array is pace-sorted, hence each departing group is too.
            for (int i=0;i<n;++i) if (s[i].left && s[i].arrival==now) {
                if (--s[i].left==0) { --active; continue; }
                s[i].lane ^= 1;
                s[i].arrival=max(now+s[i].pace,last[s[i].lane]);
                last[s[i].lane]=s[i].arrival;
            }
        }
        printf("%d\n",now);
    }
    return 0;
}
