// POJ 1553 - Fax Regions
// Model: gpt-6.1-sol
// Partition columns at encoded run boundaries. Process exceptional rows by
// frontier union-find; skip identical rows and count alternating complements.
// Same/different labels compare vertically, including a white imaginary row.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct DSU {
    vector<int> p;
    DSU(int n):p(n) { for(int i=0;i<n;++i)p[i]=i; }
    int root(int x) { return p[x]==x?x:p[x]=root(p[x]); }
    bool join(int a,int b) { a=root(a);b=root(b);if(a==b)return false;p[a]=b;return true; }
};
int runs(const vector<int>& a,int color) {
    int n=0;for(int i=0;i<(int)a.size();++i)if(a[i]==color&&(i==0||a[i-1]!=color))++n;return n;
}
int main() {
    ios::sync_with_stdio(false);cin.tie(0);
    long long w;
    while(cin>>w && w!=-1) {
        int r,g;cin>>r>>g;
        vector<long long> len(r),x; x.push_back(0);x.push_back(w);
        long long sum=0;
        for(int i=0;i<r;++i){cin>>len[i];sum+=len[i];x.push_back(sum%w);}
        sort(x.begin(),x.end());x.erase(unique(x.begin(),x.end()),x.end());
        int m=(int)x.size()-1;
        vector<int> old(m,0),label(m,-1),diff(m,0);
        long long ans=0,pos=0;
        for(int k=0;k<r;++k) {
            long long left=len[k];
            while(left) {
                if(pos==0 && left>=w) {
                    long long rows=left/w;
                    if(k%2) {
                        ans+=((rows+1)/2)*runs(old,0)+(rows/2)*runs(old,1);
                        if(rows%2)for(int j=0;j<m;++j)old[j]^=1;
                        int id=-1;for(int j=0;j<m;++j){if(old[j]&&(j==0||!old[j-1]))++id;label[j]=old[j]?id:-1;}
                    }
                    left%=w;
                    continue;
                }
                long long take=min(left,w-pos);
                int a=lower_bound(x.begin(),x.end(),pos)-x.begin();
                int b=lower_bound(x.begin(),x.end(),pos+take)-x.begin();
                for(int j=a;j<b;++j)diff[j]=k%2;
                pos+=take;left-=take;
                if(pos==w) {
                    vector<int> cur(m),nl(m,-1);
                    DSU d(2*m+2);int id=m;
                    for(int j=0;j<m;++j) {
                        cur[j]=old[j]^diff[j];
                        if(!cur[j])continue;
                        if(j==0||!cur[j-1]){++id;++ans;}
                        nl[j]=id;
                        if(old[j]&&d.join(id,label[j]))--ans;
                    }
                    vector<int> map(2*m+2,-1);int next=0;
                    for(int j=0;j<m;++j)if(cur[j]){int z=d.root(nl[j]);if(map[z]<0)map[z]=next++;nl[j]=map[z];}
                    old.swap(cur);label.swap(nl);pos=0;
                }
            }
        }
        cout<<ans<<'\n';
    }
}
