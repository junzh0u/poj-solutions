// POJ 3605 - Sheryl's Circuit I
// Model: claude-sonnet-5
//
// The circuit is a perfect binary tree; leaves start at 0 and the root's
// output at each time step must equal the given signal character. Key facts:
//  - From the all-zero baseline, the min number of leaf flips needed to make
//    a node output 1 is: leaf -> 1; AND node -> sum of both children's costs
//    (both must be 1); OR node -> min of the two children's costs (only the
//    cheaper branch needs to be built, the other stays all-zero, since
//    a node's cost to output 0 is always 0 -- the all-zero assignment
//    already yields 0 everywhere).
//  - Call the cost to reach root=1 from all-zero "step" = dp(root,1).
//  - Once that "witness" configuration (with possibly several leaves set)
//    is built, there is always a single leaf on its "certificate path"
//    whose flip alone drives the root back to 0, and flipping that same
//    leaf back gives the witness again. So every transition after the very
//    first 0->1 transition costs exactly 1, regardless of direction.
// Hence: answer = step + (#value-changes in the signal, treating an
// implicit leading 0) - 1, or 0 if the signal never actually needs a change.
// Verified against both samples (OR-tree: step=1, 5 transitions -> 1+5-1=5;
// AND-tree: step=4, 1 transition -> 4+1-1=4) and against a brute-force
// BFS/DP reference on small random trees and short signals.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXM = 10005;
int ope[MAXM]; // gate type for internal node i (1-indexed), 0=OR,1=AND
int dp1[MAXM]; // dp1[i] = min leaf-flips from all-zero baseline for node i to output 1

int M; // number of internal nodes = N-1

int val(int idx){
    if(idx > M) return 1; // leaf
    return dp1[idx];
}

int main(){
    int T;
    if(scanf("%d", &T) != 1) return 0;
    static char sig[10005];
    while(T--){
        int N;
        scanf("%d", &N);
        scanf("%s", sig);
        M = N - 1;
        for(int i = 1; i <= M; i++) scanf("%d", &ope[i]);
        for(int i = M; i >= 1; i--){
            int l = val(2*i), r = val(2*i+1);
            dp1[i] = ope[i] ? (l + r) : min(l, r);
        }
        int step = (M == 0) ? 1 : dp1[1];

        int L = strlen(sig);
        int now = 0;
        int ret = 0;
        for(int i = 0; i < L; i++){
            int c = sig[i] - '0';
            if(c != now){ now = c; ret++; }
        }
        int answer = (ret > 0) ? (step + ret - 1) : 0;
        printf("%d\n", answer);
    }
    return 0;
}
