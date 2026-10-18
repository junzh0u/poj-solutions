// POJ 3447 - Calendar of Events
// Model: gpt-6-astra
// Assign equal meetings distinct target positions. Place the suffix with
// pancake flips, then use a complete BFS table for the first nine positions.
// All 9! states are at distance at most 10: 2*(30-9)+10 = 52 flips.
// For fewer than nine meetings, ordinary pancake sorting suffices.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
int fac[10], bits[512];
unsigned char moveToParent[362880], depth[362880];
struct Perm { unsigned char p[9]; };
int rankPerm(const unsigned char *p) {
    int r=0, used=0;
    for(int i=0;i<9;++i) {
        r += (p[i]-bits[used & ((1<<p[i])-1)])*fac[8-i];
        used |= 1<<p[i];
    }
    return r;
}
void build() {
    fac[0]=1;
    for(int i=1;i<=9;++i) fac[i]=fac[i-1]*i;
    for(int i=1;i<512;++i) bits[i]=bits[i>>1]+(i&1);
    vector<Perm> q(1);
    for(int i=0;i<9;++i) q[0].p[i]=i;
    moveToParent[0]=1;
    for(size_t h=0;h<q.size();++h) {
        Perm cur=q[h];
        int d=depth[rankPerm(cur.p)];
        for(int k=2;k<=9;++k) {
            reverse(cur.p,cur.p+k);
            int r=rankPerm(cur.p);
            if(!moveToParent[r]) {
                moveToParent[r]=k;
                depth[r]=d+1;
                assert(depth[r]<=10);
                q.push_back(cur);
            }
            reverse(cur.p,cur.p+k);
        }
    }
    assert(q.size()==362880);
}
void flip(vector<int>& p, int k, vector<int>& ans) {
    if(k<2) return;
    reverse(p.begin(),p.begin()+k);
    ans.push_back(k);
}
int main() {
    build();
    int n;
    while(cin>>n && n) {
        vector<int> a(n), b(n), p(n), used(n,0), ans;
        for(int i=0;i<n;++i) cin>>a[i];
        for(int i=0;i<n;++i) cin>>b[i];
        for(int i=0;i<n;++i) {
            int j=0;
            while(used[j] || a[i]!=b[j]) ++j;
            p[i]=j; used[j]=1;
        }
        int remain=n;
        while(remain>9 || (n<9 && remain>1)) {
            if(p[remain-1]!=remain-1) {
                int j=0;
                while(p[j]!=remain-1) ++j;
                flip(p,j+1,ans);
                flip(p,remain,ans);
            }
            --remain;
        }
        if(remain==9) {
            unsigned char small[9];
            for(;;) {
                for(int i=0;i<9;++i) small[i]=p[i];
                int r=rankPerm(small);
                if(!r) break;
                flip(p,moveToParent[r],ans);
            }
        }
        assert(ans.size()<=52);
        for(int i=0;i<n;++i) assert(p[i]==i);
        for(size_t i=0;i<ans.size();++i) {
            if(i) cout<<' ';
            cout<<ans[i];
        }
        cout<<'\n';
    }
}
