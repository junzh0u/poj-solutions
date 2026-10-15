// POJ 3964 - Factorial Simplification
// Model: gpt-6.1-sol
// Track prime exponents, reject negative ones, then greedily divide by each
// factorial in descending order as many times as possible. A result factorial
// may exceed 10000, but cannot reach 10007 because that prime is absent.
#include <cstdio>
#include <vector>
#include <utility>
#include <climits>
using namespace std;
const int B = 10006;
int sp[B+1], pos[B+1], have[B+1], fac[B+1];
vector<int> primes;
void addFactorial(int x, int sign) {
    for (int j=0; j<(int)primes.size() && primes[j]<=x; ++j) {
        int t=x;
        while (t) { t/=primes[j]; have[j]+=sign*t; }
    }
}
int main() {
    for (int i=2;i<=B;++i) {
        if (!sp[i]) {
            pos[i]=(int)primes.size(); primes.push_back(i);
            for (int j=i;j<=B;j+=i) if (!sp[j]) sp[j]=i;
        }
    }
    int n,m,x;
    if (scanf("%d%d",&n,&m)!=2) return 0;
    for (int i=0;i<n;++i) { scanf("%d",&x); addFactorial(x,1); }
    for (int i=0;i<m;++i) { scanf("%d",&x); addFactorial(x,-1); }
    for (int j=0;j<(int)primes.size();++j) {
        if (have[j]<0) { puts("-1"); return 0; }
        int t=B;
        while (t) { t/=primes[j]; fac[j]+=t; }
    }
    vector< pair<int,int> > ans;
    for (int r=B;r>=2;--r) {
        int s=INT_MAX;
        for (int j=0;j<(int)primes.size() && primes[j]<=r;++j)
            if (have[j]/fac[j]<s) s=have[j]/fac[j];
        if (s>0) {
            ans.push_back(make_pair(r,s));
            for (int j=0;j<(int)primes.size() && primes[j]<=r;++j)
                have[j]-=s*fac[j];
        }
        for (int t=r;t>1;t/=sp[t]) --fac[pos[sp[t]]];
    }
    printf("%d\n",(int)ans.size());
    for (int i=0;i<(int)ans.size();++i) printf("%d %d\n",ans[i].first,ans[i].second);
    return 0;
}
