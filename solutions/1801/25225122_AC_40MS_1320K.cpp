// POJ 1801 - Formula Racing
// Model: gpt-6.1-sol
// Simulate each command and every traversed grid cell. Non-road hits
// take effect after finishing the move, without changing crossing speeds.
// Replay each car to print events after its final status, using constant
// auxiliary memory. The sample establishes zero-based turn numbers.
#include <iostream>
#include <vector>
#include <string>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    const int dx[8]={0,1,1,1,0,-1,-1,-1};
    const int dy[8]={-1,-1,0,1,1,1,0,-1};
    int cases; cin>>cases;
    for(int cs=1;cs<=cases;++cs) {
        int w,h; cin>>w>>h; vector<string> grid(h);
        for(int i=0;i<h;++i) cin>>grid[i];
        cout<<"Scenario #"<<cs<<":\n";
        int n; cin>>n;
        while(n--) {
            int x,y,d,m; string commands; cin>>x>>y>>d>>m>>commands;
            int ox=x,oy=y,od=d;
            for(int pass=0;pass<2;++pass) {
            x=ox; y=oy; d=od;
            int speed=0; bool slow=false,crashed=false;
            for(int t=0;t<(int)commands.size()&&!crashed;++t) {
                if(slow) speed=1;
                slow=false;
                char c=commands[t];
                if(c=='a' && speed<m) ++speed;
                if(c=='b' && speed>0) --speed;
                if(c=='l') d=(d+7)%8;
                if(c=='r') d=(d+1)%8;
                for(int k=0;k<speed;++k) {
                    x+=dx[d]; y+=dy[d];
                    char cell=grid[y][x];
                    if(cell=='W') { crashed=true; break; }
                    if(cell=='.') slow=true;
                    if(cell=='s' && pass==1)
                        cout<<"crossing startline: "<<x<<' '<<y<<' '<<d<<' '<<speed<<' '<<t<<'\n';
                }
            }
            if(slow && !crashed) speed=1;
            if(pass==0) {
                cout<<x<<' '<<y<<' '<<d<<' '<<speed;
                if(crashed) cout<<" crashed";
                cout<<'\n';
            }
            }
        }
        cout<<'\n';
    }
}
