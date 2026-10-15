// POJ 3198 - Polygon Encoder
// Model: gpt-6.1-sol
// Invert the diagonal pairing with exact big-integer square roots, then
// decode the right-nested vertex tuple and apply the shoelace formula.
// The figure gives P(i,j)=(i+j)(i+j+1)/2+j. No numeric bound is stated,
// so encoded values, coordinates, and twice the area all remain exact.
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
struct Big {
    vector<int> a;
    Big(int x=0) { do {a.push_back(x%10000);x/=10000;} while(x); }
    Big(string s) { for(int i=(int)s.size();i>0;i-=4) {int x=0;for(int j=(i>4?i-4:0);j<i;++j)x=x*10+s[j]-'0';a.push_back(x);} trim(); }
    void trim(){while(a.size()>1 && !a.back())a.pop_back();}
    int cmp(const Big& b) const {if(a.size()!=b.a.size())return a.size()<b.a.size()?-1:1;for(int i=(int)a.size()-1;i>=0;--i)if(a[i]!=b.a[i])return a[i]<b.a[i]?-1:1;return 0;}
    Big operator+(const Big& b)const {Big c;c.a.assign(max(a.size(),b.a.size())+1,0);int carry=0;for(size_t i=0;i<c.a.size();++i){int x=carry+(i<a.size()?a[i]:0)+(i<b.a.size()?b.a[i]:0);c.a[i]=x%10000;carry=x/10000;}c.trim();return c;}
    Big operator-(const Big& b)const {Big c=*this;int borrow=0;for(size_t i=0;i<a.size();++i){int x=a[i]-borrow-(i<b.a.size()?b.a[i]:0);borrow=x<0;if(borrow)x+=10000;c.a[i]=x;}c.trim();return c;}
    Big operator*(const Big& b)const {Big c;c.a.assign(a.size()+b.a.size()+1,0);for(size_t i=0;i<a.size();++i){int carry=0;for(size_t j=0;j<b.a.size()||carry;++j){int x=c.a[i+j]+carry+(j<b.a.size()?a[i]*b.a[j]:0);c.a[i+j]=x%10000;carry=x/10000;}}c.trim();return c;}
    Big half()const {Big c=*this;int rem=0;for(int i=(int)a.size()-1;i>=0;--i){int x=rem*10000+a[i];c.a[i]=x/2;rem=x%2;}c.trim();return c;}
    int integer()const {int x=0;for(int i=(int)a.size()-1;i>=0;--i)x=x*10000+a[i];return x;}
    void print()const {cout<<a.back();for(int i=(int)a.size()-2;i>=0;--i)cout<<setw(4)<<setfill('0')<<a[i];}
};
Big root(const Big& z){
    Big result,rem;
    for(int i=((int)z.a.size()-1)/2;i>=0;--i){
        rem.a.insert(rem.a.begin(),2,0);
        rem.a[0]=z.a[2*i];
        rem.a[1]=2*i+1<(int)z.a.size()?z.a[2*i+1]:0;
        rem.trim();
        Big twice=result*Big(20000);
        int lo=0,hi=10000;
        while(hi-lo>1){int m=(lo+hi)/2;if(((twice+Big(m))*Big(m)).cmp(rem)<=0)lo=m;else hi=m;}
        rem=rem-(twice+Big(lo))*Big(lo);
        result=result*Big(10000)+Big(lo);
    }
    return result;
}
void unpair(const Big& z,Big& x,Big& y){
    Big diagonal=(root(z*Big(8)+Big(1))-Big(1)).half();
    y=z-(diagonal*(diagonal+Big(1))).half();x=diagonal-y;
}
int main(){string s;while(cin>>s && s!="*"){Big n,rest;unpair(Big(s),n,rest);int count=n.integer();vector<Big>x(count),y(count);for(int i=0;i<count;++i){Big point;if(i+1==count)point=rest;else {Big tail;unpair(rest,point,tail);rest=tail;}unpair(point,x[i],y[i]);}Big pos,neg;for(int i=0;i<count;++i){int j=(i+1)%count;pos=pos+x[i]*y[j];neg=neg+y[i]*x[j];}Big area=pos.cmp(neg)>=0?pos-neg:neg-pos;area.half().print();cout<<(area.a[0]%2?".5":".0")<<'\n';}return 0;}
