// POJ 2655 - Structural Equivalence
// Model: gpt-6.1-sol
// Resolve aliases, then remove incompatible pairs from the greatest fixed point
// of ordered structural equivalence. Recursive structures need no unfolding.
// Whitespace is optional; punctuation between structure fields is ignored.
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
struct Type { int kind, alias; vector<int> fields; Type():kind(-1),alias(-1){} };
Type t[26];
int root(int x) { for(int i=0;i<26 && t[x].alias>=0;i++) x=t[x].alias; return x; }
void solve() {
    bool eq[26][26]; int r[26];
    for(int i=0;i<26;i++) r[i]=root(i);
    for(int i=0;i<26;i++) for(int j=0;j<26;j++) {
        Type &a=t[r[i]], &b=t[r[j]];
        eq[i][j]=a.kind==b.kind && a.fields.size()==b.fields.size();
    }
    bool changed=true;
    while(changed) {
        changed=false;
        for(int i=0;i<26;i++) for(int j=0;j<26;j++) if(eq[i][j]) {
            Type &a=t[r[i]], &b=t[r[j]];
            for(size_t k=0;k<a.fields.size();k++) if(!eq[a.fields[k]][b.fields[k]]) {
                eq[i][j]=false; changed=true; break;
            }
        }
    }
    bool done[26]={false};
    for(int i=0;i<26;i++) if(t[i].kind!=-1 && !done[i]) {
        cout << char('A'+i); done[i]=true;
        for(int j=i+1;j<26;j++) if(t[j].kind!=-1 && !done[j] && eq[i][j]) {
            cout << ' ' << char('A'+j); done[j]=true;
        }
        cout << '\n';
    }
}
int main() {
    string line; bool first=true, have=false;
    while(getline(cin,line)) {
        string s;
        for(size_t i=0;i<line.size();i++) if(!isspace((unsigned char)line[i])) s+=line[i];
        if(s.empty()) continue;
        if(s[0]=='-') {
            if(have) { if(!first) cout << '\n'; solve(); first=false; }
            if(s=="--") break;
            for(int i=0;i<26;i++) t[i]=Type();
            have=false; continue;
        }
        int id=s[4]-'A'; string rhs=s.substr(s.find('=')+1);
        have=true;
        if(rhs[0]>='A' && rhs[0]<='Z') { t[id].kind=4; t[id].alias=rhs[0]-'A'; }
        else if(rhs[0]=='s') {
            t[id].kind=3;
            for(size_t i=0;i<rhs.size();i++) if(rhs[i]>='A' && rhs[i]<='Z') t[id].fields.push_back(rhs[i]-'A');
        } else t[id].kind=rhs=="int"?0:rhs=="real"?1:2;
    }
    return 0;
}
