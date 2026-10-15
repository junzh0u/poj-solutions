// POJ 2324 - Intellectual Property
// Model: gpt-6.1-sol
// Build a suffix array and LCP array for both codebases. Two scans find
// the longest TDP match at every JCN position. Discard intervals contained
// in an earlier interval, then order by decreasing length and position.
// Newlines belong to each codebase; overlapping maximal segments are kept.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Segment { int p,l; Segment(int P,int L):p(P),l(L){} };
bool order(const Segment&a,const Segment&b) { return a.l!=b.l?a.l>b.l:a.p<b.p; }
vector<int> suffixArray(const vector<int>&s) {
    int n=s.size(), m=258;
    vector<int> sa(n),r(s),tmp(n),y(n),c(max(n,258),0);
    for(int i=0;i<n;++i) ++c[r[i]];
    for(int i=1;i<m;++i) c[i]+=c[i-1];
    for(int i=n-1;i>=0;--i) sa[--c[r[i]]]=i;
    for(int k=1;k<n;k*=2) {
        int p=0;
        for(int i=n-k;i<n;++i) y[p++]=i;
        for(int i=0;i<n;++i) if(sa[i]>=k) y[p++]=sa[i]-k;
        fill(c.begin(),c.begin()+m,0);
        for(int i=0;i<n;++i) ++c[r[y[i]]];
        for(int i=1;i<m;++i) c[i]+=c[i-1];
        for(int i=n-1;i>=0;--i) sa[--c[r[y[i]]]]=y[i];
        p=1; tmp[sa[0]]=0;
        for(int i=1;i<n;++i) {
            int a=sa[i-1],b=sa[i];
            if(r[a]!=r[b] || (a+k<n?r[a+k]:-1)!=(b+k<n?r[b+k]:-1)) ++p;
            tmp[b]=p-1;
        }
        r.swap(tmp); m=p; if(p==n) break;
    }
    return sa;
}
vector<Segment> solve(const string&a,const string&b) {
    vector<int>s;
    for(size_t i=0;i<a.size();++i) s.push_back((unsigned char)a[i]+2);
    s.push_back(1); int start=s.size();
    for(size_t i=0;i<b.size();++i) s.push_back((unsigned char)b[i]+2);
    s.push_back(0);
    int n=s.size(); vector<int>sa=suffixArray(s),rank(n),h(n,0),best(b.size(),0);
    for(int i=0;i<n;++i) rank[sa[i]]=i;
    int len=0;
    for(int i=0;i<n;++i) {
        int at=rank[i]; if(!at) {len=0;continue;}
        int j=sa[at-1];
        while(i+len<n&&j+len<n&&s[i+len]==s[j+len]) ++len;
        h[at]=len; if(len) --len;
    }
    int v=0;
    for(int i=0;i<n;++i) {
        v=min(v,h[i]);
        if(sa[i]<(int)a.size()) v=n;
        else if(sa[i]>=start&&sa[i]<n-1) best[sa[i]-start]=v;
    }
    v=0;
    for(int i=n-1;i>=0;--i) {
        if(i+1<n) v=min(v,h[i+1]);
        if(sa[i]<(int)a.size()) v=n;
        else if(sa[i]>=start&&sa[i]<n-1) best[sa[i]-start]=max(best[sa[i]-start],v);
    }
    vector<Segment> out; int end=0;
    for(int i=0;i<(int)b.size();++i) if(best[i]&&i+best[i]>end) {
        out.push_back(Segment(i,best[i])); end=i+best[i];
    }
    sort(out.begin(),out.end(),order); return out;
}
string readBase(const string&ending) {
    string s,line; getline(cin,line);
    while(getline(cin,line)) {
        if(!line.empty()&&line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line==ending) break;
        s+=line; s+='\n';
    }
    return s;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int k,tc=0; string line;
    while(cin>>k&&k) {
        getline(cin,line);
        string a=readBase("END TDP CODEBASE"),b=readBase("END JCN CODEBASE");
        vector<Segment> ans=solve(a,b);
        if(tc) cout<<'\n';
        cout<<"CASE "<<++tc<<'\n';
        for(int i=0;i<k&&i<(int)ans.size();++i) {
            cout<<"INFRINGING SEGMENT "<<i+1<<" LENGTH "<<ans[i].l<<" POSITION "<<ans[i].p<<'\n';
            cout<<b.substr(ans[i].p,ans[i].l)<<'\n';
        }
    }
}
