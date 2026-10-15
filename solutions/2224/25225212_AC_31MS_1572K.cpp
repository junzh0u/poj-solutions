// POJ 2224 - Missing Piece 2001
// Model: gpt-6.1-sol
// Breadth-first searches from both layouts enumerate balls of radii
// floor(N/2) and ceil(N/2). Their minimum intersection distance is optimal.
// The goal may be arbitrary; identical layouts also solve the N=0 case.
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <cstdlib>
using namespace std;
typedef map<string,int> Dist;
void explore(const string &start, int d, int limit, Dist &dist) {
    queue<string> q;
    dist[start]=0; q.push(start);
    while (!q.empty()) {
        string s=q.front(); q.pop();
        int depth=dist.find(s)->second;
        if (depth==limit) continue;
        int z=(int)s.find(char(0));
        int r=z/d, c=z%d;
        const int dr[4]={-1,1,0,0}, dc[4]={0,0,-1,1};
        for(int k=0;k<4;++k) {
            int nr=r+dr[k], nc=c+dc[k];
            if(nr<0 || nr>=d || nc<0 || nc>=d) continue;
            int p=nr*d+nc;
            swap(s[z],s[p]);
            if(dist.insert(make_pair(s,depth+1)).second) q.push(s);
            swap(s[z],s[p]);
        }
    }
}
int main() {
    string word; int d,n; bool first=true;
    while(cin>>word>>d>>n) {
        string a,b;
        for(int t=0;t<2;++t) for(int i=0;i<d*d;++i) {
            cin>>word;
            char value=word=="X"?0:(char)atoi(word.c_str());
            (t==0?a:b).push_back(value);
        }
        cin>>word;
        Dist left,right;
        explore(a,d,n/2,left); explore(b,d,n-n/2,right);
        int answer=n+1;
        for(Dist::const_iterator it=left.begin();it!=left.end();++it) {
            Dist::const_iterator jt=right.find(it->first);
            if(jt!=right.end()) answer=min(answer,it->second+jt->second);
        }
        if(!first) cout<<'\n'; first=false;
        if(answer<=n) cout<<"SOLVABLE WITHIN "<<answer<<" MOVES\n";
        else cout<<"NOT SOLVABLE WITHIN "<<n<<" MOVES\n";
    }
    return 0;
}
