// POJ 3396 - A Scheduling Problem
// Model: gpt-6.1-sol
// Binary search the number of days. Tree DP records every feasible day
// for each subtree root; child extrema suffice for strict precedence,
// while a conflict only excludes a child's sole feasible day.
// The suffix d means parent before child; u means child before parent.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;
struct Edge { int v, type; Edge(int a,int b):v(a),type(b){} };
vector< vector<Edge> > g;
vector< vector<char> > dp;
int days;
int vertex(int label, map<int,int>& ids) {
    if(ids.find(label)!=ids.end()) return ids[label];
    int v=(int)g.size(); ids[label]=v; g.push_back(vector<Edge>()); return v;
}
void dfs(int u,int p) {
    dp[u].assign(days+1,1); dp[u][0]=0;
    for(size_t i=0;i<g[u].size();++i) {
        Edge e=g[u][i]; if(e.v==p) continue;
        dfs(e.v,u);
        int lo=days+1,hi=0,count=0;
        for(int d=1;d<=days;++d) if(dp[e.v][d]) {
            if(lo> d) lo=d; hi=d; ++count;
        }
        for(int d=1;d<=days;++d) {
            bool ok;
            if(e.type==1) ok=hi>d;
            else if(e.type==-1) ok=lo<d;
            else ok=count>1 || (count==1 && lo!=d);
            if(!ok) dp[u][d]=0;
        }
    }
}
bool feasible(int d) {
    days=d; dp.assign(g.size(),vector<char>()); dfs(0,-1);
    for(int t=1;t<=days;++t) if(dp[0][t]) return true;
    return false;
}
void solve() {
    int l=1,r=(int)g.size();
    while(l<r) { int m=(l+r)/2; if(feasible(m)) r=m; else l=m+1; }
    cout<<l<<'\n';
}
int main() {
    string line; map<int,int> ids;
    while(getline(cin,line)) {
        istringstream in(line); int label;
        if(!(in>>label)) continue;
        if(label==0) {
            if(g.empty()) break;
            solve(); g.clear(); ids.clear(); continue;
        }
        int u=vertex(label,ids); string token;
        while(in>>token && token!="0") {
            int v=vertex(atoi(token.c_str()),ids),type=0;
            char last=token[token.size()-1];
            if(last=='d') type=1; if(last=='u') type=-1;
            g[u].push_back(Edge(v,type)); g[v].push_back(Edge(u,-type));
        }
    }
    return 0;
}
