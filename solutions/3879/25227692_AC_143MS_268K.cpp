// POJ 3879 - Slalom
// Model: gpt-6-astra
// A shortest path bends only at gate endpoints. Dynamic programming over
// these endpoints uses an intersected interval of visible slopes, giving
// O(n^2) time and O(n) space. The finish may lie inside the final gate:
// choose the visible slope nearest zero, rather than only its endpoints.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
const long double INF = 1e100L;
long double x[1001][2], y[1001], d[1001][2];
int main() {
    ios::sync_with_stdio(false);
    int n;
    cout << fixed << setprecision(12);
    while (cin >> n && n) {
        cin >> x[0][0] >> y[0];
        x[0][1] = x[0][0];
        d[0][0] = d[0][1] = 0;
        for (int i=1; i<=n; ++i) {
            cin >> y[i] >> x[i][0] >> x[i][1];
            d[i][0] = d[i][1] = INF;
        }
        long double ans = INF;
        for (int i=0; i<n; ++i) for (int side=0; side<(i?2:1); ++side) {
            if (d[i][side] == INF) continue;
            long double lo=-INF, hi=INF, ox=x[i][side];
            for (int j=i+1; j<=n; ++j) {
                long double h=y[i]-y[j];
                long double a=(x[j][0]-ox)/h, b=(x[j][1]-ox)/h;
                for (int t=0; t<2; ++t) {
                    long double s=t?b:a;
                    if (lo<=s && s<=hi) {
                        long double dx=x[j][t]-ox;
                        d[j][t]=min(d[j][t],d[i][side]+sqrtl(dx*dx+h*h));
                    }
                }
                lo=max(lo,a); hi=min(hi,b);
                if (lo>hi) break;
                if (j==n) {
                    long double s=max(lo,min(hi,0.0L));
                    ans=min(ans,d[i][side]+h*sqrtl(1+s*s));
                }
            }
        }
        cout << ans << '\n';
    }
}
