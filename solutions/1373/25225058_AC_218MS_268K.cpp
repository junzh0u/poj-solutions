// POJ 1373 - L-system
// Model: gpt-6.1-sol
// Summarize each parallel expansion by its first/last |z|-1 symbols
// and whether it contains z. Concatenation preserves this information
// exactly. Repeated pairs of summaries imply all future answers repeat.
// The starting string (zero derivations) belongs to the language.
#include <iostream>
#include <string>
#include <set>
using namespace std;
struct Summary { string pre, suf; bool hit; Summary():hit(false){} };
string target;
int cap;
Summary join(const Summary &a, const Summary &b) {
    Summary c;
    c.hit = a.hit || b.hit || (a.suf+b.pre).find(target)!=string::npos;
    c.pre=(a.pre+b.pre).substr(0,cap);
    c.suf=a.suf+b.suf;
    if ((int)c.suf.size()>cap) c.suf.erase(0,c.suf.size()-cap);
    return c;
}
Summary expand(const string &s, const Summary q[2]) {
    Summary r;
    for (int i=0;i<(int)s.size();++i) r=join(r,q[s[i]-'a']);
    return r;
}
string key(const Summary q[2]) {
    string s;
    for(int i=0;i<2;++i) {
        s+=q[i].pre; s+='|'; s+=q[i].suf; s+='|';
        s+=q[i].hit?'1':'0';
    }
    return s;
}
int main() {
    ios::sync_with_stdio(false);
    string p[2],w;
    while(cin>>p[0]>>p[1]>>w>>target) {
        cap=(int)target.size()-1;
        Summary q[2];
        for(int i=0;i<2;++i) {
            string c(1,char('a'+i));
            q[i].pre=q[i].suf=c.substr(0,cap);
            q[i].hit=(c==target);
        }
        set<string> seen;
        bool yes=false;
        while(seen.insert(key(q)).second) {
            if(expand(w,q).hit) { yes=true; break; }
            Summary next[2];
            for(int i=0;i<2;++i) next[i]=expand(p[i],q);
            q[0]=next[0]; q[1]=next[1];
        }
        cout<<(yes?"YES":"NO")<<'\n';
    }
}
