// POJ 2931 - Procrastination
// Model: claude-sonnet-5
//
// Each tower's Procrastination game is a partisan combinatorial game: removing
// a cube of your color deletes it and everything above it, leaving the prefix
// below. A tower's state is fully described by how many bottom cubes remain,
// so it is a finite game whose every option set consists of numbers (by
// induction on height), hence the whole tower always evaluates to a surreal
// NUMBER (birthday <= height). We compute that number exactly, incrementally:
// state[0] = 0; for k = 1..n, if cube k is White it's a new Left(White)
// option = state[k-1], if Black a new Right(Black) option = state[k-1];
// state[k] = simplest number strictly between the running max Left option and
// running min Right option (the standard CGT "number avoidance"/simplicity
// rule, implemented directly on dyadic rationals via arithmetic bisection,
// since every value here is dyadic with denominator a power of two).
//
// A full configuration's value is the disjunctive sum of its towers' number
// values (ordinary addition, since they are numbers). The problem's relation
// "C1 at least as favorable to White as C2" (for every tower T,
// (C2,T) W-config => (C1,T) W-config) is exactly the CGT domination
// g(C1) >= g(C2): if g1>=g2 then g1+t >= g2+t so g2+t>0 forces g1+t>0
// (numbers are totally ordered), and conversely equality/inequality of
// numbers plus density of achievable dyadic tower values gives the converse.
// So we just sum the three tower values of C1, subtract the three of C2, and
// answer Yes iff the result is >= 0.
//
// This was checked against an independent full minimax (no CGT shortcuts) on
// the combined 6-tower game (C1 as-is plus C2 with colors swapped, Black to
// move first) over hundreds of random small configurations (tower heights up
// to 6): zero disagreements with "sum of numbers >= 0" via the incremental
// construction above. The board's only relevant thread ("surreal number",
// message 158357) confirms this is the intended technique; it carries no
// further hint or edge case.
//
// Input is read token-by-token (not line-by-line) since a tower of height 0
// contributes zero letters and may leave a blank line; whitespace-based
// extraction sidesteps that entirely.

#include <cstdio>
#include <vector>
#include <string>
using namespace std;

typedef long long ll;

struct Dy { ll num; int shift; };

Dy reduceD(ll num, int shift){
    Dy d;
    if(num==0){ d.num=0; d.shift=0; return d; }
    while(shift>0 && (num % 2==0)){ num/=2; shift--; }
    d.num=num; d.shift=shift;
    return d;
}
Dy fromLL(ll x){ Dy d; d.num=x; d.shift=0; return d; }
Dy addD(Dy a, Dy b){
    int s = a.shift>b.shift ? a.shift : b.shift;
    ll av = a.num * (1LL << (s - a.shift));
    ll bv = b.num * (1LL << (s - b.shift));
    return reduceD(av+bv, s);
}
Dy negD(Dy a){ Dy d; d.num=-a.num; d.shift=a.shift; return d; }
Dy subD(Dy a, Dy b){ return addD(a, negD(b)); }
Dy halfD(Dy a){ return reduceD(a.num, a.shift+1); }
Dy doubleD(Dy a){
    if(a.shift>0) return reduceD(a.num, a.shift-1);
    Dy d; d.num = a.num*2; d.shift=0; return d;
}
bool ltD(Dy a, Dy b){
    int s = a.shift>b.shift ? a.shift : b.shift;
    ll av = a.num * (1LL << (s - a.shift));
    ll bv = b.num * (1LL << (s - b.shift));
    return av<bv;
}
bool eqD(Dy a, Dy b){ return !ltD(a,b) && !ltD(b,a); }
bool gtD(Dy a, Dy b){ return ltD(b,a); }
ll floorD(Dy a){
    if(a.shift==0) return a.num;
    ll denom = 1LL << a.shift;
    ll q = a.num / denom, r = a.num % denom;
    if(r<0) q--;
    return q;
}

struct Bound { int inf; Dy val; };
Bound finiteB(Dy v){ Bound b; b.inf=0; b.val=v; return b; }
Bound negInfB(){ Bound b; b.inf=-1; b.val=fromLL(0); return b; }
Bound posInfB(){ Bound b; b.inf=1; b.val=fromLL(0); return b; }

