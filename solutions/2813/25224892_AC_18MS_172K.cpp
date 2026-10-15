// POJ 2813 - Pasting Strings
// Model: gpt-6.1-sol
// Scan whole tags to maintain the active formatting stack. Reopen the stack
// immediately before B, copy [B,E), then close the stack immediately before E.
// Boundaries at '<' precede that tag; empty selections retain their context.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void advance(const string &s, int &p, vector<string> &st) {
    if (s[p] != '<') { ++p; return; }
    int q = (int)s.find('>', p);
    if (s[p+1] == '/') st.pop_back();
    else st.push_back(s.substr(p+1, q-p-1));
    p = q+1;
}
int main() {
    int b,e;
    while (cin >> b >> e) {
        if (b == -1 && e == -1) break;
        cin.get();
        string s; getline(cin,s);
        if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        vector<string> st;
        int p=0;
        while (p<b) advance(s,p,st);
        for (int i=0;i<(int)st.size();++i) cout << '<' << st[i] << '>';
        cout << s.substr(b,e-b);
        while (p<e) advance(s,p,st);
        for (int i=(int)st.size()-1;i>=0;--i) cout << "</" << st[i] << '>';
        cout << '\n';
    }
    return 0;
}
