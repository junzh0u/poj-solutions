// POJ 2962 - Who owns the Amiga?
// Model: gpt-6.1-sol
// Room domains, binary arc consistency, and Hall-set propagation for each
// category prune a complete backtracking search. After finding one owner,
// forbid that ownership and search again to establish whether it is unique.
// left-of and right-of mean immediately adjacent rooms, as stated.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;
struct Arc { int a,b; Arc(int x,int y):a(x),b(y){} };
int allow[30][30][5], bits[32];
vector<Arc> arcs;
void constrain(int a,int b,int type) {
    for(int x=0;x<5;++x) for(int y=0;y<5;++y) {
        bool ok=type==0 ? x==y : type==1 ? x+1==y : type==2 ? x==y+1 : type==3 ? abs(x-y)==1 : x!=y;
        if(!ok) { allow[a][b][x]&=~(1<<y); allow[b][a][y]&=~(1<<x); }
    }
}
bool propagate(int *d) {
    bool changed=true;
    while(changed) {
        changed=false;
        for(unsigned k=0;k<arcs.size();++k) {
            int a=arcs[k].a,b=arcs[k].b,nd=0;
            for(int x=0;x<5;++x) if((d[a]&(1<<x)) && (allow[a][b][x]&d[b])) nd|=1<<x;
            if(!nd) return false;
            if(nd!=d[a]) { d[a]=nd; changed=true; }
        }
        for(int base=5;base<30;base+=5) for(int s=1;s<32;++s) {
            int u=0;
            for(int i=0;i<5;++i) if(s&(1<<i)) u|=d[base+i];
            if(bits[u]<bits[s]) return false;
            if(bits[u]==bits[s]) for(int i=0;i<5;++i) if(!(s&(1<<i))) {
                int nd=d[base+i]&~u;
                if(!nd) return false;
                if(nd!=d[base+i]) { d[base+i]=nd; changed=true; }
            }
        }
    }
    return true;
}
bool search(int *d) {
    if(!propagate(d)) return false;
    int v=-1;
    for(int i=5;i<30;++i) if(bits[d[i]]>1 && (v<0 || bits[d[i]]<bits[d[v]])) v=i;
    if(v<0) return true;
    for(int x=0;x<5;++x) if(d[v]&(1<<x)) {
        int c[30]; memcpy(c,d,sizeof(c)); c[v]=1<<x;
        if(search(c)) { memcpy(d,c,sizeof(c)); return true; }
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    const char *names[]={"1","2","3","4","5","blue","green","red","white","yellow","anna","bernhard","chris","david","ellen","danish","finnish","icelandic","norwegian","swedish","amiga","atari","linux","mac","windows","c","c++","java","pascal","perl"};
    map<string,int> id;
    for(int i=0;i<30;++i) id[names[i]]=i;
    for(int i=1;i<32;++i) bits[i]=bits[i>>1]+(i&1);
    int tests; cin>>tests;
    for(int tc=1;tc<=tests;++tc) {
        for(int i=0;i<30;++i) for(int j=0;j<30;++j) for(int x=0;x<5;++x) allow[i][j][x]=31;
        for(int b=0;b<30;b+=5) for(int i=b;i<b+5;++i) for(int j=i+1;j<b+5;++j) constrain(i,j,4);
        int n; cin>>n;
        while(n--) {
            string a,r,b; cin>>a>>r>>b;
            constrain(id[a],id[b],r=="same-as"?0:r=="left-of"?1:r=="right-of"?2:3);
        }
        arcs.clear();
        for(int i=0;i<30;++i) for(int j=0;j<30;++j) {
            bool used=false;
            for(int x=0;x<5;++x) if(allow[i][j][x]!=31) used=true;
            if(used) arcs.push_back(Arc(i,j));
        }
        int initial[30],d[30];
        for(int i=0;i<30;++i) initial[i]=i<5 ? 1<<i : 31;
        memcpy(d,initial,sizeof(d)); search(d);
        int owner=10;
        while(d[owner]!=d[20]) ++owner;
        constrain(owner,20,4);
        arcs.push_back(Arc(owner,20)); arcs.push_back(Arc(20,owner));
        bool other=search(initial);
        cout<<"scenario #"<<tc<<": ";
        if(other) cout<<"cannot identify the amiga owner.\n";
        else cout<<names[owner]<<" owns the amiga.\n";
    }
    return 0;
}
