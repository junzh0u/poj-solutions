// POJ 1783 - Fractran
// Model: gpt-6.1-sol
// Store the current integer as prime exponents. A reduced fraction is
// applicable exactly when all denominator exponents are available.
// Maintain missing requirements and a bitset of applicable fractions;
// only requirements of changed primes can change eligibility.
// The initial integer is part of the sequence, and 1 is 2^0.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
struct Factor { int p, e; Factor(int a,int b):p(a),e(b){} };
struct Need { int rule, e; Need(int a,int b):rule(a),e(b){} };
vector<Factor> up[100], down[100];
vector<Need> users[1001];
int exponent[1001], missing[100], nonTwo;
unsigned int ready[4];
vector<Factor> factor(int n) {
    vector<Factor> v;
    for(int p=2;p*p<=n;++p) if(n%p==0) {
        int e=0; do { n/=p; ++e; } while(n%p==0);
        v.push_back(Factor(p,e));
    }
    if(n>1) v.push_back(Factor(n,1));
    return v;
}
void change(int p,int delta) {
    int old=exponent[p], now=old+delta;
    if(p!=2) nonTwo+=(now!=0)-(old!=0);
    exponent[p]=now;
    for(unsigned int j=0;j<users[p].size();++j) {
        int i=users[p][j].rule, e=users[p][j].e;
        int d=(now<e)-(old<e);
        if(d) {
            missing[i]+=d;
            if(missing[i]==0) ready[i/32]|=1U<<(i%32);
            else ready[i/32]&=~(1U<<(i%32));
        }
    }
}
int main() {
    int m,n,k;
    while(scanf("%d",&m)==1 && m) {
        scanf("%d%d",&n,&k);
        memset(exponent,0,sizeof(exponent));
        memset(missing,0,sizeof(missing));
        memset(ready,0,sizeof(ready)); nonTwo=0;
        for(int p=0;p<=1000;++p) users[p].clear();
        vector<Factor> init=factor(n);
        for(unsigned int j=0;j<init.size();++j) {
            exponent[init[j].p]=init[j].e;
            if(init[j].p!=2) ++nonTwo;
        }
        for(int i=0;i<k;++i) {
            int a,b; scanf("%d%d",&a,&b);
            up[i]=factor(a); down[i]=factor(b);
            for(unsigned int j=0;j<down[i].size();++j) {
                Factor f=down[i][j]; users[f.p].push_back(Need(i,f.e));
                if(exponent[f.p]<f.e) ++missing[i];
            }
            if(!missing[i]) ready[i/32]|=1U<<(i%32);
        }
        int printed=0;
        for(;;) {
            if(nonTwo==0) {
                if(printed) putchar(' ');
                printf("%d",exponent[2]);
                if(++printed==m) break;
            }
            int block=0; while(block<4 && ready[block]==0) ++block;
            if(block==4) break;
            unsigned int bits=ready[block]; int bit=0;
            while(!(bits&1U)) { bits>>=1; ++bit; }
            int i=block*32+bit;
            for(unsigned int j=0;j<down[i].size();++j) change(down[i][j].p,-down[i][j].e);
            for(unsigned int j=0;j<up[i].size();++j) change(up[i][j].p,up[i][j].e);
        }
        putchar('\n');
    }
    return 0;
}
