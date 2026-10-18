// POJ 3569 - Equation
// Model: gpt-6-astra
// Keep each subtree as a ratio of linear integer polynomials. Only one
// leaf is X, so no multiplication creates a quadratic term. Record the
// zeros of every divisor: simplifying a reciprocal must not restore an
// excluded input (for example, 1/(1/X) has no zero).
// The sum of absolute numerator and denominator coefficients is at most
// the product of (digit+1) over leaves (2 for X), hence at most 10^15.
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;
typedef long long I;
struct P { I a,b; P(I aa=0,I bb=0):a(aa),b(bb){} };
P mul(P x,P y) { return P(x.a*y.b+x.b*y.a,x.b*y.b); }
P add(P x,P y,int s) { return P(x.a+s*y.a,x.b+s*y.b); }
struct F { P n,d; F(P nn=P(),P dd=P(0,1)):n(nn),d(dd){} };
I gcd(I a,I b) { if(a<0)a=-a; if(b<0)b=-b; while(b){I t=a%b;a=b;b=t;} return a; }
pair<I,I> root(P p) { I a=-p.b,b=p.a,g=gcd(a,b); a/=g;b/=g;if(b<0){a=-a;b=-b;} return make_pair(a,b); }
int main() {
    vector<F> st; vector<pair<I,I> > banned; string t; bool empty=false;
    while(cin>>t) {
        char c=t[0];
        if(c=='X') st.push_back(F(P(1,0)));
        else if(c>='0'&&c<='9') st.push_back(F(P(0,c-'0')));
        else {
            F y=st.back();st.pop_back();F x=st.back();st.pop_back();F z;
            if(c=='+'||c=='-') {z.n=add(mul(x.n,y.d),mul(y.n,x.d),c=='+'?1:-1);z.d=mul(x.d,y.d);}
            else if(c=='*') {z.n=mul(x.n,y.n);z.d=mul(x.d,y.d);}
            else {
                if(y.n.a) banned.push_back(root(y.n));
                else if(!y.n.b) empty=true;
                z.n=mul(x.n,y.d);z.d=mul(x.d,y.n);
            }
            st.push_back(z);
        }
    }
    F f=st.back();
    if(empty) cout<<"NONE\n";
    else if(!f.n.a) cout<<(f.n.b?"NONE":"MULTIPLE")<<'\n';
    else {
        pair<I,I> r=root(f.n); bool ok=true;
        for(unsigned i=0;i<banned.size();++i) if(r==banned[i]) ok=false;
        if(ok) cout<<"X = "<<r.first<<'/'<<r.second<<'\n';
        else cout<<"NONE\n";
    }
}
