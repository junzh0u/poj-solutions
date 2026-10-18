// POJ 3551 - Putin-chess
// Model: gpt-6-astra
// Exhaustive memoized search over surviving original squares. Every legal
// move removes its moving piece, so no survivor ever changes position.
// R is king, K is knight, and C is rook; colors do not restrict movement.
// Remove the union of every contiguous same-color line through the target.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <utility>
using namespace std;
int board[64], color[64], total, best;
char type[64];
int dx[4]={1,0,1,1},dy[4]={0,1,1,-1};
struct Move { int a,b; Move(int x,int y):a(x),b(y){} };
vector<Move> path, answer;
set<pair<unsigned,unsigned> > seen;
bool inside(int x,int y){return x>=0&&x<8&&y>=0&&y<8;}
vector<int> lines(int t,int c){
    bool mark[64]={false};
    for(int d=0;d<4;++d){
        vector<int> v; v.push_back(t);
        for(int s=-1;s<=1;s+=2){
            int x=t%8+s*dx[d],y=t/8+s*dy[d];
            while(inside(x,y)&&board[y*8+x]==c){v.push_back(y*8+x);x+=s*dx[d];y+=s*dy[d];}
        }
        if(v.size()>=5)for(size_t i=0;i<v.size();++i)mark[v[i]]=true;
    }
    vector<int> r;for(int i=0;i<64;++i)if(mark[i])r.push_back(i);return r;
}
bool legal(int a,int b){
    int x=a%8,y=a/8,u=b%8,v=b/8,ax=abs(u-x),ay=abs(v-y);
    if(type[a]=='K')return ax*ay==2;
    if(type[a]=='R')return ax<=1&&ay<=1;
    bool diagonal=ax==ay, straight=ax==0||ay==0;
    if(type[a]=='B'&&!diagonal)return false;
    if(type[a]=='C'&&!straight)return false;
    if(type[a]=='Q'&&!diagonal&&!straight)return false;
    int sx=(u>x)-(u<x),sy=(v>y)-(v<y);
    x+=sx;y+=sy;
    while(x!=u||y!=v){if(board[y*8+x])return false;x+=sx;y+=sy;}
    return true;
}
void dfs(int left){
    if(total-left>best){best=total-left;answer=path;}
    int count[3]={0,0,0};unsigned lo=0,hi=0;
    for(int i=0;i<64;++i)if(board[i]){++count[board[i]];if(i<32)lo|=1u<<i;else hi|=1u<<(i-32);}
    int upper=total-left+(count[1]>=5?count[1]:0)+(count[2]>=5?count[2]:0);
    if(upper<=best||!seen.insert(make_pair(lo,hi)).second)return;
    for(int b=0;b<64;++b)if(!board[b])for(int c=1;c<=2;++c){
        if(count[c]<5||lines(b,c).empty())continue;
        for(int a=0;a<64;++a)if(board[a]==c&&legal(a,b)){
            board[a]=0;
            vector<int> rem=lines(b,c);
            if(!rem.empty()){
                for(size_t j=0;j<rem.size();++j)board[rem[j]]=0;
                path.push_back(Move(a,b));dfs(left-(int)rem.size());path.pop_back();
                for(size_t j=0;j<rem.size();++j)if(rem[j]!=b)board[rem[j]]=c;
            }
            board[a]=c;
        }
    }
}
int main(){
    int w,b;if(scanf("%d%d",&w,&b)!=2)return 0;total=w+b;
    for(int i=0;i<total;++i){char t,x,y;scanf(" %c %c %c",&t,&x,&y);int p=(y-'1')*8+x-'a';type[p]=t;color[p]=board[p]=(i<w?1:2);}
    dfs(total);printf("%d %d\n",best,(int)answer.size());
    for(size_t i=0;i<answer.size();++i){int a=answer[i].a,b=answer[i].b;printf("%c%c%c-%c%c\n",type[a],'a'+a%8,'1'+a/8,'a'+b%8,'1'+b/8);}
}
