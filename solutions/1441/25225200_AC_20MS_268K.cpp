// POJ 1441 - Table
// Model: gpt-6.1-sol
// Trim only exterior cell spaces and use the longest text in each column.
// Rebuild horizontal runs with the original junction bytes (DOS code page).
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<string> lines;
    vector< vector<string> > cells;
    vector<int> width;
    string s;
    while (getline(cin,s)) {
        if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        if (s.empty()) continue;
        lines.push_back(s);
        vector<string> row;
        if ((unsigned char)s[0]==179) {
            size_t begin=1;
            for (size_t i=1;i<s.size();++i) if ((unsigned char)s[i]==179) {
                string t=s.substr(begin,i-begin);
                size_t a=t.find_first_not_of(' '), b=t.find_last_not_of(' ');
                if (a==string::npos) t="";
                else t=t.substr(a,b-a+1);
                if (width.size()<=row.size()) width.push_back(2);
                width[row.size()]=max(width[row.size()],(int)t.size()+2);
                row.push_back(t);
                begin=i+1;
            }
        }
        cells.push_back(row);
    }
    for (size_t r=0;r<lines.size();++r) {
        if (!cells[r].empty()) {
            cout << (char)179;
            for (size_t c=0;c<cells[r].size();++c)
                cout << ' ' << cells[r][c] << string(width[c]-cells[r][c].size()-1,' ') << (char)179;
        } else {
            size_t col=0;
            cout << lines[r][0];
            for (size_t i=1;i<lines[r].size();++i) if ((unsigned char)lines[r][i]!=196) {
                cout << string(width[col++],(char)196) << lines[r][i];
            }
        }
        cout << '\n';
    }
    return 0;
}
