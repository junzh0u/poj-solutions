// POJ 2012 - Triangle Cuts
// Model: gpt-6.1-sol
// Reverse the cuts by joining convex polygons along a complete edge.
// Store clockwise angle cycles for every subset of the four pieces.
// Endpoint angle sums must not exceed 180; straight vertices disappear.
// A join may scale either piece freely, so side lengths impose no condition.
// Only cyclic rotations are identified: reflecting a piece is forbidden.
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
typedef vector<int> Poly;
Poly canonical(Poly p) {
    Poly best=p;
    for (int i=1;i<(int)p.size();++i) {
        rotate(p.begin(),p.begin()+1,p.end());
        if(p<best) best=p;
    }
    return best;
}
int bits(int x) { int n=0; for(;x;x>>=1)n+=x&1; return n; }
bool solve(const Poly &target, const vector<Poly> &piece) {
    set<Poly> dp[16];
    for(int i=0;i<4;++i) dp[1<<i].insert(canonical(piece[i]));
    for(int mask=1;mask<16;++mask) {
        if(bits(mask)==1)continue;
        for(int a=(mask-1)&mask;a;a=(a-1)&mask) {
            int b=mask^a;
            if(a>b)continue;
            for(set<Poly>::const_iterator u=dp[a].begin();u!=dp[a].end();++u)
            for(set<Poly>::const_iterator v=dp[b].begin();v!=dp[b].end();++v) {
                int n=u->size(),m=v->size();
                for(int i=0;i<n;++i)for(int j=0;j<m;++j) {
                    int first=(*u)[(i+1)%n]+(*v)[j];
                    int last=(*u)[i]+(*v)[(j+1)%m];
                    if(first>180||last>180)continue;
                    Poly p;
                    if(first<180)p.push_back(first);
                    for(int k=2;k<n;++k)p.push_back((*u)[(i+k)%n]);
                    if(last<180)p.push_back(last);
                    for(int k=2;k<m;++k)p.push_back((*v)[(j+k)%m]);
                    if(p.size()>=3 && (int)p.size()<=7-bits(mask))
                        dp[mask].insert(canonical(p));
                }
            }
        }
    }
    return dp[15].count(canonical(target))!=0;
}
int main(){
    Poly big(3); vector<Poly> piece(4,Poly(3));
    while(cin>>big[0]>>big[1]>>big[2] && big[0]) {
        for(int i=0;i<4;++i)for(int j=0;j<3;++j)cin>>piece[i][j];
        cout<<(solve(big,piece)?"yes":"no")<<'\n';
    }
}
