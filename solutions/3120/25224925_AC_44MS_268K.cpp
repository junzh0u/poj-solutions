// POJ 3120 - Sudoku
// Model: gpt-6.1-sol
// Solve the unique puzzle, then enumerate column segment permutations and
// optional transposition. A chosen first row fixes the digit relabeling;
// the first column then forces all row choices, whose bands are checked.
// Operations apply to completed solutions, not to the locations of clues.
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int a[9][9], b[9][9], rm[9], cm[9], bm[9];
int perm[6][3], cols[1296][9], nc;
bool solve() {
 int br=-1,bc=-1,bits=0,best=10;
 for(int r=0;r<9;r++) for(int c=0;c<9;c++) if(!b[r][c]) {
  int m=511 & ~(rm[r]|cm[c]|bm[r/3*3+c/3]);
  int cnt=0;for(int x=m;x;x&=x-1)cnt++;
  if(cnt<best){best=cnt;br=r;bc=c;bits=m;}
 }
 if(br<0)return true;
 if(!best)return false;
 int box=br/3*3+bc/3;
 for(int d=0;d<9;d++)if(bits&(1<<d)) {
  int bit=1<<d;b[br][bc]=d+1;rm[br]|=bit;cm[bc]|=bit;bm[box]|=bit;
  if(solve())return true;
  rm[br]^=bit;cm[bc]^=bit;bm[box]^=bit;b[br][bc]=0;
 }
 return false;
}
bool equivalent() {
 for(int tr=0;tr<2;tr++)for(int p=0;p<nc;p++) {
  int v[9][9];
  for(int r=0;r<9;r++)for(int c=0;c<9;c++)v[r][c]=tr?a[cols[p][c]][r]:a[r][cols[p][c]];
  for(int first=0;first<9;first++) {
   int map[10], rowFor[10], bands[3]={-1,-1,-1};
   for(int c=0;c<9;c++)map[v[first][c]]=b[0][c];
   for(int r=0;r<9;r++)rowFor[map[v[r][0]]]=r;
   bool ok=true;
   for(int r=0;r<9&&ok;r++) {
    int sr=rowFor[b[r][0]], band=sr/3;
    if(bands[r/3]<0) {
     for(int k=0;k<r/3;k++)if(bands[k]==band)ok=false;
     bands[r/3]=band;
    }else if(bands[r/3]!=band)ok=false;
    for(int c=1;c<9&&ok;c++)if(map[v[sr][c]]!=b[r][c])ok=false;
   }
   if(ok)return true;
  }
 }
 return false;
}
int main(){
 int q[3]={0,1,2},n=0;do{for(int k=0;k<3;k++)perm[n][k]=q[k];n++;}while(next_permutation(q,q+3));
 for(int s=0;s<6;s++)for(int x=0;x<6;x++)for(int y=0;y<6;y++)for(int z=0;z<6;z++) {
  int ps[3]={x,y,z};for(int i=0;i<9;i++)cols[nc][i]=3*perm[s][i/3]+perm[ps[i/3]][i%3];nc++;
 }
 int t;scanf("%d",&t);while(t--){char str[20];
 for(int r=0;r<9;r++){scanf("%s",str);for(int c=0;c<9;c++)a[r][c]=str[c]-'0';}
 memset(rm,0,sizeof(rm));memset(cm,0,sizeof(cm));memset(bm,0,sizeof(bm));
 for(int r=0;r<9;r++){scanf("%s",str);for(int c=0;c<9;c++){int d=b[r][c]=str[c]-'0';if(d){rm[r]|=1<<(d-1);cm[c]|=1<<(d-1);bm[r/3*3+c/3]|=1<<(d-1);}}}
 solve();puts(equivalent()?"Yes":"No");
 }
 return 0;
}
