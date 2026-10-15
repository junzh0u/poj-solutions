// POJ 3527 - Generalized Connect Four
// Model: gpt-6.1-sol
// Exact negamax with alpha-beta, forced threats, and a transposition table.
// Discs fall to the bottom. The sample reverses the documented dimensions;
// the single board's line count and string width determine its actual shape.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
typedef unsigned long long U64;
struct Entry { U64 key; signed char value,flag,best; };
const int SIZE=1<<21;
Entry table[SIZE];
int R,C,b[100],h[10],cnt[300][2],inc[100][20],deg[100],lines=0,possible[2],emptyCount;
U64 z[100][2],hashValue=0,seed=123456789;
U64 rnd(){seed^=seed<<13;seed^=seed>>7;seed^=seed<<17;return seed;}
void change(int p,int who,int delta){
 for(int j=0;j<deg[p];++j){int s=inc[p][j];
  possible[0]-=(cnt[s][1]==0);possible[1]-=(cnt[s][0]==0);
  cnt[s][who]+=delta;
  possible[0]+=(cnt[s][1]==0);possible[1]+=(cnt[s][0]==0);
 }
 hashValue^=z[p][who];b[p]=delta==1?who:-1;
}
bool win(int p,int who){for(int j=0;j<deg[p];++j)if(cnt[inc[p][j]][who]==3&&cnt[inc[p][j]][1-who]==0)return true;return false;}
int search(int who,int alpha,int beta,int left){
 if(!left||(!possible[0]&&!possible[1]))return 0;
 U64 key=hashValue^(who?0x6a09e667f3bcc909ULL:0ULL);
 Entry &e=table[key&(SIZE-1)];int preferred=-1,orig=alpha;
 if(e.flag&&e.key==key){preferred=e.best;if(e.flag==1)return e.value;if(e.flag==2&&e.value>=beta)return e.value;if(e.flag==3&&e.value<=alpha)return e.value;}
 int moves[10],score[10],k=0,forced=-1,threats=0;
 for(int c=0;c<C;++c)if(h[c]>=0){int p=h[c]*C+c;if(win(p,who))return 1;if(win(p,1-who)){forced=c;++threats;}}
 if(threats>1)return -1;
 for(int c=0;c<C;++c)if(h[c]>=0&&(forced<0||forced==c)){
  int p=h[c]*C+c,s=0;
  for(int j=0;j<deg[p];++j){int l=inc[p][j];if(!cnt[l][1-who])s+=1<<(2*cnt[l][who]);}
  s=s*16-abs(C-1-2*c);if(c==preferred)s+=100000;
  int i=k++;while(i>0&&score[i-1]<s){score[i]=score[i-1];moves[i]=moves[i-1];--i;}score[i]=s;moves[i]=c;
 }
 int best=-2,bestMove=-1;
 for(int i=0;i<k;++i){int c=moves[i],p=h[c]*C+c;--h[c];change(p,who,1);
  int v=-search(1-who,-beta,-alpha,left-1);
  change(p,who,-1);++h[c];if(v>best){best=v;bestMove=c;}if(v>alpha)alpha=v;if(alpha>=beta)break;
 }
 e.key=key;e.value=best;e.best=bestMove;e.flag=best<=orig?3:(best>=beta?2:1);return best;
}
int main(){int a,d;if(!(cin>>a>>d))return 0;vector<string> rows;string s;while(cin>>s)rows.push_back(s);R=rows.size();if(!R)return 0;C=rows[0].size();
 memset(b,-1,sizeof(b));int dr[4]={0,1,1,1},dc[4]={1,0,1,-1};
 for(int r=0;r<R;++r)for(int c=0;c<C;++c)for(int t=0;t<4;++t){int rr=r+3*dr[t],cc=c+3*dc[t];if(rr<0||rr>=R||cc<0||cc>=C)continue;for(int k=0;k<4;++k){int p=(r+k*dr[t])*C+c+k*dc[t];inc[p][deg[p]++]=lines;}++lines;}
 possible[0]=possible[1]=lines;int blacks=0,whites=0;for(int p=0;p<R*C;++p)for(int t=0;t<2;++t)z[p][t]=rnd();
 for(int r=0;r<R;++r)for(int c=0;c<C;++c){char x=rows[r][c];if(x=='.')++emptyCount;else {int w=x=='W';change(r*C+c,w,1);if(w)++whites;else ++blacks;}}
 for(int l=0;l<lines;++l)for(int w=0;w<2;++w)if(cnt[l][w]==4){cout<<(w?"White Wins!":"Black Wins!")<<endl;return 0;}
 for(int c=0;c<C;++c){h[c]=R-1;while(h[c]>=0&&b[h[c]*C+c]!=-1)--h[c];}
 int who=blacks==whites?0:1;int ans=search(who,-1,1,emptyCount);if(!ans)cout<<"Draw!"<<endl;else cout<<((ans==1?who:1-who)?"White Wins!":"Black Wins!")<<endl;
}
