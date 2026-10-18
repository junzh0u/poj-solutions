// POJ 3548 - Restoring the digits
// Model: gpt-6-astra
// Search decimal columns from right to left, assigning distinct digits to
// letters and propagating carries. Rewrite A-B=C as B+C=A.
// Leading zeroes are explicitly allowed; literal digits do not reserve digits.
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
string s[3];
int val[26], used[10], n;
int operand[2]={0,1};
bool present[26];
char digit(int k,int p) { return p<(int)s[k].size()?s[k][p]:'0'; }
int value(char c) { return c>='A'&&c<='Z'?val[c-'A']:c-'0'; }
bool solve(int p,int carry,int slot) {
    if(p==n) {
        if(carry) return false;
        for(int k=0;k<2;++k) {
            int x=0;
            const string &a=s[operand[k]];
            for(int j=(int)a.size()-1;j>=0;--j) {
                if(x>99999999) return false;
                x=x*10+value(a[j]);
            }
        }
        return true;
    }
    if(slot<2) {
        char c=digit(slot,p);
        if(value(c)>=0) return solve(p,carry,slot+1);
        for(int d=0;d<10;++d) if(!used[d]) {
            val[c-'A']=d; used[d]=1;
            if(solve(p,carry,slot+1)) return true;
            used[d]=0; val[c-'A']=-1;
        }
        return false;
    }
    int sum=value(digit(0,p))+value(digit(1,p))+carry;
    char c=digit(2,p);
    if(value(c)>=0) return value(c)==sum%10 && solve(p+1,sum/10,0);
    int d=sum%10;
    if(used[d]) return false;
    used[d]=1; val[c-'A']=d;
    if(solve(p+1,sum/10,0)) return true;
    used[d]=0; val[c-'A']=-1;
    return false;
}
int main() {
    string in; if(!(cin>>in)) return 0;
    size_t op=in.find_first_of("+-"), eq=in.find('=');
    s[0]=in.substr(0,op); s[1]=in.substr(op+1,eq-op-1); s[2]=in.substr(eq+1);
    if(in[op]=='-') { swap(s[0],s[2]); operand[0]=2; }
    memset(val,-1,sizeof val);
    for(int k=0;k<3;++k) {
        n=max(n,(int)s[k].size());
        for(size_t j=0;j<s[k].size();++j)
            if(s[k][j]>='A'&&s[k][j]<='Z') present[s[k][j]-'A']=true;
        reverse(s[k].begin(),s[k].end());
    }
    if(solve(0,0,0)) for(int i=0;i<26;++i)
        if(present[i]) cout<<char('A'+i)<<' '<<val[i]<<'\n';
    return 0;
}
