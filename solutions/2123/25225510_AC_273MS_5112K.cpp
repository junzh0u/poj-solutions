// POJ 2123 - Artinals
// Model: gpt-6.1-sol
// Intern sorted element vectors to share all equal hereditarily finite sets.
// Compare canonical vectors lexicographically; equal children share an id.
// Merge sorted vectors for operations, reducing ordinal and identity cases
// directly so long unary-union chains never repeatedly expand ordinals.
// Inclusion is non-strict; numeric names are ordinary assignable variables.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;
vector<vector<int> > nodes;
vector<int> ordinal;
map<vector<int>,int> interned;
map<string,int> vars;
int cmp(int a,int b) {
    if(a==b) return 0;
    if(ordinal[a]>=0 && ordinal[b]>=0) return ordinal[a]<ordinal[b]?-1:1;
    const vector<int>& x=nodes[a]; const vector<int>& y=nodes[b];
    size_t i=0;
    for(;i<x.size() && i<y.size();++i) {
        int c=cmp(x[i],y[i]); if(c) return c;
    }
    return x.size()<y.size()?-1:1;
}
struct Less { bool operator()(int a,int b)const{return cmp(a,b)<0;} };
int makeSet(vector<int> x) {
    sort(x.begin(),x.end(),Less()); x.erase(unique(x.begin(),x.end()),x.end());
    map<vector<int>,int>::iterator it=interned.find(x);
    if(it!=interned.end()) return it->second;
    int o=(int)x.size();
    for(size_t i=0;i<x.size();++i) if(ordinal[x[i]]!=(int)i) {o=-1;break;}
    int id=(int)nodes.size(); nodes.push_back(x); ordinal.push_back(o); interned[x]=id;
    return id;
}
int binary(int a,int b,char op) {
    if(a==b) return (op=='+' || op=='*')?a:0;
    if(a==0) return (op=='+' || op=='^')?b:0;
    if(b==0) return op=='*'?0:a;
    if(ordinal[a]>=0 && ordinal[b]>=0) {
        if(op=='+')return ordinal[a]>ordinal[b]?a:b;
        if(op=='*')return ordinal[a]<ordinal[b]?a:b;
        if(op=='-' && ordinal[a]<=ordinal[b])return 0;
    }
    // No nodes are added until both input vectors have been consumed.
    const vector<int>& x=nodes[a]; const vector<int>& y=nodes[b];
    vector<int> z; size_t i=0,j=0;
    while(i<x.size() || j<y.size()) {
        int c=i==x.size()?1:(j==y.size()?-1:cmp(x[i],y[j]));
        if(c<0) {if(op!='*') z.push_back(x[i]); ++i;}
        else if(c>0) {if(op=='+' || op=='^')z.push_back(y[j]); ++j;}
        else {if(op=='+' || op=='*')z.push_back(x[i]); ++i;++j;}
    }
    return makeSet(z);
}
int unary(int a,char op) {
    if(ordinal[a]>=0) return op=='*' || a==0?0:nodes[a].back();
    vector<int> elems=nodes[a];
    int r=op=='*'?elems[0]:0;
    for(size_t i=op=='*'?1:0;i<elems.size();++i) r=binary(r,elems[i],op);
    return r;
}
void printSet(int a) {
    if(ordinal[a]>=0) {cout<<ordinal[a];return;}
    cout<<'{';
    for(size_t i=0;i<nodes[a].size();++i) {if(i)cout<<',';printSet(nodes[a][i]);}
    cout<<'}';
}
struct Parser {
    string s; size_t p;
    Parser(const string& t):s(t),p(0){}
    void space(){while(p<s.size() && isspace((unsigned char)s[p]))++p;}
    char peek(){space();return p<s.size()?s[p]:0;}
    char take(){space();return p<s.size()?s[p++]:0;}
    string ident(){space();size_t st=p;while(p<s.size()&&(isalnum((unsigned char)s[p])||s[p]=='_'))++p;return s.substr(st,p-st);}
    int expr(int level=1){
        char c=peek(); int a;
        if(c=='+'||c=='*'){take();a=unary(expr(c=='*'?6:5),c);}
        else if(c=='('){take();a=expr();take();}
        else if(c=='{'){
            take();vector<int> x;
            if(peek()!='}') {do{x.push_back(expr());}while(peek()==',' && take());}
            take();a=makeSet(x);
        } else a=vars[ident()];
        for(;;){
            c=peek(); int pr=c=='^'?1:(c=='-'?2:(c=='+'?3:(c=='*'?4:0)));
            // The arrow relation starts with '-', rather than subtraction.
            if(c=='-'){size_t q=p+1;while(q<s.size()&&isspace((unsigned char)s[q]))++q;if(q<s.size()&&s[q]=='>')pr=0;}
            if(pr<level)break;
            take();int b=expr(pr+1);a=binary(a,b,c);
        }
        return a;
    }
};
bool contains(int a,int b){return binary_search(nodes[a].begin(),nodes[a].end(),b,Less());}
bool subset(int a,int b){return includes(nodes[b].begin(),nodes[b].end(),nodes[a].begin(),nodes[a].end(),Less());}
int main(){
    makeSet(vector<int>());
    vector<int> ords;
    for(int n=0;n<=512;++n){int a=makeSet(ords);ostringstream os;os<<n;vars[os.str()]=a;ords.push_back(a);}
    string line;
    while(getline(cin,line)){
        if(!line.empty()&&line[line.size()-1]=='\r')line.erase(line.size()-1);
        Parser p(line);char c=p.peek();
        if(!c)continue;
        if(c=='#'){cout<<line<<'\n';continue;}
        if(c=='!'){p.take();printSet(p.expr());cout<<'\n';}
        else if(c=='?'){
            p.take();int a=p.expr();string rel;rel+=p.take();
            c=p.peek();if(c=='='||c=='>'||c=='<'||c=='-')rel+=p.take();
            int b=p.expr();bool r=false;int order=cmp(a,b);
            if(rel=="<")r=order<0;else if(rel==">")r=order>0;
            else if(rel=="=")r=a==b;else if(rel=="<=")r=order<=0;
            else if(rel==">=")r=order>=0;else if(rel=="<>")r=a!=b;
            else if(rel=="->")r=contains(b,a);else if(rel=="<-")r=contains(a,b);
            else if(rel=="<<")r=subset(a,b);else if(rel==">>")r=subset(b,a);
            cout<<(r?"TRUE":"FALSE")<<'\n';
        } else {string name=p.ident();p.take();p.take();int a=p.expr();vars[name]=a;}
    }
    return 0;
}
