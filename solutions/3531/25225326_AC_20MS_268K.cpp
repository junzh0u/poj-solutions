// POJ 3531 - Alternating Sum of Digits
// Model: gpt-6.1-sol
// Concatenate complete digit-prefix blocks, tracking length parity and
// alternating sum. Fixed-length numbers reverse the next start sign only
// when their length is odd. N is read in base K; the answer is signed.
// Small signed big integers keep intermediate block counts exact.
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
struct Big {
    vector<int> a; int s;
    Big(int x=0):s(x<0?-1:1) { if(x<0)x=-x; do {a.push_back(x%10000);x/=10000;}while(x); }
    void norm(){while(a.size()>1 && a.back()==0)a.pop_back();if(a.size()==1 && !a[0])s=1;}
    Big neg()const {Big b=*this;if(b.a.size()!=1||b.a[0])b.s=-b.s;return b;}
    Big operator+(const Big& b)const {
        Big c; c.a.clear();
        if(s==b.s){int carry=0; for(size_t i=0;i<a.size()||i<b.a.size()||carry;i++){int v=carry+(i<a.size()?a[i]:0)+(i<b.a.size()?b.a[i]:0);c.a.push_back(v%10000);carry=v/10000;}c.s=s;}
        else {int cmp=0;if(a.size()!=b.a.size())cmp=a.size()>b.a.size()?1:-1;else for(int i=(int)a.size()-1;i>=0&&!cmp;i--)if(a[i]!=b.a[i])cmp=a[i]>b.a[i]?1:-1;
            if(!cmp)return Big();const Big &x=cmp>0?*this:b,&y=cmp>0?b:*this;int borrow=0;
            for(size_t i=0;i<x.a.size();i++){int v=x.a[i]-borrow-(i<y.a.size()?y.a[i]:0);borrow=v<0;if(borrow)v+=10000;c.a.push_back(v);}c.s=x.s;}
        c.norm();return c;
    }
    Big mul(int n)const {Big c=*this;if(n<0){c=c.neg();n=-n;}int carry=0;for(size_t i=0;i<c.a.size();i++){int v=c.a[i]*n+carry;c.a[i]=v%10000;carry=v/10000;}while(carry){c.a.push_back(carry%10000);carry/=10000;}c.norm();return c;}
    void print()const {if(s<0)cout<<'-';cout<<a.back();for(int i=(int)a.size()-2;i>=0;i--)cout<<setw(4)<<setfill('0')<<a[i];cout<<'\n';}
};
struct Block {Big sum; int parity;Block(Big v=Big(),int p=0):sum(v),parity(p){} };
Block join(Block a,Block b){return Block(a.sum+(a.parity?b.sum.neg():b.sum),a.parity^b.parity);}
int k,L; vector<Big> cnt,tail; vector<int> odd;
Block full(int pos,int prefix){int r=L-pos;Big w=(L%2)?Big(odd[r]):cnt[r];return Block(w.mul(prefix)+tail[pos],(L%2)*odd[r]);}
Block bounded(const string& n,int pos,int prefix){if(pos==L)return Block(Big(prefix),L%2);Block result;int lo=pos?0:1,d=n[pos]-'0';for(int x=lo;x<d;x++)result=join(result,full(pos+1,prefix+(pos%2?-x:x)));return join(result,bounded(n,pos+1,prefix+(pos%2?-d:d)));}
void prepare(int len){L=len;cnt.assign(L+1,Big(1));odd.assign(L+1,1);for(int r=1;r<=L;r++){cnt[r]=cnt[r-1].mul(k);odd[r]=odd[r-1]*(k%2);}tail.assign(L+1,Big());for(int p=L-1;p>=0;p--){Block v;for(int d=0;d<k;d++)v=join(v,full(p+1,p%2?-d:d));tail[p]=v.sum;}}
int main(){string n;while(cin>>k>>n){size_t start=n.find_first_not_of('0');if(start==string::npos){cout<<0<<'\n';continue;}n=n.substr(start);Block answer;for(int len=1;len<(int)n.size();len++){prepare(len);Block v;for(int d=1;d<k;d++)v=join(v,full(1,d));answer=join(answer,v);}prepare((int)n.size());answer=join(answer,bounded(n,0,0));answer.sum.print();}return 0;}
