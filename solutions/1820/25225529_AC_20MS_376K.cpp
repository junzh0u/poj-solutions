// POJ 1820 - Expression
// Model: gpt-6.1-sol
// Resolve aliases, then compare recursive expression graphs coinductively.
// Undefined names and alias-only cycles form an unresolved expression type;
// concrete types, list lengths and numeric values must agree.
// Names are case insensitive, including names first encountered in a query.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <cctype>
using namespace std;
struct Node {
    int type, value, target;
    vector<int> child;
    Node(int t=0,int v=0):type(t),value(v),target(-1){}
};
vector<Node> nodes;
map<string,int> names;
int nameNode(string s) {
    for(size_t i=0;i<s.size();++i) s[i]=char(tolower((unsigned char)s[i]));
    if(names.count(s)) return names[s];
    int id=nodes.size(); nodes.push_back(Node()); names[s]=id; return id;
}
void space(const string &s,size_t &p) {
    while(p<s.size() && isspace((unsigned char)s[p])) ++p;
}
int parse(const string &s,size_t &p) {
    space(s,p);
    if(s[p]=='(') {
        ++p; int id=nodes.size(); nodes.push_back(Node(2));
        space(s,p);
        while(p<s.size() && s[p]!=')') {
            int c=parse(s,p); nodes[id].child.push_back(c); space(s,p);
        }
        ++p; return id;
    }
    if(isdigit((unsigned char)s[p])) {
        int v=0;
        while(p<s.size() && isdigit((unsigned char)s[p])) v=v*10+s[p++]-'0';
        int id=nodes.size(); nodes.push_back(Node(1,v)); return id;
    }
    string sname;
    while(p<s.size() && isalpha((unsigned char)s[p])) sname+=s[p++];
    return nameNode(sname);
}
set<pair<int,int> > seen;
int resolve(int a) {
    set<int> path;
    while(nodes[a].type==0 && nodes[a].target!=-1) {
        if(!path.insert(a).second) return -1;
        a=nodes[a].target;
    }
    return nodes[a].type==0 ? -1 : a;
}
bool equalExpr(int a,int b) {
    a=resolve(a); b=resolve(b);
    if(a==-1 || b==-1) return a==b;
    if(a==b) return true;
    if(a>b) swap(a,b);
    if(!seen.insert(make_pair(a,b)).second) return true;
    if(nodes[a].type!=nodes[b].type) return false;
    if(nodes[a].type==1) return nodes[a].value==nodes[b].value;
    if(nodes[a].child.size()!=nodes[b].child.size()) return false;
    for(size_t i=0;i<nodes[a].child.size();++i)
        if(!equalExpr(nodes[a].child[i],nodes[b].child[i])) return false;
    return true;
}
int main() {
    string line;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line.find_first_not_of(" \t\r")==string::npos) continue;
        if(line.substr(line.find_first_not_of(" \t"),3)=="EOD") break;
        size_t q=line.find('='); if(q==string::npos) continue;
        size_t p=0; int a=parse(line,p); p=q+1; int b=parse(line,p);
        nodes[a].target=b;
    }
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line.find_first_not_of(" \t\r")==string::npos) continue;
        if(line.substr(line.find_first_not_of(" \t"),3)=="EOE") break;
        size_t q=line.find('?'); if(q==string::npos) continue;
        size_t p=0; int a=parse(line,p); p=q+1; int b=parse(line,p);
        seen.clear(); cout<<(equalExpr(a,b)?1:0)<<'\n';
    }
    return 0;
}
