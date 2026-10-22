// POJ 4033 - Lanes
// Model: gpt-6-astra
// Remove the reversible lane's service slots one interval at a time.
// A displaced car occupies the next unused slot; its extra wait is the distance.
// Left removals run backwards with a stack, right removals forwards with a finger.
// Drainage after the last arrival is represented by one partial slot and a tail.
// This linear free-capacity method follows the NEERC 2011 jury's model.
import java.io.*;
public class Main {
 static int[] a=new int[100005],b=new int[100005],f=new int[100005],linkto=new int[100005];
 static long[] leftCost=new long[100005];
 static byte[] input=new byte[1048576];
 static int pos;
 static int readInt() {
  while(input[pos]<=32)++pos;
  int x=0;
  while(input[pos]>=48 && input[pos]<=57)x=x*10+input[pos++]-48;
  return x;
 }
 public static void main(String[] args) throws Exception {
  int len=0,n;
  while((n=System.in.read(input,len,input.length-len))>0)len+=n;
 int n1=readInt(),n2=readInt(),m=readInt(),r=readInt();
 if(m==0)return;
 for(int i=0;i<m;++i){a[i]=readInt();b[i]=readInt();}
 int q=0;
 for(int i=0;i<m;++i) {
  q+=a[i];f[i]=n1+1-q;
  if(f[i]<0)f[i]=0;
  q-=n1+1;if(q<0)q=0;
 }
 int tail=m+q/n1,spare=n1-q%n1,cur=m;
 long cost=0;
 for(int i=m-1;i>=0;--i) {
  if(f[i]!=0) {
   if(--f[i]!=0){linkto[i]=cur;cur=i;}
  } else {
   cost+=(cur==m?tail:cur)-i;
   if(cur<m){if(--f[cur]==0)cur=linkto[cur];}
   else if(--spare==0){++tail;spare=n1;}
  }
  leftCost[i]=cost;
 }
 q=0;int cap=n2+1;
 for(int i=0;i<m;++i) {
  q+=b[i];f[i]=cap-q;if(f[i]<0)f[i]=0;
  q-=cap;if(q<0)q=0;
 }
 tail=m+q/cap;spare=cap-q%cap;cur=0;cost=0;
 long best=-1;int answer=1;
 for(int i=0;i<m+r-1;++i) {
  int free=(i<m?f[i]:(i<tail?0:(i==tail?spare:cap)));
  if(free==0) {
   if(cur<=i)cur=i+1;
   while(cur<m && f[cur]==0)++cur;
   if(cur>=m && cur<tail)cur=tail;
   cost+=cur-i;
   if(cur<m)--f[cur];
   else {
    if(cur>tail){tail=cur;spare=cap;}
    if(--spare==0){++tail;spare=cap;}
   }
  }
  if(i>=r-1) {
   int t=i-r+1;long v=leftCost[t]+cost;
   if(best<0 || v<best){best=v;answer=t+1;}
  }
 }
 System.out.println(answer);
 return;
}
}
