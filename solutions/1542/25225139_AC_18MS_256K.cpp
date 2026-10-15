// POJ 1542 - DNA Translation
// Model: gpt-6.1-sol
// Translate the first start codon in each of four oriented strands.
// RNA candidates are S, reverse(S), reverse(complement(S)), complement(S)
// with T read as U; this is the discussion board's required priority.
// A valid protein has at least one amino acid and an in-frame stop codon.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
const char *amino[64] = {
 "Phe","Phe","Leu","Leu", "Ser","Ser","Ser","Ser",
 "Tyr","Tyr","","", "Cys","Cys","","Trp",
 "Leu","Leu","Leu","Leu", "Pro","Pro","Pro","Pro",
 "His","His","Gln","Gln", "Arg","Arg","Arg","Arg",
 "Ile","Ile","Ile","Met", "Thr","Thr","Thr","Thr",
 "Asn","Asn","Lys","Lys", "Ser","Ser","Arg","Arg",
 "Val","Val","Val","Val", "Ala","Ala","Ala","Ala",
 "Asp","Asp","Glu","Glu", "Gly","Gly","Gly","Gly"
};
int base(char c) { return c=='T'?0:c=='C'?1:c=='A'?2:3; }
string complement(string s) {
 for (size_t i=0;i<s.size();++i)
  s[i]=s[i]=='A'?'T':s[i]=='T'?'A':s[i]=='C'?'G':'C';
 return s;
}
string translate(const string &s) {
 size_t p=s.find("ATG");
 if(p==string::npos) return "";
 string result;
 for(p+=3;p+2<s.size();p+=3) {
  const char *a=amino[base(s[p])*16+base(s[p+1])*4+base(s[p+2])];
  if(!*a) return result;
  if(!result.empty()) result+='-';
  result+=a;
 }
 return "";
}
int main() {
 string s;
 while(cin>>s && s!="*") {
  string candidates[4];
  candidates[0]=s;
  reverse(s.begin(),s.end());
  candidates[1]=s;
  candidates[2]=complement(s);
  reverse(s.begin(),s.end());
  candidates[3]=complement(s);
  string answer;
  for(int i=0;i<4 && answer.empty();++i) answer=translate(candidates[i]);
  if(answer.empty()) answer="*** No translatable DNA found ***";
  cout<<answer<<'\n';
 }
 return 0;
}
