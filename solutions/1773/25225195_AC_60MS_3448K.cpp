// POJ 1773 - Outernet
// Model: gpt-6.1-sol
// Keep an independent destination for each incoming port and emit protocol
// events in order, coalescing consecutive output on the same port.
// Forward DATA unchanged, recognizing unescaped dots as terminators.
// A successful TO silently closes the previous outgoing session. Emit #-1
// as specified, although the published sample omits that final line.
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
using namespace std;
int lastPort;
void output(int p, const string &s) {
    if (lastPort != p) { cout << '#' << p << '\n'; lastPort = p; }
    cout << s;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int m;
    string line, name;
    while (cin >> m && m) {
        map<string,int> route;
        int p;
        for (int i=0;i<m;++i) { cin >> name >> p; route[name]=p; }
        getline(cin,line);
        int dest[32768];
        for (int i=0;i<32768;++i) dest[i]=-1;
        int incoming=0;
        lastPort=-2;
        while (getline(cin,line)) {
            if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if (!line.empty() && line[0]=='#') {
                incoming=atoi(line.c_str()+1);
                if (incoming<0) { cout << "#-1\n"; break; }
            } else if (line.substr(0,3)=="TO:") {
                map<string,int>::iterator it=route.find(line.substr(3));
                if (it==route.end()) output(incoming,"301\n");
                else if (it->second==incoming) output(incoming,"303\n");
                else {
                    if (dest[incoming]>0) output(dest[incoming],"QUIT\n");
                    dest[incoming]=it->second;
                    if (it->second>0) output(it->second,line+"\n");
                    output(incoming,it->second==0 ? "101\n" : "100\n");
                }
            } else if (line=="QUIT") {
                if (dest[incoming]<0) output(incoming,"302\n");
                else {
                    if (dest[incoming]>0) output(dest[incoming],"QUIT\n");
                    dest[incoming]=-1;
                    output(incoming,"200\n");
                }
            } else if (line=="DATA") {
                int d=dest[incoming];
                if (d>0) output(d,"DATA\n");
                bool escaped=false;
                char c;
                while (cin.get(c)) {
                    if (d>0) cout.put(c);
                    if (c=='.' && !escaped) {
                        if (cin.get(c)) { if (d>0) cout.put(c); }
                        break;
                    }
                    if (c=='\\') escaped=!escaped; else escaped=false;
                }
                output(incoming,d<0 ? "302\n" : d==0 ? "101\n" : "100\n");
            } else output(incoming,"300\n");
        }
    }
    return 0;
}
