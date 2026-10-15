// POJ 1335 - Digital Onion
// Model: gpt-6.1-sol
// Advance the outside, then the inside, then the inside's weight.
// Reset less significant parts to the minimum of their required weight.
// The board reports weight-zero inputs; accept the null onion as well.
#include <iostream>
#include <string>
using namespace std;
string minimum(int n) {
    string r;
    for (int i=0; i<n; ++i) r += "()";
    return r;
}
bool advance(string &s) {
    if (s.empty()) return false;
    int balance=0, cut=0;
    do { balance += s[cut]=='(' ? 1 : -1; ++cut; } while(balance);
    string a=s.substr(1,cut-2), b=s.substr(cut), t=b;
    if (advance(t)) { s="("+a+")"+t; return true; }
    t=a;
    if (advance(t)) { s="("+t+")"+minimum(b.size()/2); return true; }
    if (!b.empty()) {
        s="("+minimum(a.size()/2+1)+")"+minimum(b.size()/2-1);
        return true;
    }
    return false;
}
int main() {
    int tests; cin >> tests;
    while(tests--) {
        string s; char c;
        while(cin >> c && c!='$') if(c=='(' || c==')') s+=c;
        int n=s.size()/2;
        if(!advance(s)) s=minimum(n+1);
        for(size_t i=0;i<s.size();++i) cout << s[i] << ' ';
        cout << "$\n";
    }
    return 0;
}
