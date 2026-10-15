// POJ 1668 - Taxation With Representation Overdone
// Model: gpt-6.1-sol
// Parse the rate, then enumerate all legal boundaries between the dates.
// Exactly one interpretation is required. The contest date is 09NOV03;
// the official sample and jury output use BAD DATA for invalid input.
#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>
using namespace std;
const char *months[] = {"", "JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
bool digits(const string &s) {
    for (int i=0;i<(int)s.size();++i) if(s[i]<'0'||s[i]>'9') return false;
    return true;
}
int number(const string &s) {
    int n=0;
    for(int i=0;i<(int)s.size();++i) { n=n*10+s[i]-'0'; if(n>1000) return 1001; }
    return n;
}
string trim(string s) {
    while(!s.empty() && (s[0]==' '||s[0]=='\t')) s.erase(0,1);
    while(!s.empty() && (s[s.size()-1]==' '||s[s.size()-1]=='\t'||s[s.size()-1]=='\r')) s.erase(s.size()-1);
    return s;
}
bool date(string s,int &m,int &d) {
    if(s.size()<3||s.size()>5) return false;
    string tail=s.substr(s.size()-2), head=s.substr(0,s.size()-2);
    if(!head.empty()&&head[head.size()-1]=='-') head.erase(head.size()-1);
    if(head.size()<1||head.size()>2||!digits(head)||!digits(tail)) return false;
    m=number(head); d=number(tail);
    return m>=1&&m<=12;
}
int main() {
    string s; int index=0;
    while(getline(cin,s)) {
        ++index; s=trim(s); bool ok=true; int rate=0; size_t p=s.find('%'), slash=s.find('/'), end=0;
        if(p!=string::npos) {
            string a=s.substr(0,p); ok=!a.empty()&&digits(a)&&number(a)<=99;
            if(ok) rate=number(a)*1000; end=p+1;
        } else if(slash!=string::npos&&slash>=1&&slash+1<s.size()) {
            string a=s.substr(0,slash-1); int n=s[slash-1]-'0',d=s[slash+1]-'0';
            ok=digits(a)&&n>=0&&(d==2||d==4||d==8)&&n<d&&number(a)<=99;
            if(ok) rate=number(a)*1000+n*1000/d; end=slash+2;
        } else ok=false;
        string rest=trim(s.substr(end)); int count=0,sm=0,sd=0,em=0,ey=0;
        if(ok) for(int i=1;i<(int)rest.size();++i) {
            // A space run has only one boundary, at its first character.
            if(rest[i-1]==' '||rest[i-1]=='\t') continue;
            int a,b,c,d;
            if(date(trim(rest.substr(0,i)),a,b)&&date(trim(rest.substr(i)),c,d)&&d>=1&&d<=31) {
                ++count; em=a; ey=b; sm=c; sd=d;
            }
        }
        printf("<Kalii Index=%d>",index);
        if(!ok||count!=1) printf("BAD DATA</Kalii>\n");
        else {
            int sy=(sm<11||(sm==11&&sd<9))?4:3;
            printf("\n    <startdate>%02d%s%02d</startdate>\n",sd,months[sm],sy);
            printf("    <rate>%d.%03d</rate>\n",rate/1000,rate%1000);
            printf("    <enddate>15%s%02d</enddate>\n</Kalii>\n",months[em],ey);
        }
    }
}
