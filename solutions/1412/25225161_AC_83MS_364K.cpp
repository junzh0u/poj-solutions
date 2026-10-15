// POJ 1412 - Equals are Equals
// Model: gpt-6.1-sol
// Parse the stated grammar and expand into a sparse map of monomials.
// Each key holds all 26 exponents; zero coefficients are removed.
// Spaces are skipped between tokens, never removed inside integer tokens.
#include <iostream>
#include <string>
#include <map>
#include <cctype>
using namespace std;
typedef map<string,int> Poly;
const string unit(26, char(0));
void add(Poly &a, const Poly &b, int sign) {
    for (Poly::const_iterator it=b.begin(); it!=b.end(); ++it) {
        int &v=a[it->first]; v+=sign*it->second;
        if(v==0) a.erase(it->first);
    }
}
Poly multiply(const Poly &a,const Poly &b) {
    Poly c;
    for(Poly::const_iterator i=a.begin();i!=a.end();++i)
        for(Poly::const_iterator j=b.begin();j!=b.end();++j) {
            string k=unit;
            for(int v=0;v<26;++v) k[v]=i->first[v]+j->first[v];
            c[k]+=i->second*j->second;
        }
    for(Poly::iterator i=c.begin();i!=c.end();) {
        if(!i->second) {Poly::iterator old=i++; c.erase(old);} else ++i;
    }
    return c;
}
struct Parser {
    string s; unsigned int p;
    Parser(const string &line):s(line),p(0) {}
    void spaces(){while(p<s.size() && isspace(s[p])) ++p;}
    Poly expression(){
        Poly a=term(); spaces();
        while(p<s.size() && (s[p]=='+' || s[p]=='-')) {
            int sign=s[p++]=='+'?1:-1;
            Poly b=term(); add(a,b,sign); spaces();
        }
        return a;
    }
    Poly term(){
        Poly a; a[unit]=1; spaces();
        while(p<s.size() && s[p]!='+' && s[p]!='-' && s[p]!=')') {
            a=multiply(a,factor()); spaces();
        }
        return a;
    }
    Poly factor(){
        spaces(); Poly a;
        if(s[p]=='('){++p;a=expression();spaces();++p;}
        else if(isdigit(s[p])) {
            int v=0;while(p<s.size() && isdigit(s[p])) v=v*10+s[p++]-'0';
            if(v) a[unit]=v;
        } else {
            int v=s[p++]-'a',e=1;spaces();
            if(p<s.size() && s[p]=='^'){++p;spaces();e=s[p++]-'0';}
            string k=unit;k[v]=char(e);a[k]=1;
        }
        return a;
    }
};
int main(){
    string line;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line==".") break;
        Poly expected=Parser(line).expression();
        while(getline(cin,line)) {
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if(line==".") break;
            cout<<(Parser(line).expression()==expected?"yes":"no")<<'\n';
        }
        cout<<".\n";
    }
    return 0;
}
