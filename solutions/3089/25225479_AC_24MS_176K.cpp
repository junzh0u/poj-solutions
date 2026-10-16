// POJ 3089 - Sign Message Formatting
// Model: gpt-6.1-sol
// Parse tags while collecting each line's justification blocks, then place them.
// Spacing changes and repeated justification tags do not split a block.
// Newline overflow is checked only when text is emitted; trailing NL is harmless.
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
struct Line { string s[6]; };
int main() {
 int N; cin>>N;
 for(int tc=1;tc<=N;++tc) {
  int R,C; cin>>R>>C; string text; getline(cin,text);
  if(!text.empty()) text.erase(0,1);
  if(!text.empty() && text[text.size()-1]=='\r') text.erase(text.size()-1);
  vector<Line> lines(1); int row=0,j=2,sc=0; string err;
  for(size_t p=0;p<text.size() && err.empty();) {
   char ch=text[p++];
   if((ch=='[' || ch==']') && p<text.size() && text[p]==ch) ++p;
   else if(ch=='[') {
    size_t q=text.find(']',p);
    if(q==string::npos) {err="BAD TAG";break;}
    string tag=text.substr(p,q-p); p=q+1;
    for(size_t k=0;k<tag.size();++k) tag[k]=tolower((unsigned char)tag[k]);
    if(tag=="nl") {++row; lines.push_back(Line());}
    else if(tag=="/sc") sc=0;
    else if(tag.size()==3 && tag.substr(0,2)=="sc" && tag[2]>='0' && tag[2]<='9') sc=tag[2]-'0';
    else if(tag.size()==3 && tag.substr(0,2)=="jl" && tag[2]>='2' && tag[2]<='5') {
     int nj=tag[2]-'0'; bool any=false;
     for(int k=2;k<=5;++k) if(!lines[row].s[k].empty()) any=true;
     if(nj!=j && (((nj==2 || nj==5) && any) || !lines[row].s[5].empty() || (nj==3 && !lines[row].s[4].empty()))) err="TAG CONFLICT";
     j=nj;
    } else err="BAD TAG";
    continue;
   } else if(ch==']') {err="BAD TAG";break;}
   if(row>=R) {err="TOO BIG";break;}
   string &s=lines[row].s[j];
   if(!s.empty() && j!=5) s+=string(sc,' ');
   s+=ch;
   if((int)s.size()>C) err="TOO BIG";
  }
  vector<string> out(R,string(C,' '));
  for(int r=0;r<(int)lines.size() && r<R && err.empty();++r) {
   int last=-2;
   for(int k=2;k<=5;++k) {
    string s=lines[r].s[k]; if(s.empty()) continue;
    if(k==5 && s.size()>1) {
     int gap=(C-(int)s.size())/((int)s.size()-1); string t;
     for(size_t z=0;z<s.size();++z) {if(z) t+=string(gap,' ');t+=s[z];} s=t;
    }
    int start=0;
    if(k==3 || k==5) start=(C-(int)s.size())/2;
    if(k==4) start=C-(int)s.size();
    if(start<=last+1) {err="TOO BIG";break;}
    out[r].replace(start,s.size(),s); last=start+(int)s.size()-1;
   }
  }
  cout<<tc<<'\n';
  if(!err.empty()) cout<<err<<'\n';
  else for(int r=0;r<R;++r) cout<<out[r]<<'\n';
  cout<<'\n';
 }
}
