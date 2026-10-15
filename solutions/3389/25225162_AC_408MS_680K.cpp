// POJ 3389 - Kennings
// Model: gpt-6.1-sol
// Simulate simultaneous substitutions, keeping each original plan line separate.
// Count letters and underscores only; wrap the final expanded lines greedily.
// Every substitution increases the word count, so an active process must reach
// the target. Underscores are part of a word, including in kenning referents.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
int main() {
    int w, l, n;
    if (!(cin >> w >> l >> n)) return 0;
    string s, key, word;
    getline(cin, s);
    map<string, vector<string> > rules;
    for (int i=0; i<n; ++i) {
        getline(cin,s);
        istringstream in(s);
        in >> key;
        while (in >> word) rules[key].push_back(word);
    }
    vector<vector<string> > lines;
    while (getline(cin,s)) {
        istringstream in(s);
        vector<string> line;
        while (in >> word) line.push_back(word);
        lines.push_back(line);
    }
    for (;;) {
        int count=0;
        for (size_t i=0; i<lines.size(); ++i)
            for (size_t j=0; j<lines[i].size(); ++j)
                count += lines[i][j].size();
        if (count >= l) break;
        bool changed=false;
        for (size_t i=0; i<lines.size(); ++i) {
            vector<string> next;
            for (size_t j=0; j<lines[i].size(); ++j) {
                map<string,vector<string> >::const_iterator it=rules.find(lines[i][j]);
                if (it==rules.end()) next.push_back(lines[i][j]);
                else {
                    changed=true;
                    next.insert(next.end(),it->second.begin(),it->second.end());
                }
            }
            lines[i].swap(next);
        }
        if (!changed) { cout << "No result\n"; return 0; }
    }
    for (size_t i=0; i<lines.size(); ++i) {
        int used=0;
        for (size_t j=0; j<lines[i].size(); ++j) {
            int len=lines[i][j].size();
            if (used && used+1+len>w) { cout << '\n'; used=0; }
            if (used) { cout << ' '; ++used; }
            cout << lines[i][j]; used+=len;
        }
        cout << '\n';
    }
    return 0;
}
