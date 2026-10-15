// POJ 1421 - Peter's Calculator
// Model: gpt-6.1-sol
// Store definitions unevaluated. For each PRINT, parse expressions with
// precedence and memoize a three-color dependency DFS; missing names and
// back edges propagate UNDEF, even when multiplied by zero.
#include <iostream>
#include <string>
#include <map>
#include <cctype>
using namespace std;
typedef long long Integer;
map<string,string> definitions;
map<string,int> state;
map<string,Integer> values;
bool variable(const string&, Integer&);
struct Parser {
    const string &s; size_t p; bool ok;
    Parser(const string &text):s(text),p(0),ok(true) {}
    Integer factor() {
        if(s[p]=='(') { ++p; Integer v=expression(); ++p; return v; }
        if(isalpha((unsigned char)s[p])) {
            size_t b=p++; while(p<s.size() && isalnum((unsigned char)s[p])) ++p;
            Integer v=0; if(!variable(s.substr(b,p-b),v)) ok=false; return v;
        }
        bool neg=false; if(s[p]=='-') {neg=true; ++p;}
        Integer v=0; while(p<s.size() && isdigit((unsigned char)s[p])) v=v*10+s[p++]-'0';
        return neg?-v:v;
    }
    Integer term() {
        Integer v=factor(); while(p<s.size() && s[p]=='*') {++p; Integer w=factor(); v*=w;} return v;
    }
    Integer expression() {
        Integer v=term(); while(p<s.size() && (s[p]=='+' || s[p]=='-')) {
            char op=s[p++]; Integer w=term(); if(op=='+') v+=w; else v-=w;
        } return v;
    }
};
bool variable(const string &name, Integer &v) {
    if(state[name]==1 || state[name]==3) return false;
    if(state[name]==2) {v=values[name]; return true;}
    map<string,string>::iterator it=definitions.find(name);
    if(it==definitions.end()) {state[name]=3; return false;}
    state[name]=1; Parser parser(it->second); v=parser.expression();
    if(!parser.ok) {state[name]=3; return false;}
    state[name]=2; values[name]=v; return true;
}
string compact(const string &s) {
    string r; for(size_t i=0;i<s.size();++i) if(!isspace((unsigned char)s[i])) r+=s[i]; return r;
}
int main() {
    string line;
    while(getline(cin,line)) {
        size_t colon=line.find(':');
        if(colon!=string::npos) {definitions[compact(line.substr(0,colon))]=compact(line.substr(line.find('=',colon)+1)); continue;}
        string s=compact(line); if(s.empty()) continue;
        if(s=="RESET") definitions.clear();
        else {state.clear(); values.clear(); Integer v=0; if(variable(s.substr(5),v)) cout<<v<<'\n'; else cout<<"UNDEF\n";}
    }
    return 0;
}
