// POJ 3610 - Maclaurin Series of Reciprocals
// Model: gpt-6.1-sol
// Clear input denominators with D, obtaining A(x). If A0 = a,
// write [x^k] 1/A(x) = c[k]/a^(k+1). Then c[0]=1 and
// c[k]=-sum(A[i]*a^(i-1)*c[k-i]). The answer is D*c[k]/a^(k+1).
// All denominator primes are at most 100, so reduce by small divisions.
// Input is one truncated series; coefficients are ordinary powers of x.
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;
struct Big {
    vector<int> d; int s;
    Big(int x=0):s(x<0?-1:1) { if(x<0)x=-x; while(x){d.push_back(x%10000);x/=10000;} }
    void trim(){while(!d.empty() && d.back()==0)d.pop_back();if(d.empty())s=1;}
    int cmp(const Big& b) const {if(d.size()!=b.d.size())return d.size()>b.d.size()?1:-1;for(int i=(int)d.size()-1;i>=0;--i)if(d[i]!=b.d[i])return d[i]>b.d[i]?1:-1;return 0;}
    Big operator+(const Big& b) const {
        Big r; if(s==b.s){r.s=s;int carry=0;for(unsigned i=0;i<d.size()||i<b.d.size()||carry;++i){int t=carry+(i<d.size()?d[i]:0)+(i<b.d.size()?b.d[i]:0);r.d.push_back(t%10000);carry=t/10000;}}
        else {const Big *a=this,*v=&b;if(cmp(b)<0){a=&b;v=this;}r.s=a->s;int borrow=0;for(unsigned i=0;i<a->d.size();++i){int t=a->d[i]-borrow-(i<v->d.size()?v->d[i]:0);borrow=t<0;if(borrow)t+=10000;r.d.push_back(t);}}
        r.trim();return r;
    }
    Big operator*(const Big& b) const {
        Big r;if(d.empty()||b.d.empty())return r;r.s=s*b.s;r.d.assign(d.size()+b.d.size(),0);
        for(unsigned i=0;i<d.size();++i){int carry=0;for(unsigned j=0;j<b.d.size();++j){int t=r.d[i+j]+d[i]*b.d[j]+carry;r.d[i+j]=t%10000;carry=t/10000;}r.d[i+b.d.size()]=carry;}
        r.trim();return r;
    }
    int mod(int p) const {int r=0;for(int i=(int)d.size()-1;i>=0;--i)r=(r*10000+d[i])%p;return r;}
    void divide(int p){int r=0;for(int i=(int)d.size()-1;i>=0;--i){int t=r*10000+d[i];d[i]=t/p;r=t%p;}trim();}
    void print() const {if(d.empty()){printf("0");return;}if(s<0)printf("-");printf("%d",d.back());for(int i=(int)d.size()-2;i>=0;--i)printf("%04d",d[i]);}
};
int main(){
    int n;if(scanf("%d",&n)!=1)return 0;
    int p[100],q[100],de[101]={0},ae[101]={0};char str[100];
    for(int i=0;i<n;++i){scanf("%99s",str);p[i]=atoi(str);char* slash=0;for(char* t=str;*t;++t)if(*t=='/')slash=t;q[i]=slash?atoi(slash+1):1;int x=q[i];for(int v=2;v<=100;++v){int e=0;while(x%v==0){x/=v;++e;}if(e>de[v])de[v]=e;}}
    Big D(1);for(int v=2;v<=100;++v)for(int e=0;e<de[v];++e)D=D*Big(v);
    Big A[100],T[100],c[100];for(int i=0;i<n;++i){A[i]=D;A[i].divide(q[i]);A[i]=A[i]*Big(p[i]);}
    for(int v=2;v<=100;++v)ae[v]=de[v];int x=abs(p[0]);for(int v=2;v<=100;++v)while(x%v==0){x/=v;++ae[v];}x=q[0];for(int v=2;v<=100;++v)while(x%v==0){x/=v;--ae[v];}
    Big power(1);for(int i=1;i<n;++i){T[i]=A[i]*power;power=power*A[0];}
    c[0]=Big(1);
    for(int k=0;k<n;++k){
        if(k){for(int i=1;i<=k;++i)c[k]=c[k]+T[i]*c[k-i];if(!c[k].d.empty())c[k].s=-c[k].s;}
        Big num=D*c[k],den(1);if(A[0].s<0 && (k+1)%2 && !num.d.empty())num.s=-num.s;
        if(!num.d.empty())for(int v=2;v<=100;++v){int e=ae[v]*(k+1);while(e>0 && num.mod(v)==0){num.divide(v);--e;}while(e--)den=den*Big(v);}
        num.print();if(den.cmp(Big(1))!=0){printf("/");den.print();}printf("\n");
    }
    return 0;
}
