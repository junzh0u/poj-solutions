// POJ 3794 - Extended Normal Order Sort
// Model: gpt-6.1-sol
// Scan lexicographic tokens, mapping numbers to the digit position and
// comparing signed magnitudes without bounded integer conversion.
// Signs belong to numbers only when their preceding character is not a digit.
// Numeric tokens sort as '0' against nonnumeric characters, as in the jury code.
#include <iostream>
#include <string>
using namespace std;
bool digit(char c) { return c >= '0' && c <= '9'; }
bool number(const string &s, size_t i) {
    return digit(s[i]) || ((s[i]=='+' || s[i]=='-') &&
        (i==0 || !digit(s[i-1])) && i+1<s.size() && digit(s[i+1]));
}
int rep(const string &s, size_t i) {
    if(number(s,i)) return '0';
    char c=s[i];
    return c>='a' && c<='z' ? c-'a'+'A' : c;
}
string magnitude(const string &s, size_t &i, int &sign) {
    sign=1;
    if(s[i]=='+' || s[i]=='-') { if(s[i]=='-') sign=-1; ++i; }
    while(i<s.size() && s[i]=='0') ++i;
    size_t start=i;
    while(i<s.size() && digit(s[i])) ++i;
    if(start==i) sign=1;
    return s.substr(start,i-start);
}
int compare(const string &a, const string &b) {
    size_t i=0,j=0;
    while(i<a.size() && j<b.size()) {
        int x=rep(a,i), y=rep(b,j);
        if(x!=y) return x<y ? -1:1;
        if(x!='0') { ++i; ++j; continue; }
        int sa,sb;
        string u=magnitude(a,i,sa), v=magnitude(b,j,sb);
        if(sa!=sb) return sa<sb ? -1:1;
        int c=0;
        if(u.size()!=v.size()) c=u.size()<v.size() ? -1:1;
        else if(u!=v) c=u<v ? -1:1;
        if(c) return c*sa;
    }
    return i<a.size() ? 1 : j<b.size() ? -1:0;
}
int main() {
    int n; cin>>n;
    for(int k=1;k<=n;++k) { string a,b; cin>>a>>b; cout<<k<<' '<<compare(a,b)<<'\n'; }
}
