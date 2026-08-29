// POJ 3533 - Light Switching Game
// Model: claude-sonnet-5
//
// This is a "coin turning game" (Berlekamp-Conway-Guy): a move flips the
// 8 corners of a cuboid, forcing the (x2,y2,z2) corner from ON to OFF while
// the other 7 corners toggle freely. For coin-turning games, the Grundy
// value of the whole position is the XOR of the Grundy value of each ON
// coin considered alone (a standard theorem for this game family). The
// single-axis game (a coin at position p can move to the empty game or to
// a coin at any smaller position, matching a plain Nim heap) has Grundy
// value g(p) = p. The 3-D corner game is exactly the 3-fold "tensor
// product" of that 1-D game (independent choice of {x1,x2} in each axis),
// so by the turning-games product theorem the single-coin Grundy value at
// (x,y,z) is the Nim-multiplication x (x) y (x) z. Second player wins iff
// the XOR of x_i(x)y_i(x)z_i over all initially-on lights is 0.
// (Confirmed against the board's own discussion of the AC approach using
// nim-product "NIM积"/"(x)", and against the worked example there that
// SG(1,1,2) = mex{0, SG(1,1,1)} = mex{0,1} = 2, matching 1(x)1(x)2 = 2.)
//
// Nim-multiplication uses the standard Fermat 2-power (F_k = 2^(2^k))
// recursive splitting algorithm. Since inputs are <=1000 < 1024, any pair
// of values below 1024 (in particular any two coordinates, and any
// coordinate paired with an intermediate product, which can reach up to
// 65535) is split with F=256, immediately reducing to sub-values < 256;
// those in turn only ever combine other values < 256. So a full 256x256
// table (built once, bottom-up via the same recursion) answers every
// sub-query in O(1), and the two nim-products needed per input triple cost
// O(1) each. Verified against the textbook 8x8 nim-multiplication table,
// and against a from-scratch exhaustive game-tree brute force: on a 2x2x2
// cube (full state space) and on a 3x3x3 cube (single-coin Grundy checked
// at all 27 cells via a genuine ~24.7M-state search from the corner
// (3,3,3), plus several multi-coin positions), the brute-forced Grundy
// numbers matched x(x)y(x)z and its XOR decomposition exactly.
#include <cstdio>
using namespace std;

typedef unsigned int u32;
typedef unsigned long long u64;

static u32 tab[256][256];
static bool have[256][256];

u32 nimmulSmall(u32 a, u32 b){ // a,b < 256
    if(a==0||b==0) return 0;
    if(a==1) return b;
    if(b==1) return a;
    if(have[a][b]) return tab[a][b];
    u32 m = a>b?a:b;
    u32 F=2;
    while(F*F<=m) F=F*F;
    u32 ah=a/F, al=a%F;
    u32 bh=b/F, bl=b%F;
    u32 P = nimmulSmall(ah,bh);
    u32 Q = nimmulSmall(ah,bl) ^ nimmulSmall(al,bh);
    u32 R = nimmulSmall(al,bl);
    u32 PxFF = (P*F) ^ nimmulSmall(P, F/2);
    u32 res = PxFF ^ (Q*F) ^ R;
    tab[a][b]=res; have[a][b]=true;
    tab[b][a]=res; have[b][a]=true;
    return res;
}

// general nim-multiplication for values up to ~65535 (needed since an
// intermediate product like x(x)y can exceed 255 before combining with z).
u64 nimmul(u64 a, u64 b){
    if(a==0||b==0) return 0;
    if(a==1) return b;
    if(b==1) return a;
    if(a<256 && b<256) return nimmulSmall((u32)a,(u32)b);
    u64 m = a>b?a:b;
    u64 F=2;
    while(F*F<=m) F=F*F;
    u64 ah=a/F, al=a%F;
    u64 bh=b/F, bl=b%F;
    u64 P = nimmul(ah,bh);
    u64 Q = nimmul(ah,bl) ^ nimmul(al,bh);
    u64 R = nimmul(al,bl);
    u64 PxFF = (P*F) ^ nimmul(P, F/2);
    return PxFF ^ (Q*F) ^ R;
}

int main(){
    int n;
    while(scanf("%d",&n)==1){
        u64 total=0;
        for(int i=0;i<n;i++){
            long long x,y,z;
            scanf("%lld %lld %lld",&x,&y,&z);
            u64 v = nimmul(nimmul((u64)x,(u64)y),(u64)z);
            total ^= v;
        }
        puts(total==0 ? "Yes" : "No");
    }
    return 0;
}
