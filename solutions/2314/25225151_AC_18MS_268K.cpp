// POJ 2314 - POJ language
// Model: gpt-6.1-sol
// Compile nested statements to instructions with explicit conditional jumps.
// Variables are global and initially zero; # terminates even inside a loop.
// Boolean constants may be signed, while assignment operands are nonnegative.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <cstdlib>
using namespace std;
struct Operand {
    bool variable;
    int value;
    Operand(bool v=false, int x=0): variable(v), value(x) {}
};
struct Instruction {
    int type, target, dest;
    char op;
    Operand a,b;
    Instruction(int t=0):type(t),target(0),dest(0),op(0) {}
};
string source;
size_t pos=0;
map<string,int> names;
vector<int> values;
vector<Instruction> code;
int variable() {
    size_t start=pos;
    while(pos<source.size() && (isalnum((unsigned char)source[pos]) || source[pos]=='_')) ++pos;
    string name=source.substr(start,pos-start);
    map<string,int>::iterator it=names.find(name);
    if(it!=names.end()) return it->second;
    int id=values.size(); names[name]=id; values.push_back(0); return id;
}
Operand operand() {
    if(source[pos]=='-' || isdigit((unsigned char)source[pos])) {
        const char *begin=source.c_str()+pos;
        char *end;
        long v=strtol(begin,&end,10);
        pos=end-source.c_str();
        return Operand(false,(int)v);
    }
    return Operand(true,variable());
}
void block() {
    while(pos<source.size() && source[pos]!='}') {
        if(source[pos]=='#') {
            ++pos; Instruction ins(1); ins.a=operand(); ++pos;
            code.push_back(ins);
        } else if(source.compare(pos,3,"if(")==0 || source.compare(pos,6,"while(")==0) {
            bool loop=source[pos]=='w'; pos+=loop?6:3;
            Instruction ins(2); ins.a=operand();
            if(source[pos]=='=') { ins.op='='; pos+=2; }
            else { ++pos; ins.op='<'; if(source[pos]=='=') { ins.op='L'; ++pos; } }
            ins.b=operand(); pos+=2; // closing parenthesis and opening brace
            int at=code.size(); code.push_back(ins);
            block(); ++pos;
            if(loop) { Instruction back(3); back.target=at; code.push_back(back); }
            code[at].target=code.size();
        } else {
            Instruction ins; ins.dest=variable(); ++pos; ins.a=operand();
            if(source[pos]!=';') { ins.op=source[pos++]; ins.b=operand(); }
            ++pos; code.push_back(ins);
        }
    }
}
int get(const Operand &a) { return a.variable?values[a.value]:a.value; }
int main() {
    char c; while(cin.get(c)) if(!isspace((unsigned char)c)) source+=c;
    block();
    int pc=0;
    while(pc<(int)code.size()) {
        const Instruction &i=code[pc];
        int a=get(i.a), b=get(i.b);
        if(i.type==1) { cout<<a<<'\n'; return 0; }
        if(i.type==3) { pc=i.target; continue; }
        if(i.type==2) {
            bool ok=i.op=='='?a==b:(i.op=='<'?a<b:a<=b);
            pc=ok?pc+1:i.target; continue;
        }
        values[i.dest]=i.op=='+'?a+b:(i.op=='-'?a-b:(i.op=='*'?a*b:a));
        ++pc;
    }
    return 0;
}
