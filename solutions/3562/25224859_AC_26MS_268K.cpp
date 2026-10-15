// POJ 3562 - "Roman" corridor
// Model: gpt-6.1-sol
// Try canonical Roman numerals in increasing numeric order. For each prefix,
// retain every tile where a walk spelling it can end. The statement imposes
// no restriction on revisiting tiles; moves are only orthogonal adjacencies.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int n,m;
string grid[15];
int adj[225][4], deg[225];
string roman(int x) {
    const int val[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};
    const char *sym[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    string s;
    for(int i=0;i<13;++i) while(x>=val[i]) {s+=sym[i];x-=val[i];}
    return s;
}
bool possible(const string &s) {
    bool a[225],b[225];
    memset(a,0,sizeof(a));
    for(int r=0;r<n;++r) a[r*m]=(grid[r][0]==s[0]);
    for(int k=1;k<(int)s.size();++k) {
        memset(b,0,sizeof(b));
        bool any=false;
        for(int u=0;u<n*m;++u) if(a[u])
            for(int j=0;j<deg[u];++j) {
                int v=adj[u][j];
                if(grid[v/m][v%m]==s[k]) b[v]=any=true;
            }
        if(!any) return false;
        memcpy(a,b,sizeof(a));
    }
    for(int r=0;r<n;++r) if(a[r*m+m-1]) return true;
    return false;
}
int main() {
    if(!(cin>>n>>m)) return 0;
    for(int r=0;r<n;++r) cin>>grid[r];
    const int dr[]={-1,1,0,0},dc[]={0,0,-1,1};
    for(int r=0;r<n;++r) for(int c=0;c<m;++c)
        for(int d=0;d<4;++d) {
            int rr=r+dr[d],cc=c+dc[d];
            if(rr>=0&&rr<n&&cc>=0&&cc<m) adj[r*m+c][deg[r*m+c]++]=rr*m+cc;
        }
    for(int x=1;x<=3999;++x) {
        string s=roman(x);
        if(possible(s)) {cout<<s<<'\n';return 0;}
    }
    cout<<"NO\n";
    return 0;
}
