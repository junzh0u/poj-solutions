// POJ 3946 - Search of Concatenated Strings
// Model: gpt-6.1-sol
// A subset DP stores a bitset of all starting positions for that subset.
// Prepending an element intersects its occurrence bitset with the shifted
// smaller-subset bitset. Boolean union counts positions, not permutations.
// Text line breaks are ignored; overlapping occurrences remain distinct.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    while (cin >> n >> m && (n || m)) {
        vector<string> e(n);
        for (int i=0;i<n;++i) cin >> e[i];
        string t, line;
        for (int i=0;i<m;++i) { cin >> line; t += line; }
        int len=(int)t.size(), words=(len+1+31)/32, stride=words+1;
        vector<unsigned int> match(n*stride,0);
        for(int i=0;i<n;++i)
            for(int p=0;p+(int)e[i].size()<=len;++p)
                if(t.compare(p,e[i].size(),e[i])==0)
                    match[i*stride+p/32] |= 1U<<(p%32);
        int states=1<<n;
        vector<unsigned int> dp(states*stride,0);
        for(int p=0;p<=len;++p) dp[p/32] |= 1U<<(p%32);
        for(int mask=1;mask<states;++mask) {
            unsigned int *dest=&dp[mask*stride];
            for(int i=0;i<n;++i) if(mask&(1<<i)) {
                int shift=(int)e[i].size();
                const unsigned int *prev=&dp[(mask^(1<<i))*stride];
                const unsigned int *occ=&match[i*stride];
                for(int w=0;w<words;++w)
                    dest[w] |= occ[w] & ((prev[w]>>shift) | (prev[w+1]<<(32-shift)));
            }
        }
        int answer=0;
        for(int w=0;w<words;++w) {
            unsigned int x=dp[(states-1)*stride+w];
            while(x) { x &= x-1; ++answer; }
        }
        cout << answer << '\n';
    }
    return 0;
}
