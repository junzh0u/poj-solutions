// POJ 1261 - Huffman Trees
// Model: gpt-6.1-sol
// Backtrack over header boundaries while maintaining a prefix trie.
// Every internal node has all N children: missing child paths must each
// occur in a remaining codeword. Their digit counts give a lower bound
// on the remaining header, and Z fixes the total internal-node budget.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
struct Node { int child[10], cnt[10], depth; bool leaf; } tr[210];
int z,n,limit,used,internals,L,suffix[201][10],cuts[21];
string s;
bool feasible(int pos,int left) {
    int need[10]={0}, holes=0;
    for(int u=0;u<used;++u) if(!tr[u].leaf) {
        for(int d=0;d<n;++d) if(!tr[u].child[d]) {
            ++holes;
            for(int c=0;c<n;++c) need[c]+=tr[u].cnt[c]+(c==d);
        }
    }
    if(holes>left) return false;
    for(int c=0;c<n;++c) if(need[c]>suffix[pos][c]) return false;
    return true;
}
bool dfs(int pos,int k) {
    if(k==z) return pos==L && internals==limit;
    if(L-pos<z-k || L-pos>(z-k)*limit || !feasible(pos,z-k)) return false;
    int u=0,oldUsed=used,oldInternal=internals;
    int firstParent=-1,firstDigit=-1;
    for(int len=1;len<=limit && pos+len<=L-(z-k-1);++len) {
        int d=s[pos+len-1]-'0';
        if(tr[u].leaf) break;
        if(!tr[u].child[d]) {
            if(firstParent<0) {firstParent=u;firstDigit=d;}
            int v=used++;
            memset(&tr[v],0,sizeof(Node));
            tr[v].depth=tr[u].depth+1;
            for(int c=0;c<n;++c) tr[v].cnt[c]=tr[u].cnt[c]+(c==d);
            tr[u].child[d]=v;
            u=v;
            tr[u].leaf=true;
            cuts[k]=pos+len;
            if(dfs(pos+len,k+1)) return true;
            tr[u].leaf=false;
            ++internals;
            if(internals>limit) break;
        } else u=tr[u].child[d];
    }
    if(firstParent>=0) tr[firstParent].child[firstDigit]=0;
    used=oldUsed; internals=oldInternal;
    return false;
}
int main() {
    int T; cin>>T;
    while(T--) {
        cin>>z>>n>>s; L=s.size();limit=(z-1)/(n-1);
        memset(tr,0,sizeof(tr));used=1;internals=1;
        memset(suffix,0,sizeof(suffix));
        for(int i=L-1;i>=0;--i) {
            for(int d=0;d<n;++d) suffix[i][d]=suffix[i+1][d];
            ++suffix[i][s[i]-'0'];
        }
        if(dfs(0,0)) {
            int p=0;
            for(int i=0;i<z;++i) {cout<<i<<"->"<<s.substr(p,cuts[i]-p)<<'\n';p=cuts[i];}
        }
    }
}
