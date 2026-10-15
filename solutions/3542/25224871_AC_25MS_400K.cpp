// POJ 3542 - History of Football
// Model: gpt-6.1-sol
// Remove one team by enumerating its results, then memoize the sorted
// residual points. Sorting only relabels the remaining complete tournament;
// distinct result assignments are still added separately, preserving counts.
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
typedef long long Count;
map<unsigned long long, Count> memo[9];
Count solve(vector<int> a);
Count games(vector<int>& a, int j, int need) {
    int n = (int)a.size();
    if (need < 0 || need > 3*(n-j)) return 0;
    if (j == n) {
        vector<int> b(a.begin()+1, a.end());
        return solve(b);
    }
    Count ans = 0;
    const int own[3] = {0,1,3}, other[3] = {3,1,0};
    for (int k=0; k<3; ++k) {
        int left = a[j]-other[k];
        if (left < 0 || left > 3*(n-2)) continue;
        a[j] = left;
        ans += games(a,j+1,need-own[k]);
        a[j] += other[k];
    }
    return ans;
}
Count solve(vector<int> a) {
    int n=(int)a.size(), sum=0;
    if (n==0) return 1;
    sort(a.begin(),a.end());
    unsigned long long key=0;
    for(int i=0;i<n;++i) {
        if(a[i]<0 || a[i]>3*(n-1)) return 0;
        sum+=a[i]; key=(key<<5)|a[i];
    }
    int matches=n*(n-1)/2;
    if(sum<2*matches || sum>3*matches) return 0;
    if(n==1) return a[0]==0;
    map<unsigned long long,Count>::iterator it=memo[n].find(key);
    if(it!=memo[n].end()) return it->second;
    Count ans=games(a,1,a[0]);
    memo[n][key]=ans;
    return ans;
}
int main() {
    int n; if(!(cin>>n)) return 0;
    vector<int> a(n); for(int i=0;i<n;++i) cin>>a[i];
    cout<<solve(a)<<endl;
    return 0;
}
