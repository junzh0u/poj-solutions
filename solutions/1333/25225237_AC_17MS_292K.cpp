// POJ 1333 - Christmas Gifts
// Model: gpt-6.1-sol
// Start with empty gift sets and repeatedly add every required expression.
// All expressions are monotone (except-for removes only a constant set),
// so this reaches the least satisfying assignment. Bitsets evaluate sets.
// Conditions are indexed by their explicit child id: input order may vary.
#include <cstdio>
#include <bitset>
#include <vector>
using namespace std;
typedef bitset<1001> Gifts;
struct Operand {
    int child;
    Gifts fixed;
    Operand(): child(0) {}
};
struct Rule { int target, type; Operand a,b; };
Gifts gifts[101];
Operand readOperand(int type) {
    Operand a;
    if(type == -2) scanf("%d", &a.child);
    else {
        int k, x; scanf("%d", &k);
        while(k--) { scanf("%d", &x); a.fixed.set(x); }
    }
    return a;
}
Gifts value(const Operand &a) {
    return a.child ? gifts[a.child] : a.fixed;
}
int main() {
    int tests; if(scanf("%d", &tests)!=1) return 0;
    while(tests--) {
        int n,m; scanf("%d%d", &n,&m);
        for(int i=1;i<=m;++i) gifts[i].reset();
        vector<Rule> rules;
        for(int i=0;i<m;++i) {
            int id,k; scanf("%d%d", &id,&k);
            while(k--) {
                Rule r; r.target=id; scanf("%d", &r.type);
                if(r.type == -1 || r.type == -2) r.a=readOperand(r.type);
                else {
                    int t; scanf("%d", &t); r.a=readOperand(t);
                    scanf("%d", &t); r.b=readOperand(t);
                }
                rules.push_back(r);
            }
        }
        bool changed=true;
        while(changed) {
            changed=false;
            for(size_t i=0;i<rules.size();++i) {
                const Rule &r=rules[i];
                Gifts add=value(r.a);
                if(r.type == -3) add &= value(r.b);
                if(r.type == -4) add &= ~value(r.b);
                Gifts next=gifts[r.target] | add;
                if(next != gifts[r.target]) {
                    gifts[r.target]=next; changed=true;
                }
            }
        }
        for(int i=1;i<=m;++i) {
            printf("%d",i);
            for(int j=1;j<=n;++j) if(gifts[i].test(j)) printf(" %d",j);
            putchar('\n');
        }
    }
    return 0;
}
