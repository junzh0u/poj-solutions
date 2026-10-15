// POJ 1498 - Matching Meetings
// Model: gpt-6.1-sol
// Union all appointments on a non-leap-year quarter-hour calendar, then
// greedily allocate the earliest free weekday intervals from 09:00 to 17:00.
// The planning horizon is the year starting at the current date.
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;
int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
bool busy[366][32];
int ordinal(int m,int d){int x=d-1;for(int i=0;i<m-1;++i)x+=days[i];return x;}
int slot(int t){return ((t/100)*60+t%100-540)/15;}
int main(){
 char wd;int month,date,n,t;
 if(!(cin>>wd>>month>>date>>n>>t))return 0;
 string line;getline(cin,line);
 int start=ordinal(month,date), weekday=string("MTWRF").find(wd);
 while(getline(cin,line)){
  if(!line.empty()&&line[line.size()-1]=='\r')line.erase(line.size()-1);
  if(line=="done")break;
  while(getline(cin,line)){
   if(!line.empty()&&line[line.size()-1]=='\r')line.erase(line.size()-1);
   if(line=="done")break;
   char dw;int m,d,a,b;istringstream in(line);
   if(!(in>>dw>>m>>d>>a>>b))continue;
   int off=(ordinal(m,d)-start+365)%365;
   if(off==0&&dw!=wd)off=365;
   for(int s=slot(a);s<slot(b);++s)if(s>=0&&s<32)busy[off][s]=true;
  }
 }
 int length=t/15;
 for(int off=0;off<365&&n>0&&length>0&&length<=32;++off){
  if((weekday+off)%7>=5)continue;
  int ord=(start+off)%365,m=0;
  while(ord>=days[m])ord-=days[m++];
  int freeCount=0;
  for(int s=0;s<32&&n>0;++s){
   if(busy[off][s])freeCount=0;else ++freeCount;
   if(freeCount==length){
    int minutes=540+(s-length+1)*15;
    cout<<"MTWRF"[(weekday+off)%7]<<' '<<m+1<<' '<<ord+1<<' '
        <<setfill('0')<<setw(2)<<minutes/60<<setw(2)<<minutes%60<<'\n';
    --n;freeCount=0;
   }
  }
 }
 if(n>0)cout<<"No more times available\n";
 return 0;
}
