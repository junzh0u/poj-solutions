// POJ 1210 - City Directions
// Model: gpt-6.1-sol
// Simulate the last passed intersection and heading. A valid turn advances
// to the next intersection; an invalid command leaves the entire state alone.
// Circles waive throughway turn restrictions, but the outgoing road must exist.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
int dx[8]={0,1,1,1,0,-1,-1,-1};
int dy[8]={1,1,0,-1,-1,-1,0,1};
string names[8]={"N","NE","E","SE","S","SW","W","NW"};
bool road(int x,int y,int d){
 if(x+dx[d]<-50||x+dx[d]>50||y+dy[d]<-50||y+dy[d]>50)return false;
 if(d%2==0)return true;
 if(dx[d]==dy[d])return x==y||abs(x-y)==50;
 return x==-y||abs(x+y)==50;
}
bool highway(int x,int y,int d){
 if(d==0||d==4)return x==0||abs(x)==50;
 if(d==2||d==6)return y==0||abs(y)==50;
 return dx[d]==dy[d]?x==y:x==-y;
}
bool circle(int x,int y){return (x==0||abs(x)==50)&&(y==0||abs(y)==50);}
int coord(string s){int n=atoi(s.c_str()+1);return s[s.size()-1]=='W'||s[s.size()-1]=='S'?-n:n;}
int main(){string line;
 while(getline(cin,line)){
  istringstream start(line);string a,b,h;start>>a;if(a=="END")break;if(a.empty())continue;start>>b>>h;
  int x=coord(a),y=coord(b),d=0;while(d<8&&names[d]!=h)++d;
  while(getline(cin,line)){
   istringstream in(line);vector<string> w;string t;while(in>>t)w.push_back(t);
   if(w.size()==1&&w[0]=="STOP")break;
   if(w.empty())continue;
   if(w[0]=="GO"){
    string num;
    if(w.size()==2)num=w[1];else if(w.size()==3&&w[1]=="STRAIGHT")num=w[2];else continue;
    int n=0;bool ok=!num.empty();for(unsigned i=0;i<num.size();++i){if(num[i]<'0'||num[i]>'9'){ok=false;break;} n=n*10+num[i]-'0';if(n>99){ok=false;break;}}
    if(ok&&n>=1){x+=dx[d]*n;y+=dy[d]*n;}
   }else if(w[0]=="TURN"){
    int angle=2;string side;
    if(w.size()==2)side=w[1];else if(w.size()==3&&(w[1]=="HALF"||w[1]=="SHARP")){angle=w[1]=="HALF"?1:3;side=w[2];}else continue;
    if(side!="LEFT"&&side!="RIGHT")continue;
    int delta=side=="LEFT"?-angle:angle,nd=(d+delta+8)%8;
    int nx=x+dx[d],ny=y+dy[d];if(!road(nx,ny,nd))continue;
    if(!circle(nx,ny)){
     if(highway(nx,ny,d)&&delta!=((d%2)?-3:-2))continue;
     if(highway(nx,ny,nd)&&delta!=((nd%2)?-3:-2))continue;
    }
    x=nx;y=ny;d=nd;
   }
  }
  if(highway(x,y,d))cout<<"Illegal stopping place\n";
  else cout<<"A"<<abs(x)<<(x<0?'W':'E')<<" S"<<abs(y)<<(y<0?'S':'N')<<" "<<names[d]<<"\n";
 }
}
