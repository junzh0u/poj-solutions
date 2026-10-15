// POJ 3149 - Billing Tables
// Model: gpt-6.1-sol
// Apply rules in reverse order to a lazy decimal trie, so earlier rules win.
// Collapse equal children and emit maximal valid uniform subtrees in order.
// The root cannot be emitted: even a universal plan needs ten one-digit entries.
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;
typedef long long Int;
struct Node {
    int plan, child[10];
    Node(int p=0):plan(p) { for(int i=0;i<10;++i) child[i]=-1; }
};
vector<Node> tree;
vector<string> names;
struct Rule { Int lo,hi; int plan; };
void assignRange(int v,Int lo,Int size,Int a,Int b,int p) {
    if(b<lo || lo+size-1<a) return;
    if(a<=lo && lo+size-1<=b) { tree[v].plan=p; return; }
    if(tree[v].plan>=0) {
        int old=tree[v].plan;
        for(int i=0;i<10;++i) {
            int u=(int)tree.size(); tree.push_back(Node(old)); tree[v].child[i]=u;
        }
        tree[v].plan=-1;
    }
    Int step=size/10;
    for(int i=0;i<10;++i) assignRange(tree[v].child[i],lo+i*step,step,a,b,p);
    int same=tree[tree[v].child[0]].plan;
    for(int i=1;i<10;++i) if(tree[tree[v].child[i]].plan!=same) same=-1;
    tree[v].plan=same;
}
vector<pair<string,int> > answer;
void emit(int v,string prefix) {
    if(tree[v].plan>=0) {
        if(tree[v].plan==0) return;
        if(prefix.empty()) {
            for(int i=0;i<10;++i) answer.push_back(make_pair(string(1,char('0'+i)),tree[v].plan));
        } else answer.push_back(make_pair(prefix,tree[v].plan));
        return;
    }
    for(int i=0;i<10;++i) emit(tree[v].child[i],prefix+char('0'+i));
}
Int number(const string &s) { Int x=0; for(unsigned i=0;i<s.size();++i) x=x*10+s[i]-'0'; return x; }
int main() {
    int n; if(!(cin>>n)) return 0;
    names.push_back("invalid"); vector<Rule> rules;
    for(int i=0;i<n;++i) {
        string a,dash,b,name; cin>>a>>dash>>b>>name;
        b=a.substr(0,a.size()-b.size())+b;
        Int scale=1; for(unsigned j=a.size();j<11;++j) scale*=10;
        int p=0; while(p<(int)names.size() && names[p]!=name) ++p;
        if(p==(int)names.size()) names.push_back(name);
        Rule r; r.lo=number(a)*scale; r.hi=(number(b)+1)*scale-1; r.plan=p; rules.push_back(r);
    }
    tree.push_back(Node());
    Int total=1; for(int i=0;i<11;++i) total*=10;
    for(int i=n-1;i>=0;--i) assignRange(0,0,total,rules[i].lo,rules[i].hi,rules[i].plan);
    emit(0,""); cout<<answer.size()<<'\n';
    for(unsigned i=0;i<answer.size();++i) cout<<answer[i].first<<' '<<names[answer[i].second]<<'\n';
}
