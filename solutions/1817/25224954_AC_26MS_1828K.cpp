// POJ 1817 - Traffic Jam
// Model: gpt-6.1-sol
// Breadth-first search over mixed-radix vehicle positions. Every unobstructed
// slide of any positive distance is one edge; a clear exit adds one final move.
// Fixed lanes and lengths are inferred from the original grid.
#include <cstdio>
#include <cstring>
#include <vector>
#include <deque>
using namespace std;
int n, len[10], lane[10], hor[10], mult[10], pos[10];
int solve(int start, int total) {
    vector<unsigned char> seen((total+7)/8,0);
    deque<int> q;
    q.push_back(start); seen[start>>3]|=1<<(start&7);
    int depth=0;
    while(!q.empty()) {
        int count=(int)q.size();
        while(count--) {
            int state=q.front(); q.pop_front();
            int row[6]={0,0,0,0,0,0};
            for(int i=0;i<n;i++) {
                pos[i]=(state/mult[i])%(7-len[i]);
                for(int k=0;k<len[i];k++) {
                    int r=hor[i]?lane[i]:pos[i]+k;
                    int c=hor[i]?pos[i]+k:lane[i];
                    row[r]|=1<<c;
                }
            }
            int mask=63 ^ ((1<<(pos[0]+len[0]))-1);
            if(!(row[lane[0]]&mask)) return depth+1;
            for(int i=0;i<n;i++) for(int dir=-1;dir<=1;dir+=2) {
                for(int step=1;;step++) {
                    int edge=dir<0?pos[i]-step:pos[i]+len[i]+step-1;
                    if(edge<0||edge>=6) break;
                    int r=hor[i]?lane[i]:edge;
                    int c=hor[i]?edge:lane[i];
                    if(row[r]&(1<<c)) break;
                    int next=state+dir*step*mult[i];
                    if(!(seen[next>>3]&(1<<(next&7)))) {
                        seen[next>>3]|=1<<(next&7); q.push_back(next);
                    }
                }
            }
        }
        depth++;
    }
    return -1;
}
int main() {
    int tc=0;
    while(scanf("%d",&n)==1&&n) {
        char b[6][8]; for(int r=0;r<6;r++) scanf("%s",b[r]);
        int start=0,total=1;
        for(int i=0;i<n;i++) {
            char ch=i==0?'x':'a'+i-1;
            int r0=-1,c0=-1,r1=0,c1=0;
            for(int r=0;r<6;r++) for(int c=0;c<6;c++) if(b[r][c]==ch) {
                if(r0<0) {r0=r;c0=c;} r1=r;c1=c;
            }
            hor[i]=(r0==r1); len[i]=hor[i]?c1-c0+1:r1-r0+1;
            lane[i]=hor[i]?r0:c0; mult[i]=total;
            start+=(hor[i]?c0:r0)*mult[i]; total*=7-len[i];
        }
        int ans=solve(start,total); tc++;
        if(ans<0) printf("You are trapped in scenario #%d.\n",tc);
        else printf("Scenario #%d requires %d moves.\n",tc,ans);
    }
    return 0;
}
