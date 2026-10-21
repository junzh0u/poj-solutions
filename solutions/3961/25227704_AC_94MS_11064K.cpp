// POJ 3961 - Cactus Revolution
// Model: gpt-6-astra
// Root the cactus into bridge and cycle blocks. Each processed subtree leaves
// its uniquely sized connected remainder at its attachment vertex. On a cycle,
// prefix sums with one residue modulo the district size identify all full
// districts between the two endpoint remainders. Removed vertices split cycles
// into paths. Linked lists assemble districts without repeatedly copying them.
// Input contains one cactus; every printed district is sorted independently.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int N=50005;
vector<int> adj[N], cycles[N];
vector< vector<int> > blocks, answer;
int par[N],dep[N],it[N],inCycle[N],sz[N],head[N],tail[N],nxt[N];
int n,m,k,S; bool ok=true;
void join(int a,int b){
    if(!sz[b])return;
    if(!sz[a])head[a]=head[b]; else nxt[tail[a]]=head[b];
    tail[a]=tail[b];sz[a]+=sz[b];
}
void finish(int a){
    vector<int> v; for(int x=head[a];x;x=nxt[x])v.push_back(x);
    sort(v.begin(),v.end());answer.push_back(v);sz[a]=head[a]=tail[a]=0;
}
// Process a path from its detached end towards its attachment.
void path(const vector<int>&v,int l,int r,int step,int root,bool attached){
    int acc=0;sz[0]=head[0]=tail[0]=0;
    for(int j=l;j!=r;j+=step){join(acc,v[j]);if(sz[acc]>S){ok=false;return;}if(sz[acc]==S)finish(acc);}
    if(attached)join(root,acc);else if(sz[acc])ok=false;
}
void cycle(const vector<int>&v,int root){
    int len=(int)v.size(),first=-1,last=-1;
    for(int j=0;j<len;j++)if(sz[v[j]]==0){if(first<0)first=j;last=j;}
    if(first>=0){
        path(v,first-1,-1,-1,root,true);
        path(v,last+1,len,1,root,true);
        int start=first+1;
        for(int j=first+1;j<=last;j++)if(!sz[v[j]]){path(v,start,j,1,root,false);start=j+1;}
        return;
    }
    vector<int> p(len+1,0);
    for(int j=0;j<len;j++)p[j+1]=p[j]+sz[v[j]];
    int total=p[len],q=total/S,rem=total%S;
    if(!q){for(int j=0;j<len;j++)join(root,v[j]);return;}
    // Sorting pairs avoids an O(S) array initialization for every cycle.
    vector< pair<int,int> > residues;
    for(int j=0;j<=len;j++)residues.push_back(make_pair(p[j]%S,j));
    sort(residues.begin(),residues.end());
    int begin=-1,end=-1;
    for(int a=0;a<=len;){
        int b=a+1;while(b<=len&&residues[b].first==residues[a].first)b++;
        if(b-a==q+1 && p[residues[a].second]<=rem){begin=residues[a].second;end=residues[b-1].second;break;}
        a=b;
    }
    if(begin<0){ok=false;return;}
    for(int j=0;j<begin;j++)join(root,v[j]);
    sz[0]=head[0]=tail[0]=0;
    for(int j=begin;j<end;j++){join(0,v[j]);if(sz[0]==S)finish(0);}
    for(int j=end;j<len;j++)join(root,v[j]);
}
int main(){
    if(scanf("%d%d%d",&n,&m,&k)!=3)return 0;S=n/k;
    for(int i=0;i<m;i++){int s,a,b;scanf("%d%d",&s,&a);for(int j=1;j<s;j++){scanf("%d",&b);adj[a].push_back(b);adj[b].push_back(a);a=b;}}
    vector<int> st,order;st.push_back(1);dep[1]=1;
    while(!st.empty()){
        int u=st.back();
        if(it[u]==0)order.push_back(u);
        if(it[u]==(int)adj[u].size()){st.pop_back();continue;}
        int v=adj[u][it[u]++];if(v==par[u])continue;
        if(!dep[v]){par[v]=u;dep[v]=dep[u]+1;st.push_back(v);}
        else if(dep[v]<dep[u]){
            vector<int> c;for(int x=u;x!=v;x=par[x]){c.push_back(x);inCycle[x]=1;}
            cycles[v].push_back((int)blocks.size());blocks.push_back(c);
        }
    }
    for(int z=n-1;z>=0&&ok;z--){
        int u=order[z];sz[u]=1;head[u]=tail[u]=u;
        for(int j=0;j<(int)adj[u].size();j++){int v=adj[u][j];if(par[v]==u&&!inCycle[v])join(u,v);}
        for(int j=0;j<(int)cycles[u].size()&&ok;j++)cycle(blocks[cycles[u][j]],u);
        if(sz[u]>S)ok=false;else if(sz[u]==S)finish(u);
    }
    if(!ok||sz[1]||(int)answer.size()!=k){puts("-1");return 0;}
    for(int i=0;i<k;i++){for(int j=0;j<S;j++)printf("%d%c",answer[i][j],j+1==S?'\n':' ');}
    return 0;
}
