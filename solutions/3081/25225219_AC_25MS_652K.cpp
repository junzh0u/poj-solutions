// POJ 3081 - Core Wars
// Model: gpt-6.1-sol
// Simulate alternating instructions in a circular memory array. Resolve both
// addresses before changing memory, and retain the executing instruction.
// All field values, including negative input operands, are normalized modulo 8000.
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
const int N=8000;
struct Instruction {
    string op;
    char am,bm;
    int a,b;
    Instruction():op("DAT"),am('#'),bm('#'),a(0),b(0) {}
    bool operator==(const Instruction& x) const {
        return op==x.op && am==x.am && bm==x.bm && a==x.a && b==x.b;
    }
};
Instruction mem[N];
int norm(int x) { return (x%N+N)%N; }
int address(int ip,char mode,int value) {
    int p=(ip+value)%N;
    return mode=='@' ? (p+mem[p].b)%N : p;
}
int main() {
    int cases; cin>>cases;
    while(cases--) {
        for(int i=0;i<N;++i) mem[i]=Instruction();
        int ip[2];
        for(int w=0;w<2;++w) {
            int m; cin>>m>>ip[w];
            for(int j=0;j<m;++j) {
                Instruction x; string a,b;
                cin>>x.op>>a>>b;
                x.am=a[0]; x.bm=b[0];
                x.a=norm(atoi(a.c_str()+1)); x.b=norm(atoi(b.c_str()+1));
                mem[(ip[w]+j)%N]=x;
            }
        }
        int winner=0;
        for(int step=0;step<32000;++step) {
            int w=step%2,p=ip[w]; Instruction x=mem[p];
            if(x.op=="DAT") { winner=2-w; break; }
            int a=address(p,x.am,x.a),b=address(p,x.bm,x.b);
            int next=(p+1)%N;
            if(x.op=="MOV") {
                if(x.am=='#') mem[b].b=x.a;
                else mem[b]=mem[a];
            } else if(x.op=="ADD") {
                if(x.am=='#') mem[b].b=(mem[b].b+x.a)%N;
                else {
                    Instruction src=mem[a];
                    mem[b].a=(mem[b].a+src.a)%N;
                    mem[b].b=(mem[b].b+src.b)%N;
                }
            } else if(x.op=="JMP") next=a;
            else if(x.op=="JMZ") { if(mem[b].b==0) next=a; }
            else if(x.op=="SLT") {
                if((x.am=='#'?x.a:mem[a].b)<mem[b].b) next=(p+2)%N;
            } else if(x.op=="CMP") { if(mem[a]==mem[b]) next=(p+2)%N; }
            ip[w]=next;
        }
        if(winner) cout<<"Program #"<<winner<<" is the winner.\n";
        else cout<<"Programs are tied.\n";
    }
}
