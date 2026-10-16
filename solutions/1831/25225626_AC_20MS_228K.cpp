// POJ 1831 - 不定方程组
// Model: gpt-6-astra
// Exact small solutions; double denominators and append 2 for even sums,
// or append 3 and 6 for odd sums. All sums from 24 onward are possible.
// Handle 65536 too, as documented by the discussion board.
#include <cstdio>
#include <vector>
using namespace std;
const int small[60][9] = {
{0},
{1,1},
{0},
{0},
{2,2,2},
{0},
{0},
{0},
{0},
{3,3,3,3},
{3,2,4,4},
{3,2,3,6},
{0},
{0},
{0},
{0},
{4,4,4,4,4},
{4,3,4,4,6},
{4,3,3,6,6},
{0},
{4,2,6,6,6},
{0},
{4,2,4,8,8},
{0},
{4,2,4,6,12},
{5,5,5,5,5,5},
{5,4,4,6,6,6},
{5,3,6,6,6,6},
{5,4,4,4,8,8},
{4,2,3,12,12},
{4,2,3,10,15},
{4,2,4,5,20},
{4,2,3,9,18},
{5,3,3,9,9,9},
{5,2,8,8,8,8},
{5,2,6,9,9,9},
{5,2,6,8,8,12},
{4,2,3,8,24},
{5,2,6,6,12,12},
{5,2,6,6,10,15},
{6,4,4,8,8,8,8},
{5,2,6,6,9,18},
{5,2,4,12,12,12},
{5,2,4,10,12,15},
{5,3,3,6,8,24},
{5,2,4,9,12,18},
{5,2,4,8,16,16},
{6,3,4,8,8,12,12},
{6,3,4,8,8,10,15},
{6,3,4,6,12,12,12},
{5,2,4,8,12,24},
{5,3,3,5,10,30},
{5,2,5,5,20,20},
{5,2,5,6,10,30},
{4,2,3,7,42},
{5,2,4,7,14,28},
{6,2,6,12,12,12,12},
{5,2,5,5,15,30},
{5,3,3,4,24,24},
{5,2,3,18,18,18}
};
void construct(int s, int scale, vector<int>& a) {
    if (s < 60) {
        for (int i=1; i<=small[s][0]; ++i) a.push_back(scale*small[s][i]);
    } else if (s%2==0) {
        a.push_back(2*scale);
        construct((s-2)/2,scale*2,a);
    } else {
        a.push_back(3*scale); a.push_back(6*scale);
        construct((s-9)/2,scale*2,a);
    }
}
int main() {
    int t,s;
    if (scanf("%d",&t)!=1) return 0;
    while(t-- && scanf("%d",&s)==1) {
        if (s<60 && !small[s][0]) { puts("-1"); continue; }
        vector<int> a;
        construct(s,1,a);
        printf("%d",(int)a.size());
        for (unsigned i=0;i<a.size();++i) printf(" %d",a[i]);
        puts("");
    }
    return 0;
}
