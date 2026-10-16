// POJ 3843 - Common Subexpression Elimination
// Model: gpt-6.1-sol
// Intern (label, left subtree, right subtree) bottom-up, then emit each
// distinct subtree once in preorder. References use emitted-node numbers.
// Explicit stacks avoid recursion on trees with 50000 nodes; buffered output
// avoids the per-token output overhead mentioned on the discussion board.
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
const int N=50005, H=131071;
struct Node { int label,l,r,next; } raw[N],node[N];
int canon[N],head[H],stk[4*N],seen[N];
char input[400005];
int main() {
 int cases; if(scanf("%d",&cases)!=1) return 0;
 while(cases--) {
  scanf("%400000s",input);
  int n=0,top=0;
  for(int p=0;input[p];) {
   if(input[p]>='a' && input[p]<='z') {
    int label=0; while(input[p]>='a' && input[p]<='z') label=label*27+input[p++]-'a'+1;
    ++n; raw[n].label=label; raw[n].l=raw[n].r=0;
    if(top) { int parent=stk[top-1]; if(!raw[parent].l) raw[parent].l=n; else raw[parent].r=n; }
    if(input[p]=='(') { stk[top++]=n; ++p; }
   } else { if(input[p]==')') --top; ++p; }
  }
  memset(head,0,sizeof(head)); int count=0;
  canon[0]=0;
  for(int i=n;i>=1;--i) {
   int a=raw[i].label,b=canon[raw[i].l],c=canon[raw[i].r];
   unsigned hash=(unsigned)a*1000003u+(unsigned)b*10007u+(unsigned)c*97u;
   int h=hash%H,k=head[h];
   while(k && (node[k].label!=a || node[k].l!=b || node[k].r!=c)) k=node[k].next;
   if(!k) { k=++count; node[k].label=a; node[k].l=b; node[k].r=c; node[k].next=head[h]; head[h]=k; }
   canon[i]=k;
  }
  memset(seen,0,(count+1)*sizeof(int));
  string out; out.reserve(strlen(input)+1); top=0; stk[top++]=canon[1]; int serial=0;
  while(top) {
   int k=stk[--top];
   if(k<0) { out+=k==-1?',':')'; continue; }
   if(seen[k]) { char num[16]; sprintf(num,"%d",seen[k]); out+=num; continue; }
   seen[k]=++serial; int a=node[k].label,len=0; char label[5];
   while(a) { label[len++]=char('a'+a%27-1); a/=27; }
   while(len) out+=label[--len];
   if(node[k].l) { out+='('; stk[top++]=-2; stk[top++]=node[k].r; stk[top++]=-1; stk[top++]=node[k].l; }
  }
  out+='\n'; fwrite(out.data(),1,out.size(),stdout);
 }
 return 0;
}
