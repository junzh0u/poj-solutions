// POJ 2198 - Boundaries on
// Model: gpt-6.1-sol
// Simulate every rule using packed rows and Brent cycle detection.
// The initial row is step 1; both boundary cells stay white.
// Rule bit k corresponds to the binary left-center-right neighborhood k.
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
using namespace std;
typedef bitset<256> Row;
Row advanceRow(const Row &a, int rule, const Row &mask) {
    Row left=a<<1, right=a>>1, result;
    for(int k=0;k<8;++k) if(rule&(1<<k)) {
        Row term=(k&4)?left:~left;
        term &= (k&2)?a:~a;
        term &= (k&1)?right:~right;
        result |= term;
    }
    return result & mask;
}
int main() {
    string line; int caseNo=0;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line=="END OF INPUT") break;
        istringstream in(line); unsigned long limit=0; string s, extra;
        bool valid=bool(in>>limit>>s);
        if(in>>extra) valid=false;
        int n=(int)s.size();
        if(n<3 || n>256 || n%2==0 || s[0]!='W' || s[n-1]!='W') valid=false;
        Row target, mask;
        for(int i=0;i<n;++i) {
            if(s[i]!='W' && s[i]!='B') valid=false;
            if(s[i]=='B') target.set(i);
            if(i>0 && i<n-1) mask.set(i);
        }
        cout<<"LINE "<<++caseNo; bool found=false;
        if(valid && limit>0) for(int rule=0;rule<256;++rule) {
            Row current; current.set(n/2); Row anchor=current;
            unsigned long step=1, power=1, length=0;
            while(true) {
                if(current==target) { cout<<" ("<<rule<<","<<step<<")"; found=true; break; }
                if(step==limit) break;
                current=advanceRow(current,rule,mask); ++step; ++length;
                if(current==anchor) break;
                if(length==power) {
                    anchor=current; length=0;
                    if(power<=2147483647UL) power*=2;
                }
            }
        }
        if(!found) cout<<" NONE";
        cout<<'\n';
    }
}
