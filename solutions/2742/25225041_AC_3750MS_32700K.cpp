// POJ 2742 - Organize Your Train
// Model: gpt-6.1-sol
// Bidirectional breadth-first search to depth three from each configuration.
// A move is reversible. Equal endpoint directions reverse the moved substring;
// opposite endpoint directions preserve its west-to-east order.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
struct Edge { int a,b; char p,q; };
int n; vector<Edge> edges;
string pack(const vector<string>& a) {
    string s=a[0]; for(int i=1;i<n;++i) s+='|',s+=a[i]; return s;
}
vector<string> unpack(const string& s) {
    vector<string> a(n); int j=0;
    for(size_t i=0;i<s.size();++i) if(s[i]=='|') ++j; else a[j]+=s[i];
    return a;
}
void search(const string& start, map<string,int>& dist) {
    vector<string> queue; queue.push_back(start); dist[start]=0;
    for(size_t h=0;h<queue.size();++h) {
        string key=queue[h]; int d=dist[key]; if(d==3) continue;
        vector<string> a=unpack(key);
        for(size_t e=0;e<edges.size();++e) {
            Edge z=edges[e]; int len=a[z.a].size();
            for(int k=1;k<=len;++k) {
                vector<string> b=a; string part;
                if(z.p=='W') { part=b[z.a].substr(0,k); b[z.a].erase(0,k); }
                else { part=b[z.a].substr(len-k); b[z.a].erase(len-k); }
                if(z.p==z.q) reverse(part.begin(),part.end());
                if(z.q=='W') b[z.b]=part+b[z.b]; else b[z.b]+=part;
                string next=pack(b);
                if(dist.insert(make_pair(next,d+1)).second) queue.push_back(next);
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false); int m;
    while(cin>>n>>m && n) {
        edges.clear();
        for(int i=0;i<m;++i) { Edge z; cin>>z.a>>z.p>>z.b>>z.q; edges.push_back(z); swap(z.a,z.b); swap(z.p,z.q); edges.push_back(z); }
        vector<string> s(n),t(n);
        for(int i=0;i<n;++i) {cin>>s[i]; if(s[i]=="-")s[i]="";}
        for(int i=0;i<n;++i) {cin>>t[i]; if(t[i]=="-")t[i]="";}
        map<string,int> a,b; search(pack(s),a); search(pack(t),b);
        int answer=7;
        for(map<string,int>::iterator it=a.begin();it!=a.end();++it) {
            map<string,int>::iterator jt=b.find(it->first);
            if(jt!=b.end()) answer=min(answer,it->second+jt->second);
        }
        cout<<answer<<'\n';
    }
}
