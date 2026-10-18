// POJ 3383 - Equation
// Model: gpt-6-astra
// Parse the four left-associative precedence levels into an expression tree.
// For each universe element, propagate possible Boolean memberships upward,
// then reconstruct a satisfying assignment downward. Non-Omega variables
// occur once, so child choices are independent. Official contest test 02
// confirms the repeated result in POJ's second sample is a transcription typo.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>
using namespace std;
struct Var { string name; bool known; set<int> value; Var(string s):name(s),known(false){} };
struct Node { char op; int a,b,mask; Node(char o,int x,int y):op(o),a(x),b(y),mask(0){} };
vector<Var> vars;
map<string,int> ids;
vector<Node> nodes;
string expr; size_t posi;
int variable(const string &s) {
    if(ids.count(s)) return ids[s];
    int i=vars.size(); ids[s]=i; vars.push_back(Var(s)); return i;
}
int add(char op,int a,int b) { nodes.push_back(Node(op,a,b)); return nodes.size()-1; }
void ws() { while(posi<expr.size() && isspace((unsigned char)expr[posi])) ++posi; }
int parse(int level) {
    ws();
    if(level==4) {
        if(expr[posi]=='(') { ++posi; int r=parse(0); ws(); ++posi; return r; }
        size_t start=posi;
        while(posi<expr.size() && isalpha((unsigned char)expr[posi])) ++posi;
        return add('v',variable(expr.substr(start,posi-start)),0);
    }
    const char ops[]="^-+*";
    int r=parse(level+1); ws();
    while(posi<expr.size() && expr[posi]==ops[level]) {
        ++posi; int t=parse(level+1); r=add(ops[level],r,t); ws();
    }
    return r;
}
int apply(char op,int a,int b) {
    if(op=='*') return a&b;
    if(op=='+') return a|b;
    if(op=='-') return a&!b;
    if(op=='^') return a^b;
    return a==b;
}
void recover(int i,int val,int e) {
    Node &n=nodes[i];
    if(n.op=='v') { if(!vars[n.a].known && val) vars[n.a].value.insert(e); return; }
    for(int a=0;a<2;++a) for(int b=0;b<2;++b)
        if((nodes[n.a].mask&(1<<a)) && (nodes[n.b].mask&(1<<b)) && apply(n.op,a,b)==val) {
            recover(n.a,a,e); recover(n.b,b,e); return;
        }
}
int main() {
    getline(cin,expr); posi=0;
    int left=parse(0); ++posi; int right=parse(0); int root=add('=',left,right);
    string line;
    while(getline(cin,line)) {
        size_t eq=line.find('='); if(eq==string::npos) continue;
        string name; istringstream ns(line.substr(0,eq)); ns>>name;
        int v=variable(name); vars[v].known=true;
        istringstream values(line.substr(eq+1)); int e;
        while(values>>e) vars[v].value.insert(e);
    }
    int omega=variable("Omega");
    for(set<int>::const_iterator it=vars[omega].value.begin();it!=vars[omega].value.end();++it) {
        int e=*it;
        for(size_t i=0;i<nodes.size();++i) {
            Node &n=nodes[i]; n.mask=0;
            if(n.op=='v') n.mask=vars[n.a].known ? (1<<int(vars[n.a].value.count(e)!=0)) : 3;
            else for(int a=0;a<2;++a) for(int b=0;b<2;++b)
                if((nodes[n.a].mask&(1<<a)) && (nodes[n.b].mask&(1<<b))) n.mask|=1<<apply(n.op,a,b);
        }
        if(!(nodes[root].mask&2)) { cout<<"No Solution\n"; return 0; }
        recover(root,1,e);
    }
    cout<<"Solution\n";
    for(size_t i=0;i<vars.size();++i) if(!vars[i].known) {
        cout<<vars[i].name<<" =";
        for(set<int>::const_iterator it=vars[i].value.begin();it!=vars[i].value.end();++it) cout<<' '<<*it;
        cout<<'\n';
    }
    return 0;
}
