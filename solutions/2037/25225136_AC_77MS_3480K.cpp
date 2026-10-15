// POJ 2037 - Roll Playing Games
// Model: gpt-6.1-sol
// Convolve the known dice, saturating counts above every requested count.
// Meet in the middle on sorted halves of the final die. Sort suffixes by
// count vector and faces, then find the least compatible suffix for each
// lexicographically ordered prefix. Counts are exact, not lower bounds.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int m,r,plen;
long long target[10], contribution[51][10];
struct Record {
    long long v[10];
    int face[3];
};
bool operator<(const Record &a,const Record &b) {
    for(int j=0;j<m;++j) if(a.v[j]!=b.v[j]) return a.v[j]<b.v[j];
    for(int j=0;j<3;++j) if(a.face[j]!=b.face[j]) return a.face[j]<b.face[j];
    return false;
}
vector<Record> suffixes;
int prefix[3],answer[6];
bool searchPrefix(int pos,int low,long long sums[10]) {
    if(pos==plen) {
        Record key;
        for(int j=0;j<m;++j) key.v[j]=target[j]-sums[j];
        for(int k=0;k<3;++k) key.face[k]=low;
        vector<Record>::iterator it=lower_bound(suffixes.begin(),suffixes.end(),key);
        if(it==suffixes.end()) return false;
        for(int j=0;j<m;++j) if(it->v[j]!=key.v[j]) return false;
        for(int k=0;k<plen;++k) answer[k]=prefix[k];
        for(int k=0;k<3;++k) answer[plen+k]=it->face[k];
        return true;
    }
    for(int x=low;x<=50;++x) {
        long long next[10]; bool ok=true;
        for(int j=0;j<m;++j) {
            next[j]=sums[j]+contribution[x][j];
            if(next[j]>target[j]) ok=false;
        }
        if(ok) { prefix[pos]=x; if(searchPrefix(pos+1,x,next)) return true; }
    }
    return false;
}
int main() {
    int n;
    while(cin>>n && n) {
        vector<vector<int> > dice(n);
        for(int i=0;i<n;++i) {
            int f; cin>>f; dice[i].resize(f);
            for(int k=0;k<f;++k) cin>>dice[i][k];
        }
        cin>>r>>m;
        int value[10]; long long cap=1;
        for(int j=0;j<m;++j) { cin>>value[j]>>target[j]; cap=max(cap,target[j]+1); }
        long long dp[1001]={0}; dp[0]=1;
        int maxsum=0;
        for(int i=0;i<n;++i) {
            long long next[1001]={0};
            for(int s=0;s<=maxsum;++s) for(unsigned k=0;k<dice[i].size();++k) {
                int t=s+dice[i][k];
                next[t]=min(cap,next[t]+dp[s]);
            }
            maxsum+=*max_element(dice[i].begin(),dice[i].end());
            for(int s=0;s<=maxsum;++s) dp[s]=next[s];
        }
        for(int x=1;x<=50;++x) for(int j=0;j<m;++j) {
            int s=value[j]-x;
            contribution[x][j]=(s>=0 && s<=maxsum)?dp[s]:0;
        }
        suffixes.clear();
        for(int a=1;a<=50;++a) for(int b=a;b<=50;++b) for(int c=b;c<=50;++c) {
            Record rec; bool ok=true;
            rec.face[0]=a; rec.face[1]=b; rec.face[2]=c;
            for(int j=0;j<m;++j) {
                rec.v[j]=contribution[a][j]+contribution[b][j]+contribution[c][j];
                if(rec.v[j]>target[j]) ok=false;
            }
            if(ok) suffixes.push_back(rec);
        }
        sort(suffixes.begin(),suffixes.end());
        plen=r-3; long long zero[10]={0};
        if(searchPrefix(0,1,zero)) {
            cout<<"Final die face values are";
            for(int k=0;k<r;++k) cout<<' '<<answer[k];
            cout<<'\n';
        } else cout<<"Impossible\n";
    }
}
