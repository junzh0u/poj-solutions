// POJ 3583 - Text Message Formatting
// Model: gpt-6.1-sol
// Scan the original format once, copying escaped characters literally.
// Only an unescaped brace-digit-brace sequence with a valid index is replaced.
// Leading zeroes are allowed; arguments are copied without further formatting.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string s, line;
    if (!getline(cin, s)) return 0;
    if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
    vector<string> a;
    while (getline(cin,line)) {
        if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        a.push_back(line);
    }
    for (size_t i=0; i<s.size();) {
        if (s[i]=='\\' && i+1<s.size()) {
            cout << s[i+1]; i+=2;
        } else if (s[i]=='{') {
            size_t j=i+1;
            int v=0;
            while (j<s.size() && s[j]>='0' && s[j]<='9') {
                v=v*10+s[j]-'0';
                if (v>1000) v=1001;
                ++j;
            }
            if (j>i+1 && j<s.size() && s[j]=='}' && v>=1 && v<=(int)a.size()) {
                cout << a[v-1]; i=j+1;
            } else { cout << s[i]; ++i; }
        } else { cout << s[i]; ++i; }
    }
    cout << '\n';
    return 0;
}
