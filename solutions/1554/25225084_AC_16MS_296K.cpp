// POJ 1554 - Tournament Brackets
// Model: gpt-6.1-sol
// Reconstruct each match backwards from its winner, then render the tree
// on fixed round coordinates. Absent first-round matches leave empty slots;
// blank rows and trailing spaces are removed only after all lines are drawn.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
string names[32], node[6][32];
int child[32], height;
void build(int u,int depth) {
    height=max(height,depth);
    child[u]=-1;
    for(int p=(u/2)*2-2;p>=0;p-=2)
        if(names[p]==names[u] || names[p+1]==names[u]) {
            child[u]=p; build(p,depth+1); build(p+1,depth+1); break;
        }
}
void place(int u,int depth,int pos) {
    node[height-depth][pos]=names[u];
    if(child[u]>=0) {
        place(child[u],depth+1,pos*2);
        place(child[u]+1,depth+1,pos*2+1);
    }
}
int main() {
    int n,tc=0;
    while(cin>>n && n!=-1) {
        for(int i=0;i<n;++i) cin>>names[i];
        height=0; build(n-1,0);
        for(int r=0;r<6;++r) for(int j=0;j<32;++j) node[r][j]="";
        place(n-1,0,0);
        vector<string> pic(64,string(160,' '));
        int x=0;
        for(int r=0;r<=height;++r) {
            int width=0,step=1<<r,count=1<<(height-r);
            for(int j=0;j<count;++j) if(!node[r][j].empty())
                width=max(width,(int)node[r][j].size()+2);
            for(int j=0;j<count;++j) if(!node[r][j].empty()) {
                int y=step-1+j*2*step;
                string sign="_"+node[r][j];
                sign+=string(width-sign.size(),'_');
                pic[y].replace(x,width,sign);
                if(r<height) for(int k=0;k<step;++k) {
                    int yy=(j%2)?y-k:y+k+1;
                    pic[yy][x+width+k]=(j%2)?'/':'\\';
                }
            }
            x+=width+step;
        }
        cout<<"Tournament "<<++tc<<'\n';
        for(int y=0;y<64;++y) {
            string::size_type last=pic[y].find_last_not_of(' ');
            if(last!=string::npos) cout<<pic[y].substr(0,last+1)<<'\n';
        }
    }
    return 0;
}
