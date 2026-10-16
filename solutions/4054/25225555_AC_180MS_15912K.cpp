// POJ 4054 - KINA Is Not Abbreviation
// Model: gpt-6.1-sol
// Group equal initial-letter substrings with suffix order and LCP tables.
// Reject groups representing different word sequences or an existing word;
// greedily pack nonoverlapping occurrences and charge the introduction.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
using namespace std;
int n, stride;
string initials;
vector<unsigned short> a,b;
struct Cmp { bool operator()(int x,int y) const {
 int k=a[x*stride+y];
 if(x+k==n || y+k==n) return x+k==n && y+k!=n;
 return initials[x+k]<initials[y+k];
}};
int main(){
 vector<string> w; string s; char ch;
 while(cin.get(ch)){if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) s+=char(tolower(ch)); else if(!s.empty()){w.push_back(s);s.clear();}}
 if(!s.empty())w.push_back(s);
 n=w.size();stride=n+1;
 vector<int> id(n),sum(n+1,0),order(n);map<string,int> ids;set<string> words;
 for(int i=0;i<n;i++){if(!ids.count(w[i])){int t=ids.size();ids[w[i]]=t;}id[i]=ids[w[i]];words.insert(w[i]);initials+=w[i][0];sum[i+1]=sum[i]+w[i].size();order[i]=i;}
 a.assign(stride*stride,0);b.assign(stride*stride,0);
 for(int i=n-1;i>=0;i--)for(int j=n-1;j>=0;j--){if(initials[i]==initials[j])a[i*stride+j]=1+a[(i+1)*stride+j+1];if(id[i]==id[j])b[i*stride+j]=1+b[(i+1)*stride+j+1];}
 sort(order.begin(),order.end(),Cmp());int best=0,where=0,len=0;
 for(int l=1;l<=n/2;l++){
  for(int p=0;p<n;){int q=p+1;while(q<n&&a[order[q-1]*stride+order[q]]>=l)q++;
   if(q-p>=2&&order[p]+l<=n){bool good=true;for(int k=p+1;k<q;k++)if(b[order[k-1]*stride+order[k]]<l){good=false;break;}
    if(good){vector<int> pos(order.begin()+p,order.begin()+q);sort(pos.begin(),pos.end());int count=0,last=-l;for(int k=0;k<(int)pos.size();k++)if(pos[k]>=last+l){count++;last=pos[k];}
     int score=(count-1)*(sum[pos[0]+l]-sum[pos[0]]-l)-l;
     if(score>best&&!words.count(initials.substr(pos[0],l))){best=score;where=pos[0];len=l;}
    }
   }p=q;
  }
 }
 cout<<best<<'\n';if(best){s=initials.substr(where,len);for(int i=0;i<len;i++)s[i]=toupper(s[i]);cout<<s<<'\n';}
}
