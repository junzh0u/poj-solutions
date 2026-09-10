// POJ 1818 - ATP
// Model: claude-fable-5-1
//
// n = 2^x players ranked 1..n; a match with rank gap > k is won by the
// better player, otherwise either may win.  Seeding and every uncertain
// result are adversarial; find the worst rank that can be champion.
//
// Approach.  A knockout is the same thing as assigning every player a
// win count w in {0..x} (exactly 2^(x-1-j) players have w = j for j < x,
// one has w = x) such that each player with w = t beats one player of
// each win count 0..t-1, and s may beat r iff r >= s - k.  For a fixed
// level j the beaters are all players with w > j and the victims those
// with w = j; both lists have the same size and, because the constraint
// sets are nested, a perfect matching exists iff the i-th largest victim
// is >= the i-th largest beater minus k.  The conditions of different
// levels only share the sets, so the whole tournament is valid iff every
// level passes this sorted pairing test.
//
// For a candidate champion c the levels are built from the top down:
// level x = {c}; at each lower level the victims are chosen as the
// smallest still-unused ranks that satisfy the pairing test (filling the
// weakest constraint first).  Smaller victims are weaker beaters below
// and leave larger ranks to be eliminated later, so the pointwise-minimal
// choice is optimal.  Candidates are scanned from n downwards and the
// first feasible one is printed.  The "next unused rank >= t" query is a
// union-find over ranks; a check is O(n log n) and the scan is well
// within the limit for n <= 5000.
//
// Verified against an exhaustive subset DP of the original tournament
// for n <= 16 (all k), the exact table for n = 32, and a CP-SAT model of
// the level formulation proving greedy+1 infeasible for n = 64 and 128.
#include <cstdio>
#include <vector>
#include <algorithm>

static int par[6005];
static int findp(int a){ while(par[a]!=a){ par[a]=par[par[a]]; a=par[a]; } return a; }

static bool feasible(int n,int x,int k,int c){
    for(int i=1;i<=n+1;i++) par[i]=i;
    par[c]=c+1;
    std::vector<int> U; U.push_back(c);          /* sorted descending */
    for(int j=x-1;j>=0;j--){
        int m=(int)U.size();
        std::vector<int> V(m);
        int lo=0;
        for(int i=m-1;i>=0;i--){
            int t=U[i]-k; if(t<lo+1) t=lo+1; if(t<1) t=1;
            t=findp(t);
            if(t>n) return false;
            par[t]=t+1;
            V[i]=t; lo=t;
        }
        std::vector<int> W(2*m);
        std::merge(U.begin(),U.end(),V.begin(),V.end(),W.begin(),std::greater<int>());
        U.swap(W);
    }
    return true;
}

int main(){
    int n,k;
    while(scanf("%d %d",&n,&k)==2){
        int x=0; while((1<<(x+1))<=n) x++;
        int ans=1;
        for(int c=n;c>=1;c--) if(feasible(n,x,k,c)){ ans=c; break; }
        printf("%d\n",ans);
    }
    return 0;
}
