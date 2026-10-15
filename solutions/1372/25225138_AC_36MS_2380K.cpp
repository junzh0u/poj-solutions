// POJ 1372 - FORCAL
// Model: gpt-6.1-sol
// Scan maximal letter/digit/underscore words before checking their length.
// Comments end at the line boundary; an error discards the rest of its block.
// Only lexical recognition is required, so reserved words retain their spelling.
#include <iostream>
#include <string>
using namespace std;
bool digit(char c) { return c >= '0' && c <= '9'; }
bool word(char c) {
    return digit(c) || (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') || c == '_';
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string s;
    bool bad = false, active = false;
    while (getline(cin, s)) {
        if (!s.empty() && s[s.size()-1] == '\r') s.erase(s.size()-1);
        if (s.empty()) {
            cout << '\n'; bad = false; active = false; continue;
        }
        active = true;
        if (bad) continue;
        size_t i = 0;
        while (i < s.size()) {
            char c = s[i];
            if (c == ' ' || c == '\t') { ++i; continue; }
            if (c == '-' && i+1 < s.size() && s[i+1] == '-') break;
            if (word(c)) {
                size_t j = i; bool number = true;
                while (j < s.size() && word(s[j])) {
                    if (!digit(s[j])) number = false;
                    ++j;
                }
                if (j-i > (number ? 8u : 32u)) { bad = true; break; }
                cout << s.substr(i, j-i) << '\n'; i = j;
            } else if (c == ':' && i+1 < s.size() && s[i+1] == '=') {
                cout << ":=\n"; i += 2;
            } else if (c == '+' || c == '-' || c == '(' || c == ')' || c == ';' || c == ',') {
                cout << c << '\n'; ++i;
            } else { bad = true; break; }
        }
        if (bad) cout << "TOKEN ERROR\n";
    }
    if (active) cout << '\n';
    return 0;
}
