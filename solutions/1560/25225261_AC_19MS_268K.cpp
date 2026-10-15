// POJ 1560 - 18 Wheeler Caravans (aka Semigroups)
// Model: gpt-6.1-sol
// Check closure, associativity, then commutativity by exhaustive enumeration.
// Counterexamples use alphabetic element order; the displayed table keeps input order.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int n;
    while (cin >> n && n) {
        string s(n, ' '), a[26];
        int pos[26];
        fill(pos, pos+26, -1);
        for (int i=0;i<n;++i) { cin >> s[i]; pos[s[i]-'a']=i; }
        for (int i=0;i<n;++i) {
            a[i]=string(n,' ');
            for (int j=0;j<n;++j) cin >> a[i][j];
        }
        cout << "S = {";
        for (int i=0;i<n;++i) { if(i) cout << ','; cout << s[i]; }
        cout << "}\n #|" << s << "\n -+" << string(n,'-') << '\n';
        for (int i=0;i<n;++i) cout << ' ' << s[i] << '|' << a[i] << '\n';
        cout << '\n';
        string order=s; sort(order.begin(),order.end());
        bool done=false;
        for (int i=0;i<n && !done;++i) for (int j=0;j<n && !done;++j) {
            char x=order[i], y=order[j], z=a[pos[x-'a']][pos[y-'a']];
            if (pos[z-'a']==-1) {
                cout << "NOT A SEMIGROUP: " << x << '#' << y << " = " << z << "  WHICH IS NOT AN ELEMENT OF THE SET\n";
                done=true;
            }
        }
        for (int i=0;i<n && !done;++i) for (int j=0;j<n && !done;++j) for (int k=0;k<n && !done;++k) {
            char x=order[i], y=order[j], z=order[k];
            int u=pos[x-'a'], v=pos[y-'a'], w=pos[z-'a'];
            if (a[pos[a[u][v]-'a']][w] != a[u][pos[a[v][w]-'a']]) {
                cout << "NOT A SEMIGROUP: (" << x << '#' << y << ")#" << z << " IS NOT EQUAL TO " << x << "#(" << y << '#' << z << ")\n";
                done=true;
            }
        }
        for (int i=0;i<n && !done;++i) for (int j=0;j<n && !done;++j) {
            char x=order[i], y=order[j]; int u=pos[x-'a'],v=pos[y-'a'];
            if(a[u][v]!=a[v][u]) {
                cout << "SEMIGROUP BUT NOT COMMUTATIVE  (" << x << '#' << y << " IS NOT EQUAL TO " << y << '#' << x << ")\n";
                done=true;
            }
        }
        if(!done) cout << "COMMUTATIVE SEMIGROUP\n";
        cout << "------------------------------\n\n";
    }
}
