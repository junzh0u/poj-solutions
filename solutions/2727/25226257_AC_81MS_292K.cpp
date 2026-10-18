// POJ 2727 - Special Judge
// Model: gpt-6-astra
// Validate exactly one output line, allowing only space/tab separators and
// canonical decimal integers in [0, 10^15]. Interval DP finds the optimal
// weighted depth sum and whether it is unique; compare the supplied tree.
// Two base-10^9 limbs keep all comparisons exact in portable C++98.
// Zero frequencies are allowed, including zero for the only node at N=1.
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
struct Number {
    unsigned long hi, lo;
    Number(unsigned long h=0, unsigned long l=0): hi(h), lo(l) {}
};
Number operator+(Number a, Number b) {
    unsigned long s=a.lo+b.lo;
    return Number(a.hi+b.hi+s/1000000000UL, s%1000000000UL);
}
bool operator<(Number a, Number b) {
    return a.hi<b.hi || (a.hi==b.hi && a.lo<b.lo);
}
bool operator==(Number a, Number b) { return a.hi==b.hi && a.lo==b.lo; }
Number w[52], dp[53][53];
bool uniqueBest[53][53];
int leftChild[52], rightChild[52];
Number treeCost(int r, Number &sum) {
    if(r==-1) { sum=Number(); return Number(); }
    Number a,b;
    Number cost=treeCost(leftChild[r],a)+treeCost(rightChild[r],b);
    sum=a+b+w[r];
    return cost+sum;
}
bool parse(const string &s, int n) {
    size_t p=0; int count=0;
    while(p<s.size()) {
        if(s[p]==' ' || s[p]=='\t') { ++p; continue; }
        size_t start=p;
        while(p<s.size() && s[p]>='0' && s[p]<='9') ++p;
        if(p==start || ++count>n) return false;
        string t=s.substr(start,p-start);
        if(t.size()>1 && t[0]=='0') return false;
        if(t.size()>16 || (t.size()==16 && t>"1000000000000000")) return false;
        if(p<s.size() && s[p]!=' ' && s[p]!='\t') return false;
        if(t.size()<=9) w[count]=Number(0,strtoul(t.c_str(),0,10));
        else w[count]=Number(strtoul(t.substr(0,t.size()-9).c_str(),0,10),
                            strtoul(t.substr(t.size()-9).c_str(),0,10));
    }
    return count==n;
}
bool judge(int n) {
    bool child[52]={false};
    for(int i=1;i<=n;++i) {
        if(leftChild[i]!=-1) child[leftChild[i]]=true;
        if(rightChild[i]!=-1) child[rightChild[i]]=true;
        dp[i][i-1]=Number(); uniqueBest[i][i-1]=true;
    }
    dp[n+1][n]=Number(); uniqueBest[n+1][n]=true;
    for(int len=1;len<=n;++len) for(int l=1;l+len-1<=n;++l) {
        int r=l+len-1;
        Number sum;
        for(int k=l;k<=r;++k) sum=sum+w[k];
        for(int k=l;k<=r;++k) {
            Number cost=dp[l][k-1]+dp[k+1][r]+sum;
            if(k==l || cost<dp[l][r]) {
                dp[l][r]=cost;
                uniqueBest[l][r]=uniqueBest[l][k-1] && uniqueBest[k+1][r];
            } else if(cost==dp[l][r]) uniqueBest[l][r]=false;
        }
    }
    int root=1;
    while(child[root]) ++root;
    Number sum;
    return uniqueBest[1][n] && treeCost(root,sum)==dp[1][n];
}
int main() {
    string line;
    while(getline(cin,line) && line!="#End#") {
        if(line!="#Start Input#") continue;
        int n; cin>>n;
        for(int i=1;i<=n;++i) cin>>leftChild[i]>>rightChild[i];
        getline(cin,line);
        getline(cin,line); // #End Input#
        getline(cin,line); // #Start Programmer's Output#
        int lines=0; string output;
        while(getline(cin,line) && line!="#End Programmer's Output#") {
            ++lines;
            if(lines==1) output=line;
        }
        bool ok=lines==1 && parse(output,n) && judge(n);
        cout<<(ok?"Accepted":"Wrong")<<'\n';
    }
    return 0;
}
