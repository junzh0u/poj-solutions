// POJ 1227 - RoboContest
// Model: claude-sonnet-5
// Approach: Map region ids (arbitrary non-negative integers, not necessarily
// 1..n) to a dense 0..n-1 index. Robots must move along a directed edge of
// the map every tick. All robots can meet at some future tick iff, taking
// any one robot's start as reference, every other robot's start is reachable
// from it by a walk of *even* length in the state graph (node, parity):
// - if the connected component is bipartite, that means "same side" of the
//   bipartition as the reference (an even-length walk exists exactly between
//   same-side vertices);
// - if the component contains an odd cycle, any two vertices of the
//   component admit walks of every sufficiently large length (the odd cycle
//   lets you fix parity), so every vertex reachable at all is reachable via
//   an even walk.
// This is computed with a single BFS over the doubled state graph
// (node,parity) rooted at (start,0); a robot's start qualifies iff its
// (node,0) state was reached. k=1 needs no other check and passes trivially
// since the loop over the remaining robots is empty.
// Ambiguity: region ids are not guaranteed to be 1..n (confirmed on the
// discuss board), so a map<int,int> canonicalizes them before building
// adjacency; edges are stored exactly as listed (not forced symmetric),
// matching known-accepted solutions.
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include <utility>
using namespace std;

int main(){
    int t;
    if(scanf("%d",&t)!=1) return 0;
    while(t--){
        int n,k;
        scanf("%d %d",&n,&k);
        map<int,int> id2idx;
        vector<vector<int> > adj;
        adj.reserve(n+5);
        for(int i=0;i<n;i++){
            int x,m;
            scanf("%d %d",&x,&m);
            map<int,int>::iterator it=id2idx.find(x);
            int xi;
            if(it==id2idx.end()){
                xi=(int)id2idx.size();
                id2idx[x]=xi;
            } else xi=it->second;
            if(xi>=(int)adj.size()) adj.resize(xi+1);
            for(int j=0;j<m;j++){
                int y;
                scanf("%d",&y);
                map<int,int>::iterator it2=id2idx.find(y);
                int yi;
                if(it2==id2idx.end()){
                    yi=(int)id2idx.size();
                    id2idx[y]=yi;
                } else yi=it2->second;
                if(yi>=(int)adj.size()) adj.resize(yi+1);
                adj[xi].push_back(yi);
            }
        }
        vector<int> robots(k);
        for(int i=0;i<k;i++){
            int r;
            scanf("%d",&r);
            map<int,int>::iterator it=id2idx.find(r);
            int ri;
            if(it==id2idx.end()){
                ri=(int)id2idx.size();
                id2idx[r]=ri;
            } else ri=it->second;
            if(ri>=(int)adj.size()) adj.resize(ri+1);
            robots[i]=ri;
        }
        int sz=(int)adj.size();
        vector<vector<char> > reach(sz, vector<char>(2,0));
        queue<pair<int,int> > q;
        int start=robots[0];
        reach[start][0]=1;
        q.push(make_pair(start,0));
        while(!q.empty()){
            pair<int,int> cur=q.front(); q.pop();
            int u=cur.first, p=cur.second;
            for(size_t idx=0; idx<adj[u].size(); idx++){
                int v=adj[u][idx];
                int np=1-p;
                if(!reach[v][np]){
                    reach[v][np]=1;
                    q.push(make_pair(v,np));
                }
            }
        }
        bool ok=true;
        for(int i=0;i<k;i++){
            if(!reach[robots[i]][0]){ ok=false; break; }
        }
        printf(ok? "YES\n" : "NO\n");
    }
    return 0;
}
