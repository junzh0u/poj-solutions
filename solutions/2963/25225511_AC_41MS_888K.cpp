// POJ 2963 - Lazy Evaluation
// Model: gpt-6.1-sol
// Parse expressions once; evaluate closures with memoized argument thunks.
// Strict mode forces all arguments. Discard both counts if either mode loops.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;
struct Node { int kind, value; string name; vector<int> child; };
struct Fun { vector<string> param; int body; };
vector<Node> nodes;
vector<Fun> funcs;
map<string,int> names;
string builtin[9]={"add","sub","mult","div","rem","true","false","eq","gt"};
int parse(const string &s,int &p,const vector<string> &params) {
    while(p<(int)s.size() && s[p]==' ') ++p;
    Node n; n.kind=0; n.value=0;
    if(s[p]=='(') {
        ++p; n.kind=3;
        while(true) {
            while(s[p]==' ') ++p;
            if(s[p]==')') { ++p; break; }
            n.child.push_back(parse(s,p,params));
        }
    } else {
        int b=p; while(p<(int)s.size() && s[p]!=' ' && s[p]!=')') ++p;
        n.name=s.substr(b,p-b);
        if(n.name[0]=='-' || (n.name[0]>='0' && n.name[0]<='9')) n.value=(int)strtol(n.name.c_str(),0,10);
        else { n.kind=1; for(int i=0;i<(int)params.size();++i) if(params[i]==n.name) {n.kind=2;n.value=i;break;} }
    }
    nodes.push_back(n);return (int)nodes.size()-1;
}
struct Value { bool function; int x; Value(bool f=false,int v=0):function(f),x(v){} };
struct Thunk { int node,env; bool ready; Value val; Thunk(int n,int e):node(n),env(e),ready(false){} };
vector<Thunk> thunks;
vector<vector<int> > envs;
int calls, counts[5]; bool strictMode;
Value eval(int n,int e);
Value force(int t) {
    if(thunks[t].ready) return thunks[t].val;
    Value v=eval(thunks[t].node,thunks[t].env);
    thunks[t].ready=true;thunks[t].val=v;return v;
}
Value eval(int n,int e) {
    const Node &a=nodes[n];
    if(a.kind==0) return Value(false,a.value);
    if(a.kind==1) return Value(true,names[a.name]);
    if(a.kind==2) return force(envs[e][a.value]);
    Value f=eval(a.child[0],e);
    if(++calls>2345) throw 1;
    vector<int> args;
    for(int i=1;i<(int)a.child.size();++i) {
        int t=(int)thunks.size();thunks.push_back(Thunk(a.child[i],e));args.push_back(t);
        if(strictMode) force(t);
    }
    int id=f.x;
    if(id<9) {
        if(id==5 || id==6) return force(args[id==5?0:1]);
        int x=force(args[0]).x,y=force(args[1]).x;
        if(id<5) ++counts[id];
        if(id==0) return Value(false,(int)((unsigned)x+(unsigned)y));
        if(id==1) return Value(false,(int)((unsigned)x-(unsigned)y));
        if(id==2) return Value(false,(int)((unsigned)x*(unsigned)y));
        if(id==3) return Value(false,x==(-2147483647-1)&&y==-1?x:x/y);
        if(id==4) return Value(false,x==(-2147483647-1)&&y==-1?0:x%y);
        return Value(true,(id==7?x==y:x>y)?5:6);
    }
    int ne=(int)envs.size();envs.push_back(args);
    return eval(funcs[id-9].body,ne);
}
int main() {
    for(int i=0;i<9;++i) names[builtin[i]]=i;
    string line; vector<string> defs;
    while(getline(cin,line) && !line.empty() && line!="\r") {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        defs.push_back(line);
        istringstream in(line);string name;in>>name;names[name]=(int)funcs.size()+9;funcs.push_back(Fun());
    }
    for(int i=0;i<(int)defs.size();++i) {
        int eq=(int)defs[i].find('=');istringstream in(defs[i].substr(0,eq));string s;in>>s;
        while(in>>s) funcs[i].param.push_back(s);
        int p=eq+1;funcs[i].body=parse(defs[i],p,funcs[i].param);
    }
    int total[2][5]={{0}};
    while(getline(cin,line) && !line.empty() && line!="\r") {
        if(line[line.size()-1]=='\r') line.erase(line.size()-1);
        vector<string> empty;int p=0,n=parse(line,p,empty),local[2][5];bool ok=true;
        for(int mode=0;mode<2;++mode) {
            calls=0;for(int j=0;j<5;++j) counts[j]=0;
            thunks.clear();envs.clear();envs.push_back(vector<int>());strictMode=mode!=0;
            try {eval(n,0);} catch(int) {ok=false;}
            for(int j=0;j<5;++j) local[mode][j]=counts[j];
        }
        if(ok) for(int m=0;m<2;++m) for(int j=0;j<5;++j) total[m][j]+=local[m][j];
    }
    cout<<"operator lazy_evaluation strict_evaluation\n";
    for(int j=0;j<5;++j) cout<<builtin[j]<<' '<<total[0][j]<<' '<<total[1][j]<<'\n';
}
