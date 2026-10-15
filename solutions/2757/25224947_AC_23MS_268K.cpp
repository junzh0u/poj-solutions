// POJ 2757 - Beautiful Numbers
// Model: gpt-6.1-sol
// Each prefix q has exactly one valid last digit (-digitSum(q)) mod b.
// Positive beautiful numbers use prefixes 1..n; zero is not counted.
// Complete groups of b prefixes permute all possible last digits.
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;
struct Big {
    vector<int> a;
    Big(int x=0) { do { a.push_back(x%10000); x/=10000; } while(x); }
    Big(string s) { for(int i=(int)s.size();i>0;i-=4) { int v=0; for(int j=(i>4?i-4:0);j<i;++j) v=v*10+s[j]-'0'; a.push_back(v); } trim(); }
    void trim() { while(a.size()>1 && a.back()==0) a.pop_back(); }
    bool zero() const { return a.size()==1 && a[0]==0; }
    int div(int b) { int r=0; for(int i=(int)a.size()-1;i>=0;--i) { int v=r*10000+a[i]; a[i]=v/b; r=v%b; } trim(); return r; }
    Big operator+(const Big& y) const { Big z; z.a.clear(); int c=0; for(unsigned i=0;i<a.size() || i<y.a.size() || c;++i) { int v=c+(i<a.size()?a[i]:0)+(i<y.a.size()?y.a[i]:0); z.a.push_back(v%10000); c=v/10000; } if(z.a.empty()) z.a.push_back(0); return z; }
    Big operator*(const Big& y) const { Big z; z.a.assign(a.size()+y.a.size()+1,0); for(unsigned i=0;i<a.size();++i) { int c=0; unsigned j=0; for(;j<y.a.size() || c;++j) { int v=z.a[i+j]+c+(j<y.a.size()?a[i]*y.a[j]:0); z.a[i+j]=v%10000; c=v/10000; } } z.trim(); return z; }
    void print() const { cout<<a.back(); for(int i=(int)a.size()-2;i>=0;--i) cout<<setw(4)<<setfill('0')<<a[i]; cout<<'\n'; }
};
int main() {
    string s; int b; if(!(cin>>s>>b)) return 0;
    Big n(s), m=n; int r=m.div(b); Big t=m; int sum=0;
    while(!t.zero()) sum=(sum+t.div(b))%b;
    int tail=0; for(int j=0;j<=r;++j) tail+=(b-(sum+j)%b)%b;
    Big ans=n*(n+Big(1)); ans.div(2); ans=ans*Big(b);
    ans=ans+m*Big(b*(b-1)/2)+Big(tail); ans.print();
    return 0;
}
