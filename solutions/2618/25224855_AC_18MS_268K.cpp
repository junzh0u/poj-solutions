// POJ 2618 - Cube in Labyrinth
// Model: gpt-6.1-sol
// Breadth-first search over the cell and the direction of the original top face.
// Other face identities do not affect moves or the final condition.
// The discussion corrects wall coordinates: v (x,y) blocks (x,y)-(x+1,y),
// and h (x,y) blocks (x,y)-(x,y+1), contrary to the swapped prose.
#include <iostream>
#include <string>
#include <queue>
#include <cstring>
#include <cstdlib>
using namespace std;
struct State { int x,y,f; State(int a,int b,int c):x(a),y(b),f(c){} };
int main() {
    int X,Y,A,B,C,D;
    if (!(cin>>X>>Y>>A>>B>>C>>D)) return 0;
    bool wall[12][12][4]; memset(wall,0,sizeof(wall));
    string s; char mode='v';
    while(cin>>s) {
        if(s=="v" || s=="h") { mode=s[0]; continue; }
        int x=atoi(s.c_str()),y; if(!(cin>>y)) break;
        if(x<1 || x>10 || y<1 || y>10) continue;
        if(mode=='v') {wall[x][y][0]=true;wall[x+1][y][1]=true;}
        else {wall[x][y][2]=true;wall[x][y+1][3]=true;}
    }
    const int vx[6]={0,0,1,-1,0,0},vy[6]={0,0,0,0,1,-1},vz[6]={1,-1,0,0,0,0};
    int turn[6][4];
    for(int f=0;f<6;++f) for(int d=0;d<4;++d) {
        int x=vx[f],y=vy[f],z=vz[f],a=x,b=y,c=z;
        if(d==0){a=z;c=-x;} if(d==1){a=-z;c=x;}
        if(d==2){b=z;c=-y;} if(d==3){b=-z;c=y;}
        for(int g=0;g<6;++g) if(vx[g]==a && vy[g]==b && vz[g]==c) turn[f][d]=g;
    }
    int dist[12][12][6]; memset(dist,-1,sizeof(dist));
    const int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
    queue<State> q; q.push(State(A,B,0));dist[A][B][0]=0;
    while(!q.empty()) {
        State p=q.front();q.pop(); int v=dist[p.x][p.y][p.f];
        if(p.x==C && p.y==D && p.f==0){cout<<v<<'\n';return 0;}
        for(int d=0;d<4;++d) {
            int x=p.x+dx[d],y=p.y+dy[d],f=turn[p.f][d];
            if(x<1 || x>X || y<1 || y>Y || wall[p.x][p.y][d] || dist[x][y][f]!=-1)continue;
            dist[x][y][f]=v+1;q.push(State(x,y,f));
        }
    }
    cout<<"no\n";
    return 0;
}
