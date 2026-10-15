// POJ 2219 - Manifest Destiny
// Model: gpt-6.1-sol
// Simulate groups in identifier order, updating combat and food immediately.
// Keep entry counts and the previous square separately for every group.
// A foodless turn loses ceil(10%); this also applies when movement is blocked,
// an underspecified case for which the discussion reports Accepted behavior.
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
struct Group { int x,y,p,px,py,dead,vis[20][20]; } g[10];
int ground[20][20], food[20][20], occ[20][20];
int dx[4]={0,1,0,-1},dy[4]={-1,0,1,0},h,w;
bool valid(int x,int y) { return x>=0 && x<w && y>=0 && y<h; }
void kill(int id,int year) {
    if(g[id].p<=0) { g[id].p=0; g[id].dead=year; occ[g[id].y][g[id].x]=-1; }
}
int main() {
    string line; bool first=true;
    while(getline(cin,line)) {
        if(line.empty()) continue;
        istringstream start(line); string word; int years; start>>word>>years;
        memset(g,0,sizeof(g)); memset(occ,-1,sizeof(occ));
        memset(food,0,sizeof(food)); memset(ground,0,sizeof(ground));
        int n=0; h=0; w=0;
        while(getline(cin,line) && line!="END" && line!="END\r") {
            istringstream row(line); string cell; int value,x=0;
            while(row>>cell>>value) {
                ground[h][x]=(cell=="M" || cell=="W");
                if(cell=="w") food[h][x]=value;
                if(cell[0]>='0' && cell[0]<='9') {
                    int id=cell[0]-'0'; n=max(n,id+1);
                    g[id].x=x; g[id].y=h; g[id].p=value;
                    g[id].px=g[id].py=-1; g[id].vis[h][x]=1;
                    if(value) occ[h][x]=id;
                }
                ++x;
            }
            w=x; ++h;
        }
        for(int year=1;year<=years;++year) for(int id=0;id<n;++id) {
            Group &a=g[id]; if(!a.p) continue;
            int target=-1;
            for(int d=0;d<4;++d) {
                int x=a.x+dx[d],y=a.y+dy[d];
                if(valid(x,y) && occ[y][x]>=0) { target=occ[y][x]; break; }
            }
            if(target>=0) {
                g[target].p-=(a.p+1)/2; kill(target,year);
                a.p-=(a.p+9)/10;
            } else {
                int eaten=0;
                for(int d=0;d<4;++d) {
                    int x=a.x+dx[d],y=a.y+dy[d];
                    if(valid(x,y)) { int amount=min(food[y][x],a.p-eaten); food[y][x]-=amount; eaten+=amount; }
                }
                if(eaten) a.p+=(33*eaten+99)/100-(a.p-eaten+19)/20;
                else {
                    int best=-1,score=1000000;
                    for(int d=0;d<4;++d) {
                        int x=a.x+dx[d],y=a.y+dy[d];
                        if(!valid(x,y) || ground[y][x]) continue;
                        int s=a.vis[y][x]; if(x==a.px && y==a.py) s*=2;
                        if(s<score) { score=s; best=d; }
                    }
                    if(best>=0) {
                        occ[a.y][a.x]=-1; a.px=a.x; a.py=a.y;
                        a.x+=dx[best]; a.y+=dy[best]; ++a.vis[a.y][a.x]; occ[a.y][a.x]=id;
                    }
                    a.p-=(a.p+9)/10;
                }
            }
            kill(id,year);
        }
        if(!first) cout<<'\n'; first=false;
        for(int id=0;id<n;++id) {
            cout<<id<<' '<<g[id].p<<" ("<<g[id].x<<','<<g[id].y<<')';
            if(!g[id].p) cout<<' '<<g[id].dead;
            cout<<'\n';
        }
    }
    return 0;
}
