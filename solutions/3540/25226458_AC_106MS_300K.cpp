// POJ 3540 - Formula
// Model: gpt-6-astra
// Evaluate the truth table, discard inessential variables, and recursively
// factor it across disjoint variable sets using AND or OR. Projections of
// the ones (AND) or zeros (OR) uniquely determine the candidate factors.
// Any valid factor is a restriction of the original function, so greedily
// choosing a split preserves read-once representability. Constants are No:
// the grammar has no constant literals and a read-once formula is nonconstant.
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
struct Node { char op; int a,b; Node(char c,int x,int y):op(c),a(x),b(y){} };
vector<Node> nodes;
string s; int pos=0;
int add(char c,int a=-1,int b=-1){nodes.push_back(Node(c,a,b));return nodes.size()-1;}
int parseOr();
int term(){
    char c=s[pos++];
    if(c=='~'){int a=term();return add(c,a);}
    if(c=='('){int a=parseOr();++pos;return a;}
    return add(c);
}
int parseAnd(){int a=term();while(pos<(int)s.size()&&s[pos]=='&'){++pos;int b=term();a=add('&',a,b);}return a;}
int parseOr(){int a=parseAnd();while(pos<(int)s.size()&&s[pos]=='|'){++pos;int b=parseAnd();a=add('|',a,b);}return a;}
string solve(const vector<int>& f,const string& vars){
    int n=vars.size(),N=1<<n;
    if(n==0)return "";
    if(n==1){if(f[0]==f[1])return "";return (f[0]?"~":"")+vars;}
    for(int mask=1;mask<N-1;mask+=2){
        string va,vb; vector<int> ia(N,0),ib(N,0);
        for(int k=0;k<n;++k)if(mask&(1<<k))va+=vars[k];else vb+=vars[k];
        int na=1<<va.size(),nb=1<<vb.size();
        for(int x=0;x<N;++x){
            int a=0,b=0,pa=0,pb=0;
            for(int k=0;k<n;++k)if(mask&(1<<k)){if(x&(1<<k))a|=1<<pa;++pa;}else{if(x&(1<<k))b|=1<<pb;++pb;}
            ia[x]=a;ib[x]=b;
        }
        for(int op=0;op<2;++op){
            // op=0: OR projections of ones, combine by AND.
            // op=1: AND projections of values, combine by OR.
            vector<int> a(na,op),b(nb,op);
            for(int x=0;x<N;++x)if(f[x]!=op)a[ia[x]]=b[ib[x]]=1-op;
            bool ok=true;
            for(int x=0;x<N;++x)if((op?(a[ia[x]]|b[ib[x]]):(a[ia[x]]&b[ib[x]]))!=f[x]){ok=false;break;}
            if(ok){
                string left=solve(a,va),right=solve(b,vb);
                if(left.empty()||right.empty())return "";
                return "("+left+(op?"|":"&")+right+")";
            }
        }
    }
    return "";
}
int main(){
    string line;getline(cin,line);
    for(size_t i=0;i<line.size();++i)if(!isspace((unsigned char)line[i]))s+=line[i];
    int root=parseOr();vector<int> full(2048),value(nodes.size());
    for(int x=0;x<2048;++x){
        for(size_t i=0;i<nodes.size();++i){Node t=nodes[i];
            if(t.op=='~')value[i]=!value[t.a];
            else if(t.op=='&')value[i]=value[t.a]&value[t.b];
            else if(t.op=='|')value[i]=value[t.a]|value[t.b];
            else value[i]=(x>>(t.op-'a'))&1;
        }full[x]=value[root];
    }
    string vars;
    for(int k=0;k<11;++k){bool essential=false;for(int x=0;x<2048;++x)if(full[x]!=full[x^(1<<k)]){essential=true;break;}if(essential)vars+=char('a'+k);}
    vector<int> f(1<<vars.size());
    for(int x=0;x<(int)f.size();++x){int original=0;for(size_t k=0;k<vars.size();++k)if(x&(1<<k))original|=1<<(vars[k]-'a');f[x]=full[original];}
    string result=solve(f,vars);
    if(result.empty())cout<<"No\n";else cout<<"Yes\n"<<result<<"\n";
}
