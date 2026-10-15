// POJ 1194 - HIDDEN CODES
// Model: gpt-6.1-sol
// Enumerate inclusion-minimal subsequence covers with occurrence-list jumps.
// Any other cover can be shrunk without losing its word or blocking more items.
// Weighted interval scheduling then maximizes the total code-word length.
// Covers include both endpoints, so their length must be at most 1000.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
char text[1000005], words[100][105];
int pos[1000005], off[53], cnt[52];
int code(char c) { return c >= 'a' ? c-'a'+26 : c-'A'; }
struct Item {
    int l,r,w;
    bool operator<(const Item& b) const { return r<b.r; }
};
int main() {
    int n;
    while (scanf("%d", &n)==1) {
        for(int i=0;i<n;++i) scanf("%104s",words[i]);
        scanf("%1000000s",text);
        int m=(int)strlen(text);
        memset(cnt,0,sizeof(cnt));
        for(int i=0;i<m;++i) ++cnt[code(text[i])];
        off[0]=0;
        for(int c=0;c<52;++c) off[c+1]=off[c]+cnt[c];
        memset(cnt,0,sizeof(cnt));
        for(int i=0;i<m;++i) { int c=code(text[i]); pos[off[c]+cnt[c]++]=i; }
        vector<Item> a;
        for(int z=0;z<n;++z) {
            int len=(int)strlen(words[z]), cur=0;
            while(cur<m) {
                int p=cur-1;
                bool ok=true;
                for(int j=0;j<len;++j) {
                    int c=code(words[z][j]);
                    int *q=upper_bound(pos+off[c],pos+off[c+1],p);
                    if(q==pos+off[c+1]) {ok=false;break;}
                    p=*q;
                }
                if(!ok) break;
                int end=p;
                for(int j=len-2;j>=0;--j) {
                    int c=code(words[z][j]);
                    p=*(lower_bound(pos+off[c],pos+off[c+1],p)-1);
                }
                if(end-p+1<=1000) { Item t; t.l=p;t.r=end;t.w=len;a.push_back(t); }
                cur=p+1;
            }
        }
        sort(a.begin(),a.end());
        vector<int> ends(a.size()), dp(a.size()+1,0);
        for(size_t i=0;i<a.size();++i) {
            ends[i]=a[i].r;
            int k=(int)(lower_bound(ends.begin(),ends.begin()+i,a[i].l)-ends.begin());
            dp[i+1]=max(dp[i],dp[k]+a[i].w);
        }
        printf("%d\n",dp[a.size()]);
    }
    return 0;
}
