// POJ 3206 - Ikki's Story III - Rescue frkstyc!
// Model: gpt-6.1-sol
// Add the greatest label as an isolated root or as a child of an older node.
// Attaching to one of k leaves preserves k; the other choices increase it.
// The discussion confirms rooted, labeled trees and childless leaves.
// Rolling Eulerian-number DP uses base 10^6 integers and processes all queries
// together, so the large-integer table is computed only once.
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct Big {
    int a[200], len;
    Big():len(0) {}
    void combine(const Big &b, int x, int y) {
        int m=max(len,b.len), carry=0;
        for(int i=0;i<m;++i) {
            int v=carry+(i<len?a[i]*x:0)+(i<b.len?b.a[i]*y:0);
            a[i]=v%1000000; carry=v/1000000;
        }
        len=m;
        if(carry) a[len++]=carry;
        while(len && a[len-1]==0) --len;
    }
    string str() const {
        if(!len) return "0";
        char buf[16]; sprintf(buf,"%d",a[len-1]); string s=buf;
        for(int i=len-2;i>=0;--i) {sprintf(buf,"%06d",a[i]); s+=buf;}
        return s;
    }
};
Big dp[501];
struct Query {int k,index; Query(int a,int b):k(a),index(b) {}};
vector<Query> queries[501];
int main() {
    int n,k,mx=0,count=0;
    while(scanf("%d%d",&n,&k)==2 && (n||k)) {
        queries[n].push_back(Query(k,count++)); mx=max(mx,n);
    }
    vector<string> answer(count);
    dp[0].len=1; dp[0].a[0]=1;
    for(n=0;n<=mx;++n) {
        if(n) {
            for(k=n;k>=1;--k) dp[k].combine(dp[k-1],k,n-k+1);
            dp[0].len=0;
        }
        for(size_t j=0;j<queries[n].size();++j) {
            Query q=queries[n][j]; answer[q.index]=dp[q.k].str();
        }
    }
    for(int i=0;i<count;++i) puts(answer[i].c_str());
    return 0;
}
