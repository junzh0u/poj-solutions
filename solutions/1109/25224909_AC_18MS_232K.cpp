// POJ 1109 - Index Generation
// Model: gpt-6.1-sol
// Scan markers across lines, trim their fields, and collect distinct pages.
// Entry identity preserves spelling; only the output ordering ignores case.
// Line breaks inside markers become spaces; internal spaces are preserved.
#include <iostream>
#include <string>
#include <map>
#include <set>
using namespace std;
string lower(string s) {
    for (size_t i=0;i<s.size();++i)
        if (s[i]>='A' && s[i]<='Z') s[i]=char(s[i]-'A'+'a');
    return s;
}
struct Order {
    bool operator()(const string& a,const string& b) const {
        string x=lower(a), y=lower(b);
        return x==y ? a<b : x<y;
    }
};
string trim(const string& s) {
    size_t a=s.find_first_not_of(' '), b=s.find_last_not_of(' ');
    return a==string::npos ? "" : s.substr(a,b-a+1);
}
typedef map<string,set<int>,Order> Secondary;
struct Entry { set<int> pages; Secondary secondary; };
typedef map<string,Entry,Order> Index;
void add(Index& index,const string& s,int page) {
    size_t p=s.find('%'), d=s.find('$');
    size_t begin=p==string::npos ? 0 : p+1;
    size_t end=d==string::npos ? s.size() : d;
    string primary=trim(s.substr(begin,end-begin));
    if (primary.empty()) return;
    if (d==string::npos) index[primary].pages.insert(page);
    else index[primary].secondary[trim(s.substr(d+1))].insert(page);
}
void pages(const set<int>& s) {
    for (set<int>::const_iterator i=s.begin();i!=s.end();++i)
        cout << ", " << *i;
    cout << '\n';
}
int main() {
    string line, marker; Index index;
    int doc=1,page=1; bool inside=false;
    while (getline(cin,line)) {
        if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if (line=="**") break;
        if (line=="*") {
            cout << "DOCUMENT " << doc++ << '\n';
            for (Index::const_iterator i=index.begin();i!=index.end();++i) {
                cout << i->first; pages(i->second.pages);
                for (Secondary::const_iterator j=i->second.secondary.begin();
                     j!=i->second.secondary.end();++j) {
                    cout << "+ " << j->first; pages(j->second);
                }
            }
            index.clear(); page=1; inside=false; marker.clear();
            continue;
        }
        for (size_t i=0;i<line.size();++i) {
            char c=line[i];
            if (c=='{') { inside=true; marker.clear(); }
            else if (c=='}') { add(index,marker,page); inside=false; }
            else if (inside) marker+=c;
            else if (c=='&') ++page;
        }
        if (inside) marker+=' ';
    }
    return 0;
}
