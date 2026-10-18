// POJ 3108 - Horn Clauses
// Model: gpt-6-astra
// Forward chaining constructs the least satisfying assignment. Each occurrence
// of an antecedent contributes one counter and one reverse incidence; repeated
// variables therefore work without deduplication. An empty consequent is false.
// Read one formula per line and retain full, arbitrarily long variable names.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

int intern(const string &s, map<string,int> &ids, vector<string> &names) {
    map<string,int>::iterator it = ids.find(s);
    if (it != ids.end()) return it->second;
    int id = (int)names.size();
    ids[s] = id;
    names.push_back(s);
    return id;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string line;
    while (getline(cin, line)) {
        map<string,int> ids;
        vector<string> names;
        vector<vector<int> > antecedents;
        vector<int> consequent;
        size_t pos = 0;
        while (pos < line.size()) {
            if (line[pos] != '(') { ++pos; continue; }
            ++pos;
            vector<int> left;
            int right = -1;
            bool after = false;
            while (pos < line.size() && line[pos] != ')') {
                char c = line[pos];
                if (c == '=') { after = true; ++pos; }
                else if (c >= 'A' && c <= 'Z') {
                    size_t start = pos;
                    while (pos < line.size() && line[pos] >= 'A' && line[pos] <= 'Z') ++pos;
                    int id = intern(line.substr(start, pos-start), ids, names);
                    if (after) right = id;
                    else left.push_back(id);
                } else ++pos;
            }
            if (pos < line.size()) ++pos;
            antecedents.push_back(left);
            consequent.push_back(right);
        }
        if (antecedents.empty()) continue;
        int n = (int)names.size(), m = (int)consequent.size();
        vector<vector<int> > uses(n);
        vector<int> remaining(m), value(n, 0);
        queue<int> ready;
        for (int i = 0; i < m; ++i) {
            remaining[i] = (int)antecedents[i].size();
            if (!remaining[i]) ready.push(i);
            for (size_t j = 0; j < antecedents[i].size(); ++j)
                uses[antecedents[i][j]].push_back(i);
        }
        bool possible = true;
        while (!ready.empty()) {
            int c = ready.front(); ready.pop();
            int v = consequent[c];
            if (v < 0) { possible = false; break; }
            if (value[v]) continue;
            value[v] = 1;
            for (size_t j = 0; j < uses[v].size(); ++j)
                if (--remaining[uses[v][j]] == 0) ready.push(uses[v][j]);
        }
        if (!possible) cout << "unsatisfiable\n";
        else {
            for (int i = 0; i < n; ++i) {
                if (i) cout << ',';
                cout << names[i] << '=' << (value[i] ? "true" : "false");
            }
            cout << '\n';
        }
    }
    return 0;
}
