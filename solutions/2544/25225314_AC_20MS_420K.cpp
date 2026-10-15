// POJ 2544 - PreQueL
// Model: gpt-6.1-sol
// Merge equality classes, filter rows, and backtrack with bitset domains.
// Choose the smallest remaining table and propagate its shared column values.
// Check each connected component for feasibility before enumerating the join.
// Duplicate input rows retain their multiplicity; missing conditions end at EOF.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
using namespace std;
const int N=1000;
int par[260], cols[26], nr[26], off[26], root[26][9], T, total;
vector<string> tab[26];
bitset<N> masks[26][9][26];
struct Link { int a,t,b; Link(int x,int y,int z):a(x),t(y),b(z){} };
vector<Link> links[26];
struct State { bitset<N> d[26]; };
int chosen[26];
vector<string> result;
int findroot(int x){return par[x]==x?x:par[x]=findroot(par[x]);}
void unite(int a,int b){par[findroot(a)]=findroot(b);}
bool dfs(const State &s, unsigned int left, bool first){
    if(!left){
        if(first) return true;
        string row;
        for(int t=0;t<T;++t) row+=tab[t][chosen[t]];
        result.push_back(row);
        return true;
    }
    int t=-1, best=N+1;
    for(int i=0;i<T;++i) if(left&(1u<<i)){
        int n=(int)s.d[i].count();
        if(!n) return false;
        if(n<best){best=n;t=i;}
    }
    unsigned int next=left^(1u<<t);
    bool found=false;
    for(int r=0;r<nr[t];++r) if(s.d[t][r]){
        State ns=s;
        bool ok=true;
        for(size_t j=0;j<links[t].size();++j){
            Link e=links[t][j];
            if(next&(1u<<e.t)){
                ns.d[e.t]&=masks[e.t][e.b][tab[t][r][e.a]-'A'];
                if(ns.d[e.t].none()){ok=false;break;}
            }
        }
        if(ok){
            chosen[t]=r;
            if(dfs(ns,next,first)){found=true;if(first)return true;}
        }
    }
    return found;
}
int main(){
    ios::sync_with_stdio(false);cin.tie(0);
    int D;if(!(cin>>T>>D))return 0;
    for(int i=0;i<260;++i)par[i]=i;
    for(int t=0;t<T;++t){
        cin>>nr[t]>>cols[t];off[t]=total;total+=cols[t];
        for(int r=0;r<nr[t];++r){string x;cin>>x;tab[t].push_back(x);}
    }
    for(int i=0;i<D;++i){
        string q;if(!(cin>>q))break;
        int a=(q[0]-'a')*9+q[1]-'1';
        if(q[3]>='A'&&q[3]<='Z')unite(a,234+q[3]-'A');
        else unite(a,(q[3]-'a')*9+q[4]-'1');
    }
    int fixed[260];fill(fixed,fixed+260,-1);
    for(int v=0;v<26;++v){
        int k=findroot(234+v);
        if(fixed[k]!=-1&&fixed[k]!=v)return 0;
        fixed[k]=v;
    }
    State s;
    for(int t=0;t<T;++t){
        for(int c=0;c<cols[t];++c)root[t][c]=findroot(t*9+c);
        for(int r=0;r<nr[t];++r){
            bool ok=true;
            for(int c=0;c<cols[t];++c){
                int v=tab[t][r][c]-'A', k=root[t][c];
                if(fixed[k]!=-1&&fixed[k]!=v)ok=false;
                for(int b=0;b<c;++b)if(root[t][b]==k&&tab[t][r][b]!=tab[t][r][c])ok=false;
                masks[t][c][v].set(r);
            }
            if(ok)s.d[t].set(r);
        }
        if(s.d[t].none())return 0;
    }
    for(int t=0;t<T;++t)for(int u=0;u<T;++u)if(t!=u)
        for(int a=0;a<cols[t];++a)for(int b=0;b<cols[u];++b)
            if(root[t][a]==root[u][b])links[t].push_back(Link(a,u,b));
    unsigned int all=(1u<<T)-1, unseen=all;
    while(unseen){
        int start=0;while(!(unseen&(1u<<start)))++start;
        unsigned int comp=1u<<start, old=0;
        while(old!=comp){
            old=comp;
            for(int t=0;t<T;++t)if(comp&(1u<<t))
                for(size_t j=0;j<links[t].size();++j)comp|=1u<<links[t][j].t;
        }
        if(!dfs(s,comp,true))return 0;
        unseen&=~comp;
    }
    dfs(s,all,false);
    sort(result.begin(),result.end());
    for(size_t i=0;i<result.size();++i)cout<<result[i]<<'\n';
    return 0;
}
