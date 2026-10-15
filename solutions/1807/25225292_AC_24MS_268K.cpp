// POJ 1807 - Mayan Dates
// Model: gpt-6.1-sol
// Use a shared day offset for Gregorian, longcount, and ritual dates.
// Astronomical year zero denotes 1 BC and obeys the 400-year leap rule.
// Month-boundary day zero belongs to the following Mayan month.
// Use Pob from the explicit month list despite Pop in the prose examples.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;
bool leap(int y) { return y%4==0 && (y%100!=0 || y%400==0); }
int md(int y,int m) { static int d[]={31,28,31,30,31,30,31,31,30,31,30,31}; return d[m-1]+(m==2 && leap(y)); }
int main() {
    const char* months[]={"Pob","Wo","Sip","Sotz'","Tzek","Xul","Yaxk'in","Mol","Ch'en","Yax","Sak","Keh","Mak","K'ank'in","Muwan","Pax","K'ayab","Kumk'u","Wayeb"};
    const char* days[]={"Imix","Ik'","Ak'bal","K'an","Chikchan","Kimi","Manik'","Lamat","Muluk","Ok","Chuwen","Eb","Ben","Ix","Men","Kib","Kaban","Etz'nab","Kawak","Ahaw"};
    vector<int> start(6001,0);
    for(int i=0;i<6000;++i) start[i+1]=start[i]+365+leap(i-3113);
    int cases; cin>>cases; string line; getline(cin,line);
    for(int tc=1;tc<=cases;++tc) {
        getline(cin,line); int n=0;
        if(line.find('/')!=string::npos) {
            int d,m,y; char era[3]; sscanf(line.c_str(),"%d/%d/%d %2s",&d,&m,&y,era);
            if(era[0]=='B') y=1-y;
            n=start[y+3113]+d-1-224;
            for(int j=1;j<m;++j) n+=md(y,j);
        } else {
            int a,b,c,d,e; sscanf(line.c_str(),"%d.%d.%d.%d.%d",&a,&b,&c,&d,&e);
            n=((((a*20+b)*20+c)*18+d)*20+e);
        }
        int serial=n+224;
        int yi=int(upper_bound(start.begin(),start.end(),serial)-start.begin())-1;
        int y=yi-3113, rem=serial-start[yi], m=1;
        while(rem>=md(y,m)) { rem-=md(y,m); ++m; }
        cout<<"Scenario #"<<tc<<":\n"<<rem+1<<'/'<<m<<'/'<<(y<=0?1-y:y)<<' '<<(y<=0?"BC":"AD")<<'\n';
        int r=n;
        cout<<r/144000<<'.'; r%=144000;
        cout<<r/7200<<'.'; r%=7200;
        cout<<r/360<<'.'; r%=360;
        cout<<r/20<<'.'<<r%20<<'\n';
        int h=(n+348)%365;
        cout<<(n+3)%13+1<<' '<<days[(n+19)%20]<<' '<<h%20<<' '<<months[h/20]<<"\n\n";
    }
}
