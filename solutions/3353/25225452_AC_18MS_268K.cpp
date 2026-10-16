// POJ 3353 - Nuevo-Romano
// Model: gpt-6.1-sol
// Store arbitrary-size values in base 1000. Decode each O as multiplying
// the preceding value by 1000, and require canonical re-encoding to match.
// CM is valid per the contest clarification; O may repeat, but is used
// only when the preceding value exceeds three. Lower groups exclude M.
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;
typedef vector<int> Big;
const int val[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};
const char* sym[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
string roman(int n) { string s; for(int i=0;i<13;++i) while(n>=val[i]) {s+=sym[i];n-=val[i];} return s; }
void addsmall(Big& a,int n) {for(size_t i=0;n;++i) {if(i==a.size())a.push_back(0); n+=a[i];a[i]=n%1000;n/=1000;}}
string encode(Big a) { while(a.size()>1 && a.back()==0)a.pop_back(); if(a.size()==1)return roman(a[0]); if(a.size()==2 && a[1]<=3)return roman(a[1]*1000+a[0]); int r=a[0];a.erase(a.begin());return encode(a)+"O"+roman(r); }
int digit(char c) {switch(c){case 'I':return 1;case 'V':return 5;case 'X':return 10;case 'L':return 50;case 'C':return 100;case 'D':return 500;case 'M':return 1000;}return 0;}
bool parse(string s,Big& a) { if(s.empty())return false; a.assign(1,0); string group;
 for(size_t k=0;k<=s.size();++k) {if(k<s.size() && s[k]!='O') {if(!digit(s[k]))return false;group+=s[k];continue;}
 int n=0;for(size_t j=0;j<group.size();++j){int x=digit(group[j]); n+=(j+1<group.size() && x<digit(group[j+1]))?-x:x;}
 if(n<0 || n>3999 || roman(n)!=group)return false;addsmall(a,n);group.clear();if(k<s.size())a.insert(a.begin(),0);
 } return encode(a)==s;
}
int main(){string line;while(getline(cin,line)){string s;for(size_t i=0;i<line.size();++i)if(!isspace((unsigned char)line[i]))s+=(char)toupper((unsigned char)line[i]);if(s.empty())continue;size_t p=s.find('+');Big a,b;if(p==string::npos || s.find('+',p+1)!=string::npos || !parse(s.substr(0,p),a)||!parse(s.substr(p+1),b)){cout<<"INVALID\n";continue;}if(a.size()<b.size())a.resize(b.size(),0);int carry=0;for(size_t i=0;i<a.size();++i){int n=a[i]+(i<b.size()?b[i]:0)+carry;a[i]=n%1000;carry=n/1000;}if(carry)a.push_back(carry);cout<<encode(a)<<'='<<a.back();for(int i=(int)a.size()-2;i>=0;--i)cout<<setw(3)<<setfill('0')<<a[i];cout<<'\n';}}
