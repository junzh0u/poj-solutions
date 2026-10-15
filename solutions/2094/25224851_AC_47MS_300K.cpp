// POJ 2094 - Angry Teacher
// Model: gpt-6.1-sol
// Evaluate n+1 initial values modulo 10^m, then advance forward differences.
// Leading zero digits contribute zero; only the last m digits are retained.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int limbs, topbase;
struct Big {
    int d[250];
    Big() { memset(d, 0, sizeof(d)); }
    void trim() { d[limbs-1] %= topbase; }
    void read(const string& s) {
        int p = (int)s.size()-1;
        for (int i=0; i<limbs && p>=0; ++i) {
            int mul=1;
            for (int j=0; j<4 && p>=0; ++j, --p) {
                d[i] += (s[p]-'0')*mul; mul*=10;
            }
        }
        trim();
    }
};
Big add(const Big& a, const Big& b) {
    Big c; int carry=0;
    for(int i=0;i<limbs;++i) {
        int t=a.d[i]+b.d[i]+carry;
        c.d[i]=t%10000; carry=t/10000;
    }
    c.trim(); return c;
}
Big sub(const Big& a, const Big& b) {
    Big c; int borrow=0;
    for(int i=0;i<limbs;++i) {
        int t=a.d[i]-b.d[i]-borrow;
        borrow=t<0; if(borrow) t+=10000;
        c.d[i]=t;
    }
    c.trim(); return c;
}
Big product(const Big& a, const Big& b) {
    Big c;
    for(int i=0;i<limbs;++i) {
        int carry=0;
        for(int j=0;i+j<limbs;++j) {
            int t=c.d[i+j]+a.d[i]*b.d[j]+carry;
            c.d[i+j]=t%10000; carry=t/10000;
        }
    }
    c.trim(); return c;
}
int main() {
    int n,k,m; string s;
    while(cin>>n>>s>>k>>m) {
        limbs=(m+3)/4; topbase=1;
        for(int i=0;i<(m-1)%4+1;++i) topbase*=10;
        Big x; x.read(s); Big a[11], values[11], diff[11], one;
        one.d[0]=1;
        for(int i=0;i<=n;++i) {cin>>s; a[i].read(s);}
        for(int i=0;i<=n;++i) {
            Big y=a[0];
            for(int j=1;j<=n;++j) y=add(product(y,x),a[j]);
            values[i]=y; x=add(x,one);
        }
        for(int order=0;order<=n;++order) {
            diff[order]=values[0];
            for(int j=0;j<n-order;++j) values[j]=sub(values[j+1],values[j]);
        }
        for(int i=0;i<k;++i) {
            int ans=0;
            for(int j=0;j<limbs;++j) {
                int t=diff[0].d[j];
                while(t) {int digit=t%10; ans+=digit*digit; t/=10;}
            }
            cout<<ans<<'\n';
            for(int j=0;j<n;++j) diff[j]=add(diff[j],diff[j+1]);
        }
    }
    return 0;
}
