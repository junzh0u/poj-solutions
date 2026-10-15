// POJ 3856 - deltree
// Model: gpt-6.1-sol
// Store each observed file once under its absolute path. Only observed
// file sizes are guaranteed; unvisited directories may be empty.
// A trailing separator makes subtree prefix tests respect directory names.
// Each deltree ends a scenario, even when no blank line follows it.
#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;
string resolve(const string &cwd, const string &arg) {
    if (arg == "..") {
        string p = cwd.substr(0, cwd.size()-1);
        return p.substr(0, p.rfind('\\')+1);
    }
    string p = (!arg.empty() && arg[0]=='\\') ? arg : cwd+arg;
    if (p.empty() || p[p.size()-1]!='\\') p += '\\';
    return p;
}
int main() {
    ios::sync_with_stdio(false);
    string line, cwd="\\";
    map<string, long long> files;
    while (getline(cin,line)) {
        if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if (line.empty()) continue;
        if (line[0]=='>') {
            if (line==">exit") break;
            if (line.substr(0,4)==">cd ") cwd=resolve(cwd,line.substr(4));
            else if (line.substr(0,9)==">deltree ") {
                string target=resolve(cwd,line.substr(9));
                long long answer=0;
                for (map<string,long long>::const_iterator i=files.begin(); i!=files.end(); ++i)
                    if (i->first.compare(0,target.size(),target)==0) answer+=i->second;
                cout << answer << '\n';
                files.clear(); cwd="\\";
            }
        } else {
            istringstream in(line);
            string name; long long size;
            if (in >> name >> size) files[cwd+name]=size;
        }
    }
    return 0;
}
