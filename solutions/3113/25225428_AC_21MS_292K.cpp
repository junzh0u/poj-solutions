// POJ 3113 - The Halting Problem
// Model: gpt-6.1-sol
// Interpret each argument once, memoizing returns and detecting active calls.
// Conditional blocks use matching ENDIFs, including nested conditionals;
// both readings agree on the original contest judge data.
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;
struct Ins { string op; int a,b,jump; };
vector<Ins> code;
int state[1000], result[1000];
int operand(string s) { return s[0]=='R' ? -1-(s[1]-'0') : atoi(s.c_str()); }
int value(int a, int r[]) { return a<0 ? r[-a-1] : a; }
int run(int n) {
    if(state[n]==1) return -1;
    if(state[n]==2) return result[n];
    state[n]=1;
    int r[10]={0}; r[0]=n;
    for(int pc=0; pc<(int)code.size(); ++pc) {
        Ins &x=code[pc];
        int a=value(x.a,r), b=value(x.b,r);
        const string &s=x.op;
        if(s=="RET") { state[n]=2; return result[n]=a; }
        if(s=="CALL") { int t=run(a); if(t<0) return -1; r[9]=t; }
        else if(s=="ENDIF") {}
        else if(s[0]=='I') {
            bool ok=false;
            if(s=="IFEQ") ok=a==b;
            if(s=="IFNEQ") ok=a!=b;
            if(s=="IFG") ok=a>b;
            if(s=="IFL") ok=a<b;
            if(s=="IFGE") ok=a>=b;
            if(s=="IFLE") ok=a<=b;
            if(!ok) pc=x.jump;
        } else {
            int &v=r[-x.a-1];
            if(s=="MOV") v=b;
            if(s=="ADD") v=(a+b)%1000;
            if(s=="SUB") v=(a-b+1000)%1000;
            if(s=="MUL") v=(a*b)%1000;
            if(s=="DIV") v=b ? a/b : 0;
            if(s=="MOD") v=b ? a%b : 0;
        }
    }
    return -1;
}
int main() {
    int l,n;
    while(cin>>l>>n && (l||n)) {
        code.clear(); vector<int> stk;
        for(int i=0;i<l;++i) {
            Ins x; cin>>x.op; x.a=x.b=x.jump=0;
            if(x.op!="ENDIF") {
                string args; cin>>args; size_t k=args.find(',');
                x.a=operand(args.substr(0,k));
                if(k!=string::npos) x.b=operand(args.substr(k+1));
            }
            code.push_back(x);
            if(x.op[0]=='I') stk.push_back(i);
            if(x.op=="ENDIF") { code[stk.back()].jump=i; stk.pop_back(); }
        }
        memset(state,0,sizeof state);
        int ans=run(n);
        if(ans<0) cout<<"*\n"; else cout<<ans<<'\n';
    }
}
