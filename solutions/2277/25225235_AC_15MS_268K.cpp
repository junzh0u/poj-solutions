// POJ 2277 - Region Filling
// Model: gpt-6.1-sol
// Validate boundary points in traversal order, then test closure.
// Fill boundary pixels and use half-open even-odd scanline crossings for
// interiors, including self intersections. Only accepted regions affect later
// intersection checks; the official reference checks closure after all steps.
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cstring>
using namespace std;
int main() {
    int rows, cols, count;
    const int dr[8]={-1,-1,0,1,1,1,0,-1};
    const int dc[8]={0,1,1,1,0,-1,-1,-1};
    while(cin>>rows>>cols>>count && rows) {
        vector<string> board(rows,string(cols,'.'));
        for(int k=0;k<count;++k) {
            char fill; int r,c,n;
            cin>>fill>>r>>c>>n; --r; --c;
            int sr=r, sc=c, error=0; char hit='.';
            vector<pair<int,int> > points;
            int cross[48][64]; bool boundary[48][64];
            memset(cross,0,sizeof(cross));
            memset(boundary,0,sizeof(boundary));
            for(int i=0;i<n;++i) {
                char code; cin>>code; int d=code-'A';
                if(!error) {
                    if(r<0 || r>=rows || c<0 || c>=cols) error=1;
                    else if(board[r][c]!='.') {error=2; hit=board[r][c];}
                }
                points.push_back(make_pair(r,c));
                r+=dr[d]; c+=dc[d];
            }
            if(!error && (r!=sr || c!=sc)) error=3;
            if(error) {
                cout<<"REGION "<<fill;
                if(error==1) cout<<" GOES OUTSIDE THE ARRAY\n";
                else if(error==2) cout<<" BOUNDARY INTERSECTS REGION "<<hit<<'\n';
                else cout<<" BOUNDARY IS NOT CLOSED\n";
                continue;
            }
            for(int i=0;i<n;++i) {
                int y=points[i].first, x=points[i].second;
                int yy=points[(i+1)%n].first, xx=points[(i+1)%n].second;
                boundary[y][x]=true;
                if(y<yy) cross[y][x]^=1;
                else if(yy<y) cross[yy][xx]^=1;
            }
            for(int y=0;y<rows;++y) {
                int parity=0;
                for(int x=0;x<cols;++x) {
                    parity^=cross[y][x];
                    if(parity || boundary[y][x]) board[y][x]=fill;
                }
            }
        }
        for(int y=0;y<rows;++y) cout<<board[y]<<'\n';
        cout<<'\n';
    }
    return 0;
}
