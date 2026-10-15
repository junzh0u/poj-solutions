// POJ 2532 - Burned Calendar
// Model: gpt-6.1-sol
// Render each Gregorian calendar from 1900 through 2100 and search every
// possible rectangle. Weeks are columns, Monday is the first day row;
// each date is right-aligned in its two-character cell.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
bool leap(int y) { return y%400==0 || (y%4==0 && y%100!=0); }
int main() {
    int n,m;
    if (!(cin>>n>>m)) return 0;
    vector<string> piece(m);
    for(int i=0;i<m;++i) cin>>piece[i];
    const char* names[]={"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};
    int lens[]={31,28,31,30,31,30,31,31,30,31,30,31};
    int first=0; bool found=false;
    for(int year=1900;year<=2100;++year) {
        vector<string> a(28,string(73,'.'));
        int weekday=first;
        for(int month=0;month<12;++month) {
            int y=1+month/4*9, x=1+month%4*18;
            string name=names[month];
            for(int k=0;k<(int)name.size();++k) a[y][x+1+k]=name[k];
            int days=lens[month]+(month==1 && leap(year));
            for(int d=1;d<=days;++d) {
                int pos=weekday+d-1, row=y+1+pos%7, col=x+3*(pos/7);
                if(d>=10) a[row][col]=char('0'+d/10);
                a[row][col+1]=char('0'+d%10);
            }
            weekday=(weekday+days)%7;
        }
        bool match=false;
        for(int r=0;r+m<=28 && !match;++r)
            for(int c=0;c+n<=73 && !match;++c) {
                bool ok=true;
                for(int i=0;i<m && ok;++i)
                    for(int j=0;j<n;++j) if(a[r+i][c+j]!=piece[i][j]) {ok=false;break;}
                if(ok) match=true;
            }
        if(match) {cout<<year<<'\n';found=true;}
        first=weekday;
    }
    if(!found) cout<<0<<'\n';
    return 0;
}
