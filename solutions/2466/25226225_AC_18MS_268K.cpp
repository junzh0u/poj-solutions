// POJ 2466 - Pairsumonious Numbers
// Model: gpt-6-astra
// Sorted sums begin with a[0]+a[1] and a[0]+a[2]. Enumerate
// a[1]+a[2] to determine a[0], then remove every generated pair sum.
// Multiplicities and negative values are significant; any valid answer works.
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
typedef long long Int;
int main() {
    int n;
    while (cin >> n) {
        int m = n*(n-1)/2;
        vector<Int> s(m), a;
        for (int i=0; i<m; ++i) cin >> s[i];
        sort(s.begin(), s.end());
        bool found = false;
        for (int k=m-1; k>=2 && !found; --k) {
            Int twice = s[0]+s[1]-s[k];
            if (twice%2) continue;
            a.assign(1, twice/2);
            multiset<Int> left(s.begin(), s.end());
            bool ok = true;
            while ((int)a.size()<n && ok) {
                if (left.empty()) { ok=false; break; }
                Int next = *left.begin()-a[0];
                if (next<a.back()) { ok=false; break; }
                for (int j=0; j<(int)a.size(); ++j) {
                    multiset<Int>::iterator it = left.find(a[j]+next);
                    if (it==left.end()) { ok=false; break; }
                    left.erase(it);
                }
                if (ok) a.push_back(next);
            }
            found = ok && left.empty();
        }
        if (!found) cout << "Impossible\n";
        else {
            for (int i=0; i<n; ++i) cout << (i ? " " : "") << a[i];
            cout << '\n';
        }
    }
    return 0;
}
