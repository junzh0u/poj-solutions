// POJ 3963 - Evacuation Plan
// Model: gpt-6-astra
// Sort teams and shelters: uncrossing assignments never increases distance.
// DP assigns each ordered shelter a nonempty consecutive group of teams.
// Rolling costs and one-byte predecessor choices allow full reconstruction.
// Return shelter indices and team order from the original unsorted input.
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    if (!(cin >> n)) return 0;
    vector<pair<int,int> > a(n), b;
    for (int i=0;i<n;++i) { cin >> a[i].first; a[i].second=i; }
    cin >> m; b.resize(m);
    for (int j=0;j<m;++j) { cin >> b[j].first; b[j].second=j+1; }
    sort(a.begin(),a.end()); sort(b.begin(),b.end());
    const long long inf=4000000000000000LL;
    vector<long long> prev(m+1,inf), cur(m+1,inf);
    vector<unsigned char> take((n+1)*(m+1),0);
    prev[0]=0;
    for (int i=1;i<=n;++i) {
        fill(cur.begin(),cur.end(),inf);
        for (int j=1;j<=m && j<=i;++j) {
            long long d=a[i-1].first-b[j-1].first;
            if (d<0) d=-d;
            if (prev[j-1]<=prev[j]) {
                cur[j]=prev[j-1]+d;
                take[i*(m+1)+j]=1;
            } else cur[j]=prev[j]+d;
        }
        prev.swap(cur);
    }
    cout << prev[m] << '\n';
    vector<int> ans(n);
    int j=m;
    for (int i=n;i>0;--i) {
        ans[a[i-1].second]=b[j-1].second;
        j-=take[i*(m+1)+j];
    }
    for (int i=0;i<n;++i) cout << ans[i] << (i+1==n?'\n':' ');
}
