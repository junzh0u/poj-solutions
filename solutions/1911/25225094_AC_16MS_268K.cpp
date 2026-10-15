// POJ 1911 - A Well-Formed Problem
// Model: gpt-6.1-sol
// Scan tags with quoted attribute values, checking an explicit element stack.
// Names may repeat among siblings, but never among an element's ancestors.
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cctype>
using namespace std;
bool namechar(char c) { return isalnum((unsigned char)c) || c=='-'; }
void spaces(const string& s, size_t& p) {
    while(p<s.size() && isspace((unsigned char)s[p])) ++p;
}
string name(const string& s, size_t& p) {
    size_t b=p; while(p<s.size() && namechar(s[p])) ++p;
    return s.substr(b,p-b);
}
bool valid(const string& s) {
    vector<string> st; set<string> active; int roots=0;
    size_t p=0;
    while(p<s.size()) {
        if(s[p]!='<') {
            if(st.empty() && !isspace((unsigned char)s[p])) return false;
            ++p; continue;
        }
        ++p; bool closing=false;
        if(p<s.size() && s[p]=='/') { closing=true; ++p; }
        string tag=name(s,p); if(tag.empty()) return false;
        if(closing) {
            spaces(s,p);
            if(p==s.size() || s[p++]!='>' || st.empty() || st.back()!=tag) return false;
            active.erase(tag); st.pop_back(); continue;
        }
        if(active.count(tag)) return false;
        if(st.empty() && ++roots>1) return false;
        set<string> attrs; bool empty=false;
        while(true) {
            size_t old=p; spaces(s,p); bool separated=(old!=p);
            if(p==s.size()) return false;
            if(s[p]=='>') { ++p; break; }
            if(s[p]=='/') {
                ++p; if(p==s.size() || s[p++]!='>') return false;
                empty=true; break;
            }
            if(!separated) return false;
            string a=name(s,p);
            if(a.empty() || !attrs.insert(a).second) return false;
            spaces(s,p); if(p==s.size() || s[p++]!='=') return false;
            spaces(s,p); if(p==s.size() || s[p++]!='"') return false;
            while(p<s.size() && s[p]!='"') ++p;
            if(p==s.size()) return false;
            ++p;
        }
        if(!empty) { st.push_back(tag); active.insert(tag); }
    }
    return roots==1 && st.empty();
}
int main() {
    string line, doc; bool started=false;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line=="<?end?>" || line=="<?xml version=\"1.0\"?>") {
            if(started) cout << (valid(doc)?"well-formed":"non well-formed") << '\n';
            doc.clear(); started=true;
            if(line=="<?end?>") return 0;
        } else if(started) { doc+=line; doc+='\n'; }
    }
    if(started) cout << (valid(doc)?"well-formed":"non well-formed") << '\n';
    return 0;
}
