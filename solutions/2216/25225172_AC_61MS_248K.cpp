// POJ 2216 - Roulette
// Model: gpt-6.1-sol
// Keep all 47 multiplier residues consistent with the observed transitions.
// A next value is determined only if every surviving residue predicts it.
// An empty history is unknown; even an unknown multiplier predicts zero
// after a multiple of 47. Compare observed results exactly, not modulo 47.
#include <iostream>
#include <string>
using namespace std;
int main() {
    int cases; cin >> cases;
    while (cases--) {
        int m; cin >> m;
        bool possible[47];
        for (int a=0; a<47; ++a) possible[a]=true;
        int previous=0;
        for (int i=0; i<m; ++i) {
            string s; cin >> s;
            int residue=0, value=0;
            for (unsigned j=0; j<s.size(); ++j) {
                residue=(residue*10+s[j]-'0')%47;
                if (value<47) { value=value*10+s[j]-'0'; if(value>47)value=47; }
            }
            if (i) for (int a=0; a<47; ++a)
                if ((a*previous)%47 != value) possible[a]=false;
            previous=residue;
        }
        int answer=-1; bool ambiguous=(m==0), any=false;
        for (int a=0; a<47; ++a) if (possible[a]) {
            int next=a*previous%47;
            if(any && next!=answer) ambiguous=true;
            answer=next; any=true;
        }
        if (!any) cout << "Algoritmus byl zmenen.\n";
        else if (ambiguous) cout << "Dalsi cislo nelze urcit.\n";
        else cout << "Vsad na " << answer << ".\n";
    }
    return 0;
}
