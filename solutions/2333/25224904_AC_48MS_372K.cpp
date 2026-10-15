// POJ 2333 - Beach cut
// Model: gpt-6.1-sol
// Scan all endpoint pairs, maintaining the minimum slope from the left end.
// A chord lies on the beach precisely when its slope is no greater than all
// intervening slopes. Equality is allowed because touching the shore is legal.
// Prefix trapezoid sums give twice the area; all geometry is exact in 64 bits.
#include <iostream>
using namespace std;
typedef long long Int;
Int x[5005], y[5005], pref[5005];
int main() {
    int n; Int limit;
    if (!(cin >> n >> limit)) return 0;
    for (int i=0;i<n;++i) {
        cin >> x[i] >> y[i];
        if (i) pref[i]=pref[i-1]+(x[i]-x[i-1])*(y[i]+y[i-1]);
    }
    Int best=0;
    for (int i=0;i<n-1;++i) {
        Int lowdy=y[i+1]-y[i], lowdx=x[i+1]-x[i];
        for (int j=i+1;j<n;++j) {
            Int dx=x[j]-x[i], dy=y[j]-y[i];
            if (dx>limit) break;
            if (dy*lowdx<=lowdy*dx) {
                lowdy=dy; lowdx=dx;
                if (dx*dx+dy*dy<=limit*limit) {
                    Int area=pref[j]-pref[i]-dx*(y[i]+y[j]);
                    if (area>best) best=area;
                }
            }
        }
    }
    cout << best/2;
    if (best%2) cout << ".5";
    cout << '\n';
    return 0;
}
