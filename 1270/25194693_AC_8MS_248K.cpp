// POJ 1270 - Following Orders
// Model: gpt-5.6-terra
// Approach: enumerate every topological ordering with DFS. At each position,
// choose an unused zero-indegree variable in sorted order, which directly
// produces the required lexicographical output. Duplicate constraints are
// ignored so they cannot inflate indegrees.
// Input note: specifications may be separated by blank lines, so read the two
// nonempty lines of each specification explicitly until EOF.
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<char> variables;
vector<int> indegree;
vector< vector<int> > edge;
vector<bool> used;
string ordering;

void enumerate() {
    int i;

    if (ordering.size() == variables.size()) {
        cout << ordering << '\n';
        return;
    }
    for (i = 0; i < (int)variables.size(); ++i) {
        if (!used[i] && indegree[i] == 0) {
            int j;
            used[i] = true;
            ordering += variables[i];
            for (j = 0; j < (int)edge[i].size(); ++j)
                --indegree[edge[i][j]];
            enumerate();
            for (j = 0; j < (int)edge[i].size(); ++j)
                ++indegree[edge[i][j]];
            ordering.erase(ordering.size() - 1);
            used[i] = false;
        }
    }
}

bool next_nonempty_line(string &line) {
    while (getline(cin, line)) {
        bool nonempty = false;
        int i;
        for (i = 0; i < (int)line.size(); ++i) {
            if (!isspace((unsigned char)line[i])) {
                nonempty = true;
                break;
            }
        }
        if (nonempty)
            return true;
    }
    return false;
}

int main() {
    string variable_line;
    string constraint_line;
    bool first_case = true;

    while (next_nonempty_line(variable_line)) {
        vector<string> tokens;
        string token;
        stringstream variable_stream(variable_line);
        int position[26];
        int i;

        if (!next_nonempty_line(constraint_line))
            break;
        variables.clear();
        while (variable_stream >> token)
            variables.push_back(token[0]);
        sort(variables.begin(), variables.end());
        for (i = 0; i < 26; ++i)
            position[i] = -1;
        for (i = 0; i < (int)variables.size(); ++i)
            position[variables[i] - 'a'] = i;

        indegree.assign(variables.size(), 0);
        edge.assign(variables.size(), vector<int>());
        vector< vector<bool> > connected(variables.size(), vector<bool>(variables.size(), false));
        stringstream constraint_stream(constraint_line);
        while (constraint_stream >> token)
            tokens.push_back(token);
        for (i = 0; i + 1 < (int)tokens.size(); i += 2) {
            int from = position[tokens[i][0] - 'a'];
            int to = position[tokens[i + 1][0] - 'a'];
            if (!connected[from][to]) {
                connected[from][to] = true;
                edge[from].push_back(to);
                ++indegree[to];
            }
        }

        if (!first_case)
            cout << '\n';
        first_case = false;
        used.assign(variables.size(), false);
        ordering.clear();
        enumerate();
    }
    return 0;
}
