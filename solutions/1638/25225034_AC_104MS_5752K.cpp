// POJ 1638 - A number game
// Model: gpt-6.1-sol
// Memoize optimal score differences for sorted multisets of composites.
// As stipulated in the statement, collect available points immediately.
// A 1, 2, or 3 gives that many consecutive points; a larger prime gives
// one point and becomes composite. Track the resulting turn parity.
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
const int LIMIT=1000000;
int factor[LIMIT];
map<vector<int>, int> memo;
void collect(int n, int &turn, int &difference, vector<int> &board) {
    int points=0;
    if(n<=3) points=n;
    else if(factor[n]==n) { points=1; --n; board.push_back(n); }
    else board.push_back(n);
    if(points&1) { difference += turn ? -1 : 1; turn^=1; }
}
int solve(const vector<int> &board) {
    if(board.empty()) return 0;
    map<vector<int>,int>::iterator found=memo.find(board);
    if(found!=memo.end()) return found->second;
    int best=-1000000;
    for(int i=0;i<(int)board.size();++i) {
        if(i && board[i]==board[i-1]) continue;
        int n=board[i];
        for(int a=2;a*a<=n;++a) if(n%a==0) {
            vector<int> next=board;
            next.erase(next.begin()+i);
            int turn=1, difference=0;
            collect(a,turn,difference,next);
            collect(n/a,turn,difference,next);
            sort(next.begin(),next.end());
            int value=solve(next);
            best=max(best,difference+(turn ? -value : value));
        }
    }
    memo[board]=best;
    return best;
}
int total(int n) {
    if(n<=3) return n;
    if(factor[n]==n) return 1+total(n-1);
    return total(factor[n])+total(n/factor[n]);
}
int main() {
    for(int i=2;i<LIMIT;++i) if(!factor[i])
        for(int j=i;j<LIMIT;j+=i) if(!factor[j]) factor[j]=i;
    int cases,n;
    if(scanf("%d",&cases)!=1) return 0;
    while(cases--) {
        scanf("%d",&n);
        memo.clear();
        vector<int> board;
        int turn=0,difference=0;
        collect(n,turn,difference,board);
        int value=solve(board);
        difference += turn ? -value : value;
        int sum=total(n);
        printf("%d %d\n",(sum+difference)/2,(sum-difference)/2);
    }
    return 0;
}
