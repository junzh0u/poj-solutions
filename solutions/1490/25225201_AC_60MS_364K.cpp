// POJ 1490 - Cabinets
// Model: gpt-6.1-sol
// Split comma-delimited records and retain the first style name of each group.
// Rejected records still establish the group name; later names never replace it.
// Blank lines start new catalogs, each with a header and no output separator.
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;
int main() {
    string line, code, firstName;
    bool fresh = true;
    while (getline(cin, line)) {
        if (!line.empty() && line[line.size()-1] == '\r') line.erase(line.size()-1);
        if (line.empty()) { fresh = true; code = ""; continue; }
        if (fresh) { cout << "Item Id,Item Desc,Item Price\n"; fresh = false; }
        string f[5];
        string::size_type start = 0;
        for (int i=0; i<5; ++i) {
            string::size_type end = line.find(',', start);
            if (end == string::npos) { f[i] = line.substr(start); break; }
            f[i] = line.substr(start, end-start); start = end+1;
        }
        if (f[0] != code) { code = f[0]; firstName = f[1]; }
        string name = f[1].empty() ? firstName : f[1];
        string id = string(3-f[0].size(), '0') + f[0] + f[2];
        if (id.size()>13 || name.empty()) continue;
        string desc = name;
        if (!f[3].empty()) desc += "-" + f[3];
        int cents = atoi(f[4].c_str());
        cout << id << ',' << desc.substr(0,30) << ',' << cents/100 << '.'
             << setfill('0') << setw(2) << cents%100 << '\n';
    }
    return 0;
}
