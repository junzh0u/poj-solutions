// POJ 1561 - Another Puzzling Problem
// Model: gpt-6.1-sol
// Propagate edge compatibility and unique-piece constraints, then branch on
// the position with the fewest remaining pieces. Zero edges occur only on
// the outer border; pieces keep their given orientation and image spaces.
#include <iostream>
#include <string>
#include <bitset>
using namespace std;
typedef bitset<100> Set;
int n,h,w,m,edge[100][4],neighbor[100][4];
string image[100][25];
Set support[100][4];
bool search(Set *domain) {
    bool changed=true;
    while(changed) {
        changed=false;
        for(int p=0;p<m;++p) {
            if(domain[p].none()) return false;
            if(domain[p].count()==1) {
                for(int q=0;q<m;++q) if(q!=p && (domain[q]&domain[p]).any()) {
                    domain[q]&=~domain[p]; changed=true;
                    if(domain[q].none()) return false;
                }
            }
            for(int d=0;d<4;++d) {
                int q=neighbor[p][d]; if(q<0) continue;
                for(int a=0;a<m;++a) if(domain[p][a] && (support[a][d]&domain[q]).none()) {
                    domain[p].reset(a); changed=true;
                }
                if(domain[p].none()) return false;
            }
        }
    }
    int p=-1; size_t best=101;
    for(int q=0;q<m;++q) if(domain[q].count()>1 && domain[q].count()<best) {
        p=q; best=domain[q].count();
    }
    if(p<0) return true;
    for(int a=0;a<m;++a) if(domain[p][a]) {
        Set next[100]; for(int q=0;q<m;++q) next[q]=domain[q];
        next[p].reset(); next[p].set(a);
        if(search(next)) { for(int q=0;q<m;++q) domain[q]=next[q]; return true; }
    }
    return false;
}
int main() {
    if(!(cin>>n>>h>>w)) return 0;
    m=n*n; string line; getline(cin,line);
    for(int a=0;a<m;++a) {
        for(int r=0;r<h;++r) {
            getline(cin,line);
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            image[a][r]=line;
        }
        for(int d=0;d<4;++d) cin>>edge[a][d];
        getline(cin,line);
        if(a+1<m) getline(cin,line); // Exactly one separator, never an image row.
    }
    int opposite[4]={2,3,0,1};
    for(int a=0;a<m;++a) for(int d=0;d<4;++d)
        for(int b=0;b<m;++b) if(a!=b && edge[a][d]!=0 && edge[a][d]+edge[b][opposite[d]]==0)
            support[a][d].set(b);
    Set domain[100];
    for(int p=0;p<m;++p) {
        int r=p/n,c=p%n;
        neighbor[p][0]=r ? p-n : -1; neighbor[p][1]=c ? p-1 : -1;
        neighbor[p][2]=r+1<n ? p+n : -1; neighbor[p][3]=c+1<n ? p+1 : -1;
        for(int a=0;a<m;++a) {
            bool ok=true;
            for(int d=0;d<4;++d) if((edge[a][d]==0)!=(neighbor[p][d]<0)) ok=false;
            if(ok) domain[p].set(a);
        }
    }
    if(!search(domain)) return 0;
    for(int r=0;r<n;++r) for(int y=0;y<h;++y) {
        for(int c=0;c<n;++c) for(int a=0;a<m;++a) if(domain[r*n+c][a]) cout<<image[a][y];
        cout<<'\n';
    }
    return 0;
}
