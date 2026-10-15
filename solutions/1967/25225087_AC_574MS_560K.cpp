// POJ 1967 - Alibaba
// Model: gpt-6.1-sol
// Earliest feasible arrival at either end of each collected interval.
// Roll interval lengths in O(n) space; total time is O(n^2).
// Deadlines are strict: arriving exactly at the deadline is too late.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    const Int INF = 4000000000000000000LL;
    while (cin >> n) {
        vector<Int> x(n), deadline(n), left(n), right(n);
        for (int i=0; i<n; ++i) {
            cin >> x[i] >> deadline[i];
            left[i] = right[i] = deadline[i]>0 ? 0 : INF;
        }
        for (int len=2; len<=n; ++len) {
            for (int i=0, j=len-1; j<n; ++i, ++j) {
                Int a = min(left[i+1]+x[i+1]-x[i],
                            right[i+1]+x[j]-x[i]);
                Int b = min(left[i]+x[j]-x[i],
                            right[i]+x[j]-x[j-1]);
                left[i] = a<deadline[i] ? a : INF;
                right[i] = b<deadline[j] ? b : INF;
            }
        }
        Int answer = n ? min(left[0],right[0]) : 0;
        if (answer>=INF) cout << "No solution\n";
        else cout << answer << '\n';
    }
    return 0;
}
