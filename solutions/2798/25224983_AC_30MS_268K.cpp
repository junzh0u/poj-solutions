// POJ 2798 - Hardwood Cutting
// Model: gpt-6.1-sol
// Repeatedly detach a part with a boundary-to-boundary straight or L path.
// Both arms of an L are sawn from their exposed ends; partial cuts alone
// do not expose new boundary. Any such detachment only enables more cuts.
#include <cstdio>
#include <cstring>
int m,n;
char a[21][22];
bool exposed[21][21],cutedge[21][21][4];
int dr[4]={-1,0,1,0},dc[4]={0,1,0,-1};
bool legal(int r,int c,int d) {
    if(d==0) return r>0 && c>0 && c<n && a[r-1][c-1]!=a[r-1][c];
    if(d==2) return r<m && c>0 && c<n && a[r][c-1]!=a[r][c];
    if(d==1) return c<n && r>0 && r<m && a[r-1][c]!=a[r][c];
    return c>0 && r>0 && r<m && a[r-1][c-1]!=a[r][c-1];
}
bool path(int r,int c,int d,bool turned) {
    if(cutedge[r][c][d]) return false;
    if(legal(r,c,d)) {
        int x=r+dr[d],y=c+dc[d];
        if(exposed[x][y] || path(x,y,d,turned)) {
            cutedge[r][c][d]=cutedge[x][y][(d+2)%4]=true;
            exposed[r][c]=true;
            return true;
        }
    }
    if(!turned) return path(r,c,(d+1)%4,true) || path(r,c,(d+3)%4,true);
    return false;
}
int main() {
    if(scanf("%d%d",&m,&n)!=2) return 0;
    for(int r=0;r<m;++r) scanf("%s",a[r]);
    for(int r=0;r<=m;++r) exposed[r][0]=exposed[r][n]=true;
    for(int c=0;c<=n;++c) exposed[0][c]=exposed[m][c]=true;
    int answer=1;
    bool changed=true;
    while(changed) {
        changed=false;
        for(int r=0;r<=m && !changed;++r)
            for(int c=0;c<=n && !changed;++c) if(exposed[r][c])
                for(int d=0;d<4 && !changed;++d)
                    if(path(r,c,d,false)) { ++answer; changed=true; }
    }
    printf("%d\n",answer);
    return 0;
}
