// POJ 1443 - DEL command
// Model: gpt-6-astra
// Build the most restrictive common pattern for name and extension separately.
// Keep every unanimous character, use '?' for disagreements, and a trailing
// '*' only for unequal lengths. Any other feasible pattern is no stricter.
// Missing extensions and an explicit trailing dot both mean an empty extension.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct File { string name, ext; bool del; };
string pattern(const vector<string>& a) {
    size_t lo=a[0].size(), hi=lo;
    for(size_t i=1;i<a.size();++i) { lo=min(lo,a[i].size()); hi=max(hi,a[i].size()); }
    string p=a[0].substr(0,lo);
    for(size_t i=1;i<a.size();++i)
        for(size_t j=0;j<lo;++j) if(p[j]!=a[i][j]) p[j]='?';
    if(lo!=hi) p+='*';
    return p;
}
bool match(const string& p,const string& s) {
    size_t i=0;
    for(;i<p.size();++i) {
        if(p[i]=='*') return true;
        if(i>=s.size() || (p[i]!='?' && p[i]!=s[i])) return false;
    }
    return i==s.size();
}
int main() {
    vector<File> files; vector<string> names,exts; string s;
    while(cin>>s) {
        File f; f.del=s[0]=='-'; s.erase(0,1);
        size_t dot=s.find('.');
        f.name=s.substr(0,dot); f.ext=dot==string::npos?"":s.substr(dot+1);
        files.push_back(f);
        if(f.del) { names.push_back(f.name); exts.push_back(f.ext); }
    }
    if(names.empty()) return 0;
    string n=pattern(names), e=pattern(exts);
    for(size_t i=0;i<files.size();++i)
        if(!files[i].del && match(n,files[i].name) && match(e,files[i].ext)) {
            cout<<"IMPOSSIBLE\n"; return 0;
        }
    cout<<"DEL "<<n<<'.'<<e<<'\n';
    return 0;
}
