// POJ 4036 - Cybercrime Donut Investigation
// Model: gpt-6.1-sol
// Buffered input and linear radix sorting support offline x sweeps and two Fenwick maxima cover the four quadrants.
// POJ's empty statement is recovered from its discussion and SWERC 2011:
// minimize Manhattan distance; separate test cases by one blank line.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int NEG=-2147483647;
struct Point { int x,y,id,r; };
void radix(vector<Point>&p,bool byY){
 vector<Point> b(p.size()); static int count[65536];
 for(int shift=0;shift<32;shift+=16){
  fill(count,count+65536,0);
  for(int i=0;i<(int)p.size();i++)count[((byY?p[i].y:p[i].x)>>shift)&65535]++;
  int total=0;for(int i=0;i<65536;i++){int v=count[i];count[i]=total;total+=v;}
  for(int i=0;i<(int)p.size();i++)b[count[((byY?p[i].y:p[i].x)>>shift)&65535]++]=p[i];
  p.swap(b);
 }
}
struct BIT {
 vector<int> t; int n;
 BIT(int m):t(m+1,NEG),n(m){}
 void put(int i,int v){for(;i<=n;i+=i&-i)if(t[i]<v)t[i]=v;}
 int get(int i){int v=NEG;for(;i;i-=i&-i)if(v<t[i])v=t[i];return v;}
};
int nextChar(){static char buf[1<<16];static int pos=0,len=0;if(pos==len){len=fread(buf,1,sizeof(buf),stdin);pos=0;if(!len)return EOF;}return buf[pos++];}
int readInt(){int c=nextChar();while(c<=32&&c!=EOF)c=nextChar();int s=1,v=0;if(c=='-'){s=-1;c=nextChar();}while(c>='0'&&c<='9'){v=v*10+c-'0';c=nextChar();}return s*v;}
int main(){int n;bool first=true;while((n=readInt())!=-1){
 vector<Point> p(n);vector<int> ys;ys.reserve(n+50000);
 for(int i=0;i<n;i++){p[i].x=readInt();p[i].y=readInt();p[i].id=-1;ys.push_back(p[i].y);}
 int q=readInt();p.resize(n+q);vector<int> ans(q,2000000000);
 for(int i=n;i<n+q;i++){p[i].x=readInt();p[i].y=readInt();p[i].id=i-n;ys.push_back(p[i].y);}
 radix(p,true);int m=0,sz=p.size();
 for(int i=0;i<sz;i++){if(i==0||p[i].y!=p[i-1].y)m++;p[i].r=m;}
 radix(p,false);
 for(int dir=0;dir<2;dir++){
 BIT lo(m),hi(m);int at=dir?sz-1:0,step=dir?-1:1;
 while(at>=0&&at<sz){int end=at;while(end>=0&&end<sz&&p[end].x==p[at].x)end+=step;
 for(int k=at;k!=end;k+=step)if(p[k].id<0){int x=dir?-p[k].x:p[k].x;lo.put(p[k].r,x+p[k].y);hi.put(m+1-p[k].r,x-p[k].y);}
 for(int k=at;k!=end;k+=step)if(p[k].id>=0){int x=dir?-p[k].x:p[k].x;int v=lo.get(p[k].r);if(v!=NEG)ans[p[k].id]=min(ans[p[k].id],x+p[k].y-v);v=hi.get(m+1-p[k].r);if(v!=NEG)ans[p[k].id]=min(ans[p[k].id],x-p[k].y-v);}
 at=end;}
 }
 if(!first)puts("");first=false;for(int i=0;i<q;i++)printf("%d\n",ans[i]);
 }return 0;}
