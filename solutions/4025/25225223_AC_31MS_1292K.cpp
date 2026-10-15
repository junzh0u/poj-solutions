// POJ 4025 - Dictionary Size
// Model: gpt-6.1-sol
// Sorted words expose each distinct prefix after the adjacent LCP.
// Repeat on reversed words for suffixes. Subtract adjacent valid split
// pairs by their shared letter, then add original one-letter words.
// POJ's statement is blank; use original NEERC 2011 problem D rules.
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int64;
Int64 countPrefixes(vector<string> &a, int count[26]) {
    sort(a.begin(),a.end());
    Int64 total=0;
    for(int i=0;i<(int)a.size();++i) {
        int common=0;
        if(i) while(common<(int)a[i].size() && common<(int)a[i-1].size()
                    && a[i][common]==a[i-1][common]) ++common;
        total+=(int)a[i].size()-common;
        for(int j=max(1,common);j<(int)a[i].size();++j) ++count[a[i][j]-'a'];
    }
    return total;
}
int main() {
    int n;
    while(scanf("%d",&n)==1) {
        vector<string> a;
        bool single[26]={false};
        char buf[100];
        for(int i=0;i<n;++i) {
            scanf("%s",buf); a.push_back(buf);
            if(a.back().size()==1) single[buf[0]-'a']=true;
        }
        int p[26]={0},s[26]={0};
        Int64 np=countPrefixes(a,p);
        for(int i=0;i<n;++i) reverse(a[i].begin(),a[i].end());
        Int64 ns=countPrefixes(a,s), answer=np*ns;
        for(int c=0;c<26;++c) {
            answer-=(Int64)p[c]*s[c];
            if(single[c]) ++answer;
        }
        printf("%lld\n",answer);
    }
    return 0;
}
