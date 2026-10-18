// POJ 2936 - Construct the Wall Maze
// Model: gpt-6-astra
// Enumerate all translations of the path and all legal wall placements.
// A shorter path supplies a necessary constraint: some remaining wall must
// block one of its edges. Branch on those walls, using direct shortcuts first.
// Boundary walls and endpoint contacts are allowed; overlaps and proper
// crossings are forbidden. The given path need not be the unique shortest one.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
struct Wall { int x,y,X,Y; vector<int> edges; };
vector<Wall> all[7], cand[3];
int eid[36][36], adj[36][4], ae[36][4], deg[36], ec;
int lens[3], chosen[3], blocked[60], pos[36], startp, endp, plen;
bool usededge[60];
vector<int> required;
bool conflict(const Wall &a,const Wall &b) {
    bool ah=a.y==a.Y, bh=b.y==b.Y;
    if(ah==bh) {
        if(ah) return a.y==b.y && max(a.x,b.x)<min(a.X,b.X);
        return a.x==b.x && max(a.y,b.y)<min(a.Y,b.Y);
    }
    const Wall &h=ah?a:b, &v=ah?b:a;
    return h.x<v.x && v.x<h.X && v.y<h.y && h.y<v.Y;
}
bool compatible(int k,int w) {
    for(int j=0;j<3;++j) if(chosen[j]>=0 && conflict(cand[k][w],cand[j][chosen[j]])) return false;
    return true;
}
bool search(int depth) {
    vector<int> constraint;
    for(size_t i=0;i<required.size();++i) if(!blocked[required[i]]) {
        constraint.push_back(required[i]); break;
    }
    if(constraint.empty()) {
        int dist[36], par[36], pe[36], q[36], tail=1;
        fill(dist,dist+36,-1); q[0]=startp; dist[startp]=0;
        for(int head=0;head<tail && dist[endp]<0;++head) {
            int u=q[head];
            for(int j=0;j<deg[u];++j) {
                int v=adj[u][j], e=ae[u][j];
                if(!blocked[e] && dist[v]<0) {
                    dist[v]=dist[u]+1; par[v]=u; pe[v]=e; q[tail++]=v;
                }
            }
        }
        if(dist[endp]<plen) {
            if(depth==3) return false;
            for(int u=endp;u!=startp;u=par[u]) constraint.push_back(pe[u]);
        } else {
            if(depth==3) return true;
            // Any compatible extra wall preserves the valid shortest path.
            int k=0; while(chosen[k]>=0) ++k;
            for(size_t w=0;w<cand[k].size();++w) if(compatible(k,w)) {
                chosen[k]=w;
                const vector<int> &es=cand[k][w].edges;
                for(size_t i=0;i<es.size();++i) ++blocked[es[i]];
                if(search(depth+1)) return true;
                for(size_t i=0;i<es.size();++i) --blocked[es[i]];
                chosen[k]=-1;
            }
            return false;
        }
    }
    if(depth==3) return false;
    bool need[60]={false};
    for(size_t i=0;i<constraint.size();++i) need[constraint[i]]=true;
    for(int k=0;k<3;++k) if(chosen[k]<0) {
        bool duplicate=false;
        for(int j=0;j<k;++j) if(chosen[j]<0 && lens[j]==lens[k]) duplicate=true;
        if(duplicate) continue;
        for(size_t w=0;w<cand[k].size();++w) {
            const vector<int> &es=cand[k][w].edges;
            bool hit=false;
            for(size_t i=0;i<es.size();++i) if(need[es[i]]) hit=true;
            if(!hit || !compatible(k,w)) continue;
            chosen[k]=w;
            for(size_t i=0;i<es.size();++i) ++blocked[es[i]];
            if(search(depth+1)) return true;
            for(size_t i=0;i<es.size();++i) --blocked[es[i]];
            chosen[k]=-1;
        }
    }
    return false;
}
int main() {
    memset(eid,-1,sizeof(eid));
    for(int u=0;u<36;++u) for(int v=u+1;v<36;++v)
        if((u/6==v/6 && v==u+1) || v==u+6) {
            eid[u][v]=eid[v][u]=ec;
            adj[u][deg[u]]=v; ae[u][deg[u]++]=ec;
            adj[v][deg[v]]=u; ae[v][deg[v]++]=ec++;
        }
    for(int l=1;l<=6;++l) for(int rot=0;rot<2;++rot)
        for(int y=0;y<=6;++y) for(int x=0;x+l<=6;++x) {
            Wall w; w.x=rot?y:x; w.y=rot?x:y;
            w.X=rot?y:x+l; w.Y=rot?x+l:y;
            if(y>0 && y<6) for(int z=x;z<x+l;++z) {
                int u=rot?z*6+y-1:(y-1)*6+z;
                int v=rot?u+1:u+6;
                w.edges.push_back(eid[u][v]);
            }
            all[l].push_back(w);
        }
    char s[100];
    while(scanf("%d%d%d",&lens[0],&lens[1],&lens[2])==3 && lens[0]) {
        scanf("%s",s); plen=strlen(s); bool solved=false;
        for(int sy=0;sy<6 && !solved;++sy) for(int sx=0;sx<6 && !solved;++sx) {
            int x=sx,y=sy; bool valid=true;
            memset(usededge,0,sizeof(usededge)); fill(pos,pos+36,-1);
            startp=y*6+x; pos[startp]=0;
            for(int i=0;i<plen;++i) {
                int prev=y*6+x;
                if(s[i]=='N') --y; if(s[i]=='S') ++y;
                if(s[i]=='E') ++x; if(s[i]=='W') --x;
                if(x<0 || x>=6 || y<0 || y>=6 || pos[y*6+x]>=0) {valid=false;break;}
                pos[y*6+x]=i+1; usededge[eid[prev][y*6+x]]=true;
            }
            if(!valid) continue;
            endp=y*6+x; required.clear();
            for(int u=0;u<36;++u) if(pos[u]>=0)
                for(int j=0;j<deg[u];++j) {
                    int v=adj[u][j];
                    if(pos[v]>pos[u]+1) required.push_back(ae[u][j]);
                }
            for(int k=0;k<3;++k) {
                cand[k].clear();
                for(size_t w=0;w<all[lens[k]].size();++w) {
                    const Wall &a=all[lens[k]][w]; bool ok=true;
                    for(size_t j=0;j<a.edges.size();++j) if(usededge[a.edges[j]]) ok=false;
                    if(ok) cand[k].push_back(a);
                }
            }
            memset(blocked,0,sizeof(blocked)); fill(chosen,chosen+3,-1);
            if(search(0)) {
                printf("%d %d\n%d %d\n",sx+1,sy+1,x+1,y+1);
                for(int k=0;k<3;++k) {
                    const Wall &w=cand[k][chosen[k]];
                    printf("%d %d %d %d\n",w.x,w.y,w.X,w.Y);
                }
                solved=true;
            }
        }
        if(!solved) return 1;
    }
    return 0;
}
