// POJ 2285 - The Floor Bricks
// Model: gpt-6.1-sol
// Sweep columns with a three-column, five-row occupancy mask. Fill the
// first uncovered cell, trying every rotation but never a reflection.
// Printed brick rows are reversed into bottom-up floor coordinates.
// Equal placements keep their minimum price, including duplicate input bricks.
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;
typedef long long Cost;
const Cost INF = 4000000000000000000LL;
struct Piece { int mask; Cost price; Piece(int m,Cost p):mask(m),price(p){} };
static Cost dp[32768], prevdp[1024], nextdp[1024];
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int n;
    while (cin >> n && n) {
        vector<int> blocked(n+3,31);
        for(int i=0,h;i<n;++i) { cin>>h; blocked[i]=31^((1<<h)-1); }
        int m; cin>>m;
        map<int,Cost> shapes[5];
        while(m--) {
            Cost price; cin>>price;
            vector<pair<int,int> > cells;
            for(int y=0;y<3;++y) { string s;cin>>s; for(int x=0;x<3;++x) if(s[x]=='#') cells.push_back(make_pair(x,2-y)); }
            for(int rot=0;rot<4;++rot) {
                int minx=10;
                for(size_t j=0;j<cells.size();++j) minx=min(minx,cells[j].first);
                int anchor=10;
                for(size_t j=0;j<cells.size();++j) if(cells[j].first==minx) anchor=min(anchor,cells[j].second);
                for(int row=0;row<5;++row) {
                    int mask=0; bool ok=true;
                    for(size_t j=0;j<cells.size();++j) {
                        int x=cells[j].first-minx, y=cells[j].second-anchor+row;
                        if(y<0||y>=5) {ok=false;break;}
                        mask |= 1<<(x*5+y);
                    }
                    if(ok) {
                        map<int,Cost>::iterator it=shapes[row].find(mask);
                        if(it==shapes[row].end()||price<it->second) shapes[row][mask]=price;
                    }
                }
                for(size_t j=0;j<cells.size();++j) {int x=cells[j].first;cells[j].first=-cells[j].second;cells[j].second=x;}
            }
        }
        vector<Piece> pieces[5];
        for(int r=0;r<5;++r) for(map<int,Cost>::iterator it=shapes[r].begin();it!=shapes[r].end();++it) pieces[r].push_back(Piece(it->first,it->second));
        fill(prevdp,prevdp+1024,INF); prevdp[blocked[0]|(blocked[1]<<5)]=0;
        for(int col=0;col<n;++col) {
            fill(dp,dp+32768,INF);fill(nextdp,nextdp+1024,INF);
            for(int mask=0;mask<1024;++mask) if(prevdp[mask]!=INF) dp[mask|(blocked[col+2]<<10)]=prevdp[mask];
            for(int mask=0;mask<32768;++mask) if(dp[mask]!=INF) {
                if((mask&31)==31) {int out=mask>>5;nextdp[out]=min(nextdp[out],dp[mask]);continue;}
                int row=0;while(mask&(1<<row)) ++row;
                for(size_t j=0;j<pieces[row].size();++j) {
                    const Piece &p=pieces[row][j];
                    if(!(mask&p.mask)) dp[mask|p.mask]=min(dp[mask|p.mask],dp[mask]+p.price);
                }
            }
            copy(nextdp,nextdp+1024,prevdp);
        }
        if(prevdp[1023]==INF) cout<<"Impossible.\n";
        else cout<<"Need at least "<<prevdp[1023]<<" Gil(s).\n";
    }
}
