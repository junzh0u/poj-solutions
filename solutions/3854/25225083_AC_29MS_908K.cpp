// POJ 3854 - Another Brick in the Wall
// Model: gpt-6.1-sol
// Support edges form a DAG rooted at the ground. Removing a brick makes
// exactly its dominator-tree descendants fall. In bottom-up order, each
// immediate dominator is the LCA of all existing supports; empty cells
// are not supports. Sum brick lengths in each dominator subtree.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
const int MAXB=10001, LOG=14;
int up[MAXB][LOG], dep[MAXB], weight[MAXB], cell[100][100];
int lca(int a,int b) {
    if(dep[a]<dep[b]) swap(a,b);
    int d=dep[a]-dep[b];
    for(int k=0;k<LOG;++k) if(d&(1<<k)) a=up[a][k];
    if(a==b) return a;
    for(int k=LOG-1;k>=0;--k) if(up[a][k]!=up[b][k]) {
        a=up[a][k]; b=up[b][k];
    }
    return up[a][0];
}
int main() {
    int m,n;
    while(cin>>m>>n && (m||n)) {
        string row[100];
        for(int r=0;r<m;++r) cin>>row[r];
        int count=0;
        dep[0]=0; weight[0]=0;
        for(int k=0;k<LOG;++k) up[0][k]=0;
        for(int r=m-1;r>=0;--r) for(int c=0;c<n;) {
            int len=row[r][c]-'0';
            if(!len) {cell[r][c++]=0; continue;}
            int parent=-1;
            if(r==m-1) parent=0;
            else for(int x=c;x<c+len;++x) if(cell[r+1][x]) {
                int s=cell[r+1][x];
                parent=parent<0?s:lca(parent,s);
            }
            int id=++count;
            if(parent<0) parent=0; // Correct input guarantees support.
            up[id][0]=parent; dep[id]=dep[parent]+1; weight[id]=len;
            for(int k=1;k<LOG;++k) up[id][k]=up[up[id][k-1]][k-1];
            for(int x=c;x<c+len;++x) cell[r][x]=id;
            c+=len;
        }
        int answer=0;
        for(int id=count;id>0;--id) {
            answer=max(answer,weight[id]);
            weight[up[id][0]]+=weight[id];
        }
        cout<<answer<<'\n';
    }
    return 0;
}
