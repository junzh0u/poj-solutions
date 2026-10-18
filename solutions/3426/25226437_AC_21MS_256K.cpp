// POJ 3426 - Doors and... more doors
// Model: gpt-6-astra
// BFS on (cell, direction blocked by the incoming open door).
// The diagram fixes hinges: east doors 1/2 block north, 3/4 south;
// south doors 1/2 block east, 3/4 west. Odd doors open east/south,
// even doors west/north. All other doors close upon leaving a cell.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int cells=n*n;
    vector<unsigned char> east(cells), south(cells);
    for(int i=0;i<cells;++i) {
        int a,b; scanf("%d%d", &a,&b); east[i]=a; south[i]=b;
    }
    // Directions: east, south, west, north. Start has no blocked exit.
    int start=4*cells;
    vector<int> parent(start+1,-1), q(start+1);
    parent[start]=start;
    int head=0,tail=0,finish=-1;
    q[tail++]=start;
    while(head<tail) {
        int state=q[head++], cell=state==start?0:state/4;
        if(cell==cells-1) {finish=state; break;}
        int blocked=state==start?-1:state%4;
        int row=cell/n,col=cell%n;
        for(int d=0;d<4;++d) {
            if(d==blocked) continue;
            int next=cell,door=0,ban;
            if(d==0 && col+1<n) {next++; door=east[cell];}
            if(d==1 && row+1<n) {next+=n; door=south[cell];}
            if(d==2 && col>0) {next--; door=east[next];}
            if(d==3 && row>0) {next-=n; door=south[next];}
            if(!door || (door%2)!=(d<2?1:0)) continue;
            if(d%2==0) ban=door<=2?3:1;
            else ban=door<=2?0:2;
            int ns=4*next+ban;
            if(parent[ns]!=-1) continue;
            parent[ns]=state; q[tail++]=ns;
        }
    }
    if(finish==-1) {puts("0"); return 0;}
    int length=0;
    for(int s=finish;s!=start;s=parent[s]) q[length++]=s/4;
    puts("1 1");
    while(length) {int c=q[--length]; printf("%d %d\n",c%n+1,c/n+1);}
    return 0;
}
