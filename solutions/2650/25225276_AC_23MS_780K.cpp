// POJ 2650 - Spaghetti
// Model: gpt-6.1-sol
// Remove unconditional jumps, then compare all reachable pairs of statements.
// Conditional targets are labels, not statement text; both outcomes must match.
// Only a complete goto followed by an integer is a jump, not a prefix.
// A cycle consisting only of unconditional jumps has no further statements.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
using namespace std;
struct Node { string text; int type, dest; Node():type(0),dest(-1){} };
struct Program {
    vector<Node> a;
    vector<int> norm;
    void build(const vector<string>& lines) {
        map<int,int> labels;
        for (int i=0;i<(int)lines.size();++i) {
            string l=lines[i], s;
            int label=atoi(l.substr(0,5).c_str());
            if(l.substr(0,5).find_first_not_of(" ")!=string::npos) labels[label]=i;
            for(int j=6;j<(int)l.size();++j) if(l[j]!=' ' && l[j]!='\t' && l[j]!='\r') s+=l[j];
            Node v; v.text=s;
            if(s.substr(0,4)=="goto" && s.size()>4 && s.find_first_not_of("0123456789",4)==string::npos) { v.type=1; v.dest=atoi(s.c_str()+4); v.text=""; }
            else if(s.substr(0,3)=="if(") {
                string::size_type p=s.rfind(")goto");
                if(p!=string::npos && p+5<s.size() && s.find_first_not_of("0123456789",p+5)==string::npos) { v.type=2; v.dest=atoi(s.c_str()+p+5); v.text=s.substr(0,p+1); }
            }
            a.push_back(v);
        }
        int n=a.size();
        for(int i=0;i<n;++i) if(a[i].type) a[i].dest=labels[a[i].dest];
        norm.resize(n+1,n);
        for(int i=0;i<n;++i) {
            int j=i, count=0;
            while(j<n && a[j].type==1 && count<=n) { j=a[j].dest; ++count; }
            norm[i]=(count>n?n:j);
        }
        Node end; end.text="#silent-end"; end.type=3; a.push_back(end);
    }
};
bool equivalent(Program& p,Program& q) {
    int n=p.a.size(),m=q.a.size();
    vector<char> seen(n*m,0);
    vector<pair<int,int> > todo;
    todo.push_back(make_pair(0,0));
    while(!todo.empty()) {
        int x=p.norm[todo.back().first],y=q.norm[todo.back().second]; todo.pop_back();
        if(seen[x*m+y]) continue;
        seen[x*m+y]=1;
        Node &a=p.a[x],&b=q.a[y];
        if(a.type!=b.type || a.text!=b.text) return false;
        if(a.type==3 || a.text=="stop") continue;
        todo.push_back(make_pair(x+1,y+1));
        if(a.type==2) todo.push_back(make_pair(a.dest,b.dest));
    }
    return true;
}
int main() {
    vector<string> lines[2]; string s; int part=0;
    while(getline(cin,s)) {
        if(!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        if(s.find_first_not_of(" \t")==string::npos) { if(!lines[0].empty()) part=1; continue; }
        lines[part].push_back(s);
    }
    Program p,q; p.build(lines[0]); q.build(lines[1]);
    cout << "The programs are " << (equivalent(p,q)?"":"not ") << "equivalent.\n";
    return 0;
}
