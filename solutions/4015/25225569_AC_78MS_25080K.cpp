// POJ 4015 - Electrification
// Model: gpt-6.1-sol
// Reproduce Anton Akhi's official Northern 2011 jury construction.
// Split each switch into its lamp contributions, route them onto separate
// lanes, XOR equal destinations, and route the resulting lamps to the edge.
// POJ lost its statement; the official archive supplies the specification.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
char c[5000][5000];
struct End {
 int x,y,dest;
 End(int a,int b,int k):x(a),y(b),dest(k){}
 int oddity(){return (x+y+dest)%2;}
 void down(){c[x][y]=c[x][y]=='.'?'\\':'X';x++;y++;}
 void up(){c[x-1][y]=c[x-1][y]=='.'?'/':'X';x--;y++;}
 End* fork(){End* e=new End(x-1,y,dest);c[x-1][y]='L';y++;return e;}
 void join(){c[x][y]='7';y++;}
 void rev(){
 c[x-2][y]='/';c[x-1][y]='L';c[x-2][y+1]='X';c[x-1][y+1]='L';
 c[x-2][y+2]='7';c[x-1][y+2]='/';c[x-2][y+3]='X';c[x-2][y+4]='7';x-=2;y+=5;
 }
 void step(){up();down();}
 void launchUp(){up();up();}
 void reverse(){rev();down();down();}
};
vector<End*> es,ups;
void equalize(){
 int y=0;for(size_t i=0;i<es.size();i++)if(es[i])y=max(y,es[i]->y);
 for(size_t i=0;i<ups.size();i++)y=max(y,ups[i]->y);
 for(size_t i=0;i<es.size();i++)if(es[i])while(es[i]->y+2<=y)es[i]->step();
}
int main(){
 for(int i=0;i<5000;i++)fill(c[i],c[i]+5000,'.');
 int n,m;if(scanf("%d%d",&n,&m)!=2)return 0;char a[10][11];int count[10]={0};
 for(int i=0;i<n;i++){scanf("%s",a[i]);for(int j=0;j<m;j++)count[i]+=a[i][j]=='1';}
 es.resize(m,(End*)0);int s=0;
 for(int i=0;i<n;i++){
 int k=0;End* e=new End(i,0,-1);e->down();
 for(int j=0;j<count[i]-1;j++){
 while(e->x<4*(s+m+1))e->down();while(a[i][k]!='1')k++;
 End* f=e->fork();f->dest=k++;ups.push_back(f);s++;
 }
 while(e->x<4*(s+m+1)-1)e->down();while(a[i][k]!='1')k++;
 e->dest=k;ups.push_back(e);s++;
 }
 for(size_t i=0;i<ups.size();i++)if((ups[i]->x+ups[i]->y)%2)ups[i]->reverse();
 equalize();int y=0;
 for(size_t i=0;i<ups.size();i++)y=max(y,ups[i]->y);
 for(size_t i=0;i<ups.size();i++){End* e=ups[i];while(e->y<y)e->step();e->launchUp();}
 while(!ups.empty()){
 equalize();
 for(size_t j=0;j<ups.size();j++){
 End* e=ups[j];e->up();e->up();
 if(e->x==4*e->dest+3){
 if(es[e->dest]==0||es[e->dest]->dest!=e->dest){
 if(es[e->dest]){
 int k=e->dest;End* f=es[k++];
 while(f){f->down();f->down();f->down();f->down();End* t=es[k];es[k++]=f;f=t;}
 }
 e->reverse();es[e->dest]=e;
 }else{
 while(es[e->dest]->y<e->y)es[e->dest]->step();e->up();
 if(c[e->x][e->y-1]!='X'){e->step();es[e->dest]->step();}
 if(es[e->dest]->y<e->y+1)es[e->dest]->step();e->join();s--;
 }
 size_t z=0;while(ups[z]->x!=e->x||ups[z]->y!=e->y||ups[z]->dest!=e->dest)z++;ups.erase(ups.begin()+z);
 }else{e->up();e->up();}
 }
 }
 equalize();
 for(size_t i=0;i<es.size();i++)if(es[i]&&es[i]->oddity()==0){es[i]->rev();es[i]->up();}
 equalize();y=0;
 for(size_t i=0;i<es.size();i++)if(es[i])y=max(y,es[i]->y);
 for(size_t i=0;i<es.size();i++)if(es[i])while(es[i]->y<y){if(es[i]->x==es[i]->dest)es[i]->down();else es[i]->up();}
 while(true){bool good=true;for(size_t i=0;i<es.size();i++)if(es[i])good=good&&es[i]->x==es[i]->dest;if(good)break;
 for(size_t i=0;i<es.size();i++)if(es[i]){if(es[i]->x==es[i]->dest)es[i]->down();else es[i]->up();}}
 int h=0,w=0;for(int i=0;i<5000;i++)for(int j=0;j<5000;j++)if(c[i][j]!='.'){h=max(h,i+1);w=max(w,j+1);}
 printf("%d %d\n",h,w);for(int i=0;i<h;i++){fwrite(c[i],1,w,stdout);puts("");}return 0;
}
