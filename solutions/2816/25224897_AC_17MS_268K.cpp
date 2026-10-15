// POJ 2816 - Right-Hand Rule
// Model: gpt-6.1-sol
// Mark the goal's unobstructed sight lines, then follow the deterministic
// right-hand walk on (cell, direction) states. Memoize terminal outcomes;
// repeated states cannot find a goal. Short input rows are padded with walls.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    int w,h;
    const int dr[4]={-1,0,1,0}, dc[4]={0,1,0,-1};
    while(cin>>w>>h && w>=3 && h>=3) {
        string line; getline(cin,line);
        vector<string> a(h);
        int gr=0,gc=0;
        for(int r=0;r<h;++r) {
            getline(cin,line);
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            a[r]=line.substr(0,w); a[r].resize(w,'X');
            for(int c=0;c<w;++c) if(a[r][c]=='G') {gr=r;gc=c;}
        }
        vector<int> visible(w*h,0), state(w*h*4,0), answer(w*h*4,0);
        visible[gr*w+gc]=1;
        for(int d=0;d<4;++d) {
            int r=gr+dr[d],c=gc+dc[d];
            while(r>=0 && r<h && c>=0 && c<w && a[r][c]!='X') {
                visible[r*w+c]=1; r+=dr[d]; c+=dc[d];
            }
        }
        int total=0,found=0;
        for(int er=0;er<h;++er) for(int ec=0;ec<w;++ec) if(a[er][ec]=='E') {
            ++total;
            int d=er==0?2:er==h-1?0:ec==0?1:3;
            int cur=(er*w+ec)*4+d, result=0;
            vector<int> path;
            while(cur>=0) {
                if(state[cur]==2) {result=answer[cur];break;}
                if(state[cur]==1) break;
                state[cur]=1; path.push_back(cur);
                int cell=cur/4, r=cell/w,c=cell%w; d=cur%4;
                if(visible[cell]) {result=1;break;}
                cur=-1;
                for(int turn=0;turn<4;++turn) {
                    int nd=(d+1-turn+4)%4, nr=r+dr[nd],nc=c+dc[nd];
                    if(nr<0 || nr>=h || nc<0 || nc>=w) break;
                    if(a[nr][nc]!='X') {cur=(nr*w+nc)*4+nd;break;}
                }
            }
            for(size_t i=0;i<path.size();++i) {state[path[i]]=2;answer[path[i]]=result;}
            found+=result;
        }
        cout<<"The goal would be found from "<<found<<" out of "<<total<<" entrances.\n";
    }
    return 0;
}