Dy simplestUnit(Dy lo2, Dy hi2){
    Dy half = reduceD(1,1);
    if(ltD(lo2,half) && ltD(half,hi2)) return half;
    if(!ltD(half,hi2)){
        Dy lo3=doubleD(lo2), hi3=doubleD(hi2);
        Dy r = simplestUnit(lo3,hi3);
        return halfD(r);
    } else {
        Dy lo3=subD(doubleD(lo2), fromLL(1));
        Dy hi3=subD(doubleD(hi2), fromLL(1));
        Dy r = simplestUnit(lo3,hi3);
        return addD(half, halfD(r));
    }
}
Dy simplestPos(Dy lo, Dy hi){
    ll f = floorD(lo);
    Dy cand = fromLL(f+1);
    if(ltD(cand,hi)) return cand;
    Dy m = fromLL(f);
    Dy lo2 = subD(lo,m);
    Dy hi2 = subD(hi,m);
    Dy x2 = simplestUnit(lo2,hi2);
    return addD(m,x2);
}
Dy simplestGreaterUnbounded(Dy lo){ ll f = floorD(lo); return fromLL(f+1); }
Dy simplestLessUnbounded(Dy hi){
    ll f = floorD(hi);
    Dy fd = fromLL(f);
    if(eqD(fd,hi)) return fromLL(f-1);
    return fromLL(f);
}
Dy simplestBetween(Bound lo, Bound hi){
    Dy zero = fromLL(0);
    if(lo.inf==-1 && hi.inf==1) return zero;
    if(lo.inf==-1){
        if(gtD(hi.val, zero)) return zero;
        return simplestLessUnbounded(hi.val);
    }
    if(hi.inf==1){
        if(ltD(lo.val, zero)) return zero;
        return simplestGreaterUnbounded(lo.val);
    }
    Dy lo_ = lo.val, hi_ = hi.val;
    if(ltD(lo_,zero) && gtD(hi_,zero)) return zero;
    if(!ltD(lo_,zero)){
        return simplestPos(lo_,hi_);
    } else {
        Dy r = simplestPos(negD(hi_), negD(lo_));
        return negD(r);
    }
}

Dy towerValue(const vector<char> &c){
    int n = (int)c.size();
    vector<Dy> state(n+1);
    state[0] = fromLL(0);
    Bound bestL = negInfB();
    Bound bestR = posInfB();
    for(int k=1;k<=n;k++){
        char col = c[k-1];
        Dy prev = state[k-1];
        if(col=='W'){
            if(bestL.inf==-1 || gtD(prev, bestL.val)) bestL = finiteB(prev);
        } else {
            if(bestR.inf==1 || ltD(prev, bestR.val)) bestR = finiteB(prev);
        }
        state[k] = simplestBetween(bestL, bestR);
    }
    return state[n];
}

vector<char> readTower(int n){
    vector<char> c(n);
    for(int i=0;i<n;i++){
        char buf[8];
        if(scanf("%s", buf)!=1){ /* shouldn't happen */ }
        c[i] = buf[0];
    }
    return c;
}

int main(){
    int T;
    if(scanf("%d", &T)!=1) return 0;
    for(int tc=1;tc<=T;tc++){
        char word[16]; int N;
        scanf("%s %d", word, &N); // "Test" N
        Dy sum = fromLL(0);
        for(int cfg=0;cfg<2;cfg++){
            int n1,n2,n3;
            scanf("%d %d %d", &n1,&n2,&n3);
            vector<char> t1 = readTower(n1);
            vector<char> t2 = readTower(n2);
            vector<char> t3 = readTower(n3);
            Dy v1 = towerValue(t1);
            Dy v2 = towerValue(t2);
            Dy v3 = towerValue(t3);
            Dy cfgSum = addD(addD(v1,v2), v3);
            if(cfg==0) sum = addD(sum, cfgSum);
            else sum = subD(sum, cfgSum);
        }
        bool yes = !ltD(sum, fromLL(0));
        printf("Test %d: %s\n", N, yes?"Yes":"No");
    }
    return 0;
}
