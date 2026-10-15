// POJ 4005 - Moles
// Model: gpt-6.1-sol
// The insertion BST is the min Cartesian tree of insertion times in key order.
// Build it with a monotone stack, then stream its left-first Euler walk to KMP.
// Every arrival is recorded, including returns to a parent; gifts are delivered
// in inorder, so a shortest valid walk visits each child subtree left then right.
#include <cstdio>
#include <cstring>
const int N=600005;
int tim[N],lc[N],rc[N],par[N],st[N],pi[7005];
char pat[7005];
int readint(){int c=getchar(),x=0;while(c<=32 && c!=EOF)c=getchar();while(c>32 && c!=EOF){x=x*10+c-'0';c=getchar();}return x;}
int main(){
 int cases=readint();
 for(int cs=1;cs<=cases;++cs){
  int n=readint();
  for(int i=1;i<=n;++i){int x=readint();tim[x]=i;lc[i]=rc[i]=par[i]=0;}
  int top=0;
  for(int i=1;i<=n;++i){
   int last=0;
   while(top && tim[st[top-1]]>tim[i])last=st[--top];
   if(top){rc[st[top-1]]=i;par[i]=st[top-1];}
   if(last){lc[i]=last;par[last]=i;}
   st[top++]=i;
  }
  scanf("%7000s",pat);int m=(int)strlen(pat);pi[0]=0;
  for(int i=1,j=0;i<m;++i){while(j && pat[i]!=pat[j])j=pi[j-1];if(pat[i]==pat[j])++j;pi[i]=j;}
  int cur=n?st[0]:0,prev=0,matched=0,ans=0;
  while(cur){
   char c=char('0'+(cur&1));
   while(matched && pat[matched]!=c)matched=pi[matched-1];
   if(pat[matched]==c)++matched;
   if(matched==m){++ans;matched=pi[matched-1];}
   int next;
   if(prev==par[cur])next=lc[cur]?lc[cur]:(rc[cur]?rc[cur]:par[cur]);
   else if(prev==lc[cur])next=rc[cur]?rc[cur]:par[cur];
   else next=par[cur];
   prev=cur;cur=next;
  }
  printf("Case #%d: %d\n",cs,ans);
 }
 return 0;
}
