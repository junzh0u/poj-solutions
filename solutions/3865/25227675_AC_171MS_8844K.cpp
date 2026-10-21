// POJ 3865 - Database
// Model: gpt-6-astra
// Intern exact field strings, then sort row pairs for every two columns.
// Equal adjacent pairs provide the two rows and columns violating PNF.
// Read complete comma-separated lines; spaces inside values are significant.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
struct Entry {
    int a, b, row;
    bool operator<(const Entry& x) const {
        if (a != x.a) return a < x.a;
        if (b != x.b) return b < x.b;
        return row < x.row;
    }
};
int main() {
    ios::sync_with_stdio(false);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    string line;
    getline(cin, line);
    vector< vector<int> > table(n, vector<int>(m));
    map<string,int> ids;
    int next = 0;
    for (int r = 0; r < n; ++r) {
        getline(cin, line);
        if (!line.empty() && line[line.size()-1] == '\r') line.erase(line.size()-1);
        string::size_type start = 0;
        for (int c = 0; c < m; ++c) {
            string::size_type end = line.find(',', start);
            string field = line.substr(start, end == string::npos ? end : end-start);
            pair<map<string,int>::iterator,bool> p = ids.insert(make_pair(field,next));
            if (p.second) ++next;
            table[r][c] = p.first->second;
            start = end + 1;
        }
    }
    vector<Entry> entries(n);
    for (int a = 0; a < m; ++a) for (int b = a+1; b < m; ++b) {
        for (int r = 0; r < n; ++r) {
            entries[r].a = table[r][a]; entries[r].b = table[r][b]; entries[r].row = r;
        }
        sort(entries.begin(), entries.end());
        for (int r = 1; r < n; ++r) {
            if (entries[r-1].a == entries[r].a && entries[r-1].b == entries[r].b) {
                cout << "NO\n" << entries[r-1].row+1 << ' ' << entries[r].row+1
                     << '\n' << a+1 << ' ' << b+1 << '\n';
                return 0;
            }
        }
    }
    cout << "YES\n";
    return 0;
}
