// POJ 3024 - The Gossiping System
// Model: gpt-6.1-sol
// Each person belongs to exactly two groups, and each group pair has one
// person: n = g*(g-1)/2. Conversely the edges of K_g form such a system.
// Thus 8*n+1 must be a square. Decimal arithmetic avoids rounding and overflow
// at 10^50; greedily construct the integer square root one digit at a time.
// The allowed g>1 includes g=2, m=1, so n=1 is valid.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
string norm(string a) {
    size_t p=a.find_first_not_of('0');
    return p==string::npos ? "0" : a.substr(p);
}
bool leq(const string &a, const string &b) {
    return a.size()!=b.size() ? a.size()<b.size() : a<=b;
}
string mul(const string &a, const string &b) {
    vector<int> c(a.size()+b.size()+1,0);
    for (int i=(int)a.size()-1;i>=0;--i)
        for (int j=(int)b.size()-1;j>=0;--j)
            c[a.size()-1-i+b.size()-1-j]+=(a[i]-'0')*(b[j]-'0');
    for (size_t i=0;i+1<c.size();++i) { c[i+1]+=c[i]/10; c[i]%=10; }
    string s;
    for (int i=(int)c.size()-1;i>=0;--i) s+=char('0'+c[i]);
    return norm(s);
}
string discriminant(const string &n) {
    string d; int carry=1;
    for (int i=(int)n.size()-1;i>=0;--i) {
        int v=(n[i]-'0')*8+carry;
        d=char('0'+v%10)+d; carry=v/10;
    }
    if(carry) d=char('0'+carry)+d;
    return norm(d);
}
bool valid(string n) {
    string d=discriminant(n), root;
    int len=((int)d.size()+1)/2;
    for(int i=0;i<len;++i) {
        for(int digit=9;digit>=0;--digit) {
            string trial=norm(root+char('0'+digit)+string(len-i-1,'0'));
            if(leq(mul(trial,trial),d)) { root+=char('0'+digit); break; }
        }
    }
    root=norm(root);
    return mul(root,root)==d;
}
int main() {
    int t; cin>>t;
    while(t--) { string n; cin>>n; cout<<(valid(n)?"Yes.":"No.")<<'\n'; }
    return 0;
}
