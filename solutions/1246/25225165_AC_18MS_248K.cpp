// POJ 1246 - Hilbert Curve Intersections
// Model: gpt-6.1-sol
// Invert the four quadrant transformations for each query, counting the
// three joining edges separately. Memoization merges repeated subqueries.
// Integer query coordinates never coincide with half-integer curve vertices.
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
map<vector<int>, int> memo;
int countCross(int n, int axis, int a, int b, int c) {
    int s = 1 << n;
    a = max(a, 0); b = min(b, s);
    if (!n || a >= b || c <= 0 || c >= s) return 0;
    vector<int> key(5);
    key[0]=n; key[1]=axis; key[2]=a; key[3]=b; key[4]=c;
    map<vector<int>,int>::iterator it=memo.find(key);
    if (it!=memo.end()) return it->second;
    int h=s/2, ans=0;
    if (axis==0) {
        ans += countCross(n-1,0,a,b,c);
        ans += countCross(n-1,0,s-b,s-a,c);
        ans += countCross(n-1,1,h-b,h-a,c-h);
        ans += countCross(n-1,1,a-h,b-h,c-h);
        if (c==h) ans += (a==0) + (b==s);
    } else {
        ans += countCross(n-1,1,a,b,c);
        ans += countCross(n-1,1,a,b,s-c);
        ans += countCross(n-1,0,a-h,b-h,h-c);
        ans += countCross(n-1,0,a-h,b-h,c-h);
        if (c==h && a<h && b>=h) ++ans;
    }
    return memo[key]=ans;
}
int main() {
    int n,a,b,c;
    while (cin>>n && n) {
        cin>>a>>b>>c;
        memo.clear();
        cout<<countCross(n,0,a,b,c)<<'\n';
    }
    return 0;
}
