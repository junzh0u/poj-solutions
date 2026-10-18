// POJ 2647 - Doublets
// Model: gpt-6-astra
// Sort one-wildcard patterns to group equal-length words differing at one
// position. BFS expands each group once and reconstructs a shortest ladder.
// Doublets replace one letter; insertions and deletions are not allowed.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
using namespace std;
struct Pattern {
    char s[17];
    int word;
    bool operator<(const Pattern& b) const { return strcmp(s,b.s)<0; }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    vector<string> words;
    map<string,int> index;
    string s;
    while (getline(cin,s)) {
        if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        if (s.empty()) break;
        if (index.find(s)==index.end()) {
            index[s]=(int)words.size(); words.push_back(s);
        }
    }
    int n=(int)words.size();
    vector<Pattern> patterns;
    for(int i=0;i<n;++i) for(size_t j=0;j<words[i].size();++j) {
        Pattern p;
        strcpy(p.s,words[i].c_str()); p.s[j]='*'; p.word=i;
        patterns.push_back(p);
    }
    sort(patterns.begin(),patterns.end());
    vector<int> start;
    vector< vector<int> > groups(n);
    for(int i=0;i<(int)patterns.size();++i) {
        if(i==0 || strcmp(patterns[i-1].s,patterns[i].s)!=0) start.push_back(i);
        groups[patterns[i].word].push_back((int)start.size()-1);
    }
    int g=(int)start.size(); start.push_back((int)patterns.size());
    string a,b;
    bool first=true;
    while(cin>>a>>b) {
        if(!first) cout<<'\n'; first=false;
        map<string,int>::iterator ia=index.find(a),ib=index.find(b);
        if(ia==index.end() || ib==index.end() || a.size()!=b.size()) {
            cout<<"No solution.\n"; continue;
        }
        int source=ia->second,target=ib->second;
        vector<int> parent(n,-1),queue;
        vector<char> used(g,0);
        parent[source]=source; queue.push_back(source);
        for(size_t head=0;head<queue.size() && parent[target]<0;++head) {
            int u=queue[head];
            for(size_t j=0;j<groups[u].size();++j) {
                int k=groups[u][j];
                if(used[k]) continue;
                used[k]=1;
                for(int t=start[k];t<start[k+1];++t) {
                    int v=patterns[t].word;
                    if(parent[v]<0) { parent[v]=u; queue.push_back(v); }
                }
            }
        }
        if(parent[target]<0) { cout<<"No solution.\n"; continue; }
        vector<int> path;
        for(int v=target;;v=parent[v]) { path.push_back(v); if(v==source) break; }
        for(int i=(int)path.size()-1;i>=0;--i) cout<<words[path[i]]<<'\n';
    }
    return 0;
}
