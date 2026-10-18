// POJ 2988 - Function Approximation
// Model: gpt-6-astra
// The sign-function formulas select the original polyline and a polyline
// through exactly m+1 selected vertices. Precompute the integral of the
// absolute difference for each replacement chord, splitting at crossings.
// Dynamic programming chooses exactly m chords. Endpoint values have zero
// area; the displayed formulas use the selected indices as written.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
long double x[101], y[101], cost[101][101], dp[101][101];
int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;
    for (int i=0; i<=n; ++i) cin >> x[i] >> y[i];
    for (int i=0; i<n; ++i) for (int j=i+1; j<=n; ++j) {
        long double previous=0, sum=0;
        for (int k=i+1; k<=j; ++k) {
            long double current = k==j ? 0 :
                y[k]-y[i]-(y[j]-y[i])*(x[k]-x[i])/(x[j]-x[i]);
            long double a=fabs(previous), b=fabs(current), width=x[k]-x[k-1];
            if ((previous<0 && current>0) || (previous>0 && current<0))
                sum += width*(a*a+b*b)/(2*(a+b));
            else sum += width*(a+b)/2;
            previous=current;
        }
        cost[i][j]=sum;
    }
    for (int k=0; k<=m; ++k) for (int j=0; j<=n; ++j) dp[k][j]=1e100L;
    dp[0][0]=0;
    for (int k=1; k<=m; ++k) for (int j=k; j<=n; ++j)
        for (int i=k-1; i<j; ++i)
            dp[k][j]=min(dp[k][j],dp[k-1][i]+cost[i][j]);
    cout << setprecision(18) << dp[m][n] << '\n';
    return 0;
}
